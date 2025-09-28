#include <json>
#include <ParamsController>
#include <ExtendedShop>

#include "ExtendedShop/Objects/Product"

DefaultObjects_ParamType_Product_Regsiter() {
    ParamsController_RegSimpleType(EXSHOP_PRODUCT_PARAM_TYPE, "@DefaultObjects_ParamType_Product_OnRead");
}

@DefaultObjects_ParamType_Product_OnRead(const JSON:valueJson) {
    if (json_is_string(valueJson)) {
        static productKey[EXSHOP_PRODUCT_KEY_MAX_LEN];
        json_get_string(valueJson, productKey, charsmax(productKey));

        new T_ExShop_Product:product = Product_Find(productKey, .orFail = false);
        if (product == Invalid_ExShop_Product) {
            PCJson_LogForFile(valueJson, "WARNING", "Product not found: %s", productKey);
            return false;
        }

        return ParamsController_SetCell(product);
    } else if (json_is_object(valueJson)) {
        new Trie:p = PCSingle_Read(valueJson, EXSHOP_PRODUCT_OBJECT_PARAM_TYPE);
        if (p == Invalid_Trie || !TrieKeyExists(p, PCSINGLE_PARAM_KEY)) {
            PCJson_LogForFile(valueJson, "WARNING", "Error while reading product object.");
            TrieDestroy(p);
            return false;
        }

        new T_ExShop_Product:product = PCGet_Cell(p, PCSINGLE_PARAM_KEY);

        TrieDestroy(p);

        return ParamsController_SetCell(product);
    } else {
        PCJson_LogForFile(valueJson, "WARNING", "Product must be a string or object.");
        return false;
    }
}
