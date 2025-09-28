#if defined _extended_shop_included
    #endinput
#endif
#define _extended_shop_included

#define EXSHOP_PRODUCT_NAME_MAX_LEN 64
#define EXSHOP_PRODUCT_KEY_MAX_LEN 64

new const EXSHOP_PRODUCTS_FOLDER_PATH[] = "ExtendedShop/Products";
new const EXSHOP_ROOT_CONFIG_PATH[] = "ExtendedShop";
new const EXSHOP_BUY_CMD[] = "exshop_buy";

new const EXSHOP_MWALLET_CURRENCY_PARAM_TYPE[] = "ExShop-MWallet-Currency";
new const EXSHOP_PRODUCT_OBJECT_PARAM_TYPE[] = "ExShop-ProductObject";
new const EXSHOP_PRODUCT_PARAM_TYPE[] = "ExShop-Product";

enum T_ExShop_Product { Invalid_ExShop_Product = -1 }

enum E_ExShop_BuyResult {
    ExShop_Buy_Success,
    ExShop_Buy_NotEnoughMoney,
    ExShop_Buy_CantGiveItems,
}

native ExShop_Init();
