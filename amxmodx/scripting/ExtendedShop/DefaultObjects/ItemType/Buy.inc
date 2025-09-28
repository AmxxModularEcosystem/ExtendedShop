#include <ParamsController>
#include <ItemsController>
#include <ExtendedShop>

DefaultObjects_ItemType_Buy_Regsiter() {
    new T_IC_ItemType:type = IC_ItemType_SimpleRegister(EXSHOP_BUY_ITEM_TYPE, .onGive = "@DefaultObjects_ItemType_Buy_OnGive");
    IC_ItemType_AddParams(type,
        "Product", EXSHOP_PRODUCT_PARAM_TYPE, true
    );
}

@DefaultObjects_ItemType_Buy_OnGive(const playerIndex, const Trie:p) {
    return Product_Buy(PCGet_ExShopProduct(p, "Product"), playerIndex) == ExShop_Buy_Success
        ? IC_RET_GIVE_SUCCESS
        : IC_RET_GIVE_FAIL;
}
