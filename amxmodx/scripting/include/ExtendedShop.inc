#if defined _extended_shop_included
    #endinput
#endif
#define _extended_shop_included

#include <ParamsController>

#define EXSHOP_PRODUCT_NAME_MAX_LEN 64
#define EXSHOP_PRODUCT_KEY_MAX_LEN 64

#define EXSHOP_MENU_KEY_MAX_LEN 64
#define EXSHOP_MENU_TITLE_MAX_LEN 128

new const EXSHOP_ROOT_CONFIG_PATH[] = "ExtendedShop";
new const EXSHOP_PRODUCTS_FOLDER_PATH[] = "ExtendedShop/Products";
new const EXSHOP_MENUS_FOLDER_PATH[] = "ExtendedShop/Menus";
new const EXSHOP_BUY_CMD[] = "exshop_buy";
new const EXSHOP_MENU_CMD[] = "exshop_menu";

new const EXSHOP_MWALLET_CURRENCY_PARAM_TYPE[] = "ExShop-MWallet-Currency";
new const EXSHOP_PRODUCT_OBJECT_PARAM_TYPE[] = "ExShop-ProductObject";
new const EXSHOP_PRODUCT_PARAM_TYPE[] = "ExShop-Product";
new const EXSHOP_MENU_OBJECT_PARAM_TYPE[] = "ExShop-MenuObject";
new const EXSHOP_BUY_ITEM_TYPE[] = "ExShop-Buy";

enum T_ExShop_Product { Invalid_ExShop_Product = -1 }

enum T_ExShop_Menu { Invalid_ExShop_Menu = -1 }

enum E_ExShop_BuyResult {
    ExShop_Buy_Success,
    ExShop_Buy_NotEnoughMoney,
    ExShop_Buy_CantGiveItems,
}

native ExShop_Init();

stock T_ExShop_Product:PCGet_ExShopProduct(const Trie:p, const key[], const T_ExShop_Product:def = Invalid_ExShop_Product) {
    return PCGet_Cell(p, key, def);
}

stock T_ExShop_Product:PCSingle_ObjExShopProduct(const JSON:objectJson, const key[], const T_ExShop_Product:def = Invalid_ExShop_Product, const bool:dotNot = false, const bool:orFail = false) {
    return PCSingle_ObjCell(objectJson, key, EXSHOP_PRODUCT_PARAM_TYPE, def, dotNot, orFail);
}

stock T_ExShop_Product:PCSingle_ExShopProduct(const JSON:valueJson, const T_ExShop_Product:def = Invalid_ExShop_Product, const orFailKey[] = "") {
    return PCSingle_Cell(valueJson, EXSHOP_PRODUCT_PARAM_TYPE, def, orFailKey);
}
