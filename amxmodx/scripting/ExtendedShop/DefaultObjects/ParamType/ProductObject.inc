#include <json>
#include <ParamsController>
#include <ExtendedShop>

#include "ExtendedShop/Objects/Product"

DefaultObjects_ParamType_ProductObject_Regsiter() {
    ParamsController_RegSimpleType(EXSHOP_PRODUCT_OBJECT_PARAM_TYPE, "@DefaultObjects_ParamType_ProductObject_OnRead");
}

@DefaultObjects_ParamType_ProductObject_OnRead(const JSON:valueJson) {
    if (!json_is_object(valueJson)) {
        PCJson_LogForFile(valueJson, "WARNING", "Product value must be an object.", valueJson);
        return false;
    }

    if (!json_object_has_value(valueJson, "Key")) {
        PCJson_LogForFile(valueJson, "WARNING", "Product must contain 'Key' field.", valueJson);
        return false;
    }
    if (!json_object_has_value(valueJson, "Items")) {
        PCJson_LogForFile(valueJson, "WARNING", "Product must contain 'Items' field.", valueJson);
        return false;
    }
    if (!json_object_has_value(valueJson, "Price")) {
        PCJson_LogForFile(valueJson, "WARNING", "Product must contain 'Price' field.", valueJson);
        return false;
    }
    if (!json_object_has_value(valueJson, "Currency")) {
        PCJson_LogForFile(valueJson, "WARNING", "Product must contain 'Currency' field.", valueJson);
        return false;
    }
    
    static productObject[S_Product];

    PCSingle_ObjShortString(valueJson, "Key", productObject[Product_Key], charsmax(productObject[Product_Key]), .orFail = true);
    PCSingle_ObjShortString(valueJson, "Name", productObject[Product_Name], charsmax(productObject[Product_Name]));
    productObject[Product_Currency] = PCSingle_ObjCell(valueJson, "Currency", EXSHOP_MWALLET_CURRENCY_PARAM_TYPE, .orFail = true);
    productObject[Product_Price] = PCSingle_ObjFloat(valueJson, "Price", .orFail = true);
    productObject[Product_Items] = PCSingle_ObjIcItems(valueJson, "Items", .orFail = true);

    return ParamsController_SetCell(Product_Construct(productObject));
}
