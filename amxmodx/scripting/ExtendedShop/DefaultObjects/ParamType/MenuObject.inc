#include <json>
#include <ParamsController>
#include <ExtendedShop>

#include "ExtendedShop/Objects/Menu"

DefaultObjects_ParamType_MenuObject_Regsiter() {
    ParamsController_RegSimpleType(EXSHOP_MENU_OBJECT_PARAM_TYPE, "@DefaultObjects_ParamType_MenuObject_OnRead");
}

@DefaultObjects_ParamType_MenuObject_OnRead(const JSON:valueJson) {
    if (!json_is_object(valueJson)) {
        PCJson_LogForFile(valueJson, "WARNING", "Menu value must be an object.", valueJson);
        return false;
    }

    if (!json_object_has_value(valueJson, "Key")) {
        PCJson_LogForFile(valueJson, "WARNING", "Menu must contain 'Key' field.", valueJson);
        return false;
    }
    if (!json_object_has_value(valueJson, "AccountCurrency")) {
        PCJson_LogForFile(valueJson, "WARNING", "Menu must contain 'AccountCurrency' field.", valueJson);
        return false;
    }
    if (!json_object_has_value(valueJson, "Items", JSONArray)) {
        PCJson_LogForFile(valueJson, "WARNING", "Menu must contain 'Items' field.", valueJson);
        return false;
    }
    
    static menuObject[S_Menu];

    PCSingle_ObjShortString(valueJson, "Key", menuObject[Menu_Key], charsmax(menuObject[Menu_Key]), .orFail = true);
    PCSingle_ObjShortString(valueJson, "Title", menuObject[Menu_Title], charsmax(menuObject[Menu_Title]), menuObject[Menu_Key]);
    menuObject[Menu_AccountCurrency] = PCSingle_ObjCell(valueJson, "AccountCurrency", EXSHOP_MWALLET_CURRENCY_PARAM_TYPE, .orFail = true);

    menuObject[Menu_Items] = ArrayCreate(1, 1);
    new JSON:itemsJson = json_object_get_value(valueJson, "Items");
    for (new i = 0, ii = json_array_get_count(itemsJson); i < ii; ++i) {
        new JSON:itemJson = json_array_get_value(itemsJson, i);
        
        new T_ExShop_Product:product = PCSingle_ExShopProduct(itemJson);

        if (product != Invalid_ExShop_Product) {
            ArrayPushCell(menuObject[Menu_Items], product);
        } else {
            PCJson_LogForFile(itemJson, "WARNING", "Menu item #%d is invalid.", i);
        }

        json_free(itemJson);
    }

    return ParamsController_SetCell(Menu_Construct(menuObject));
}
