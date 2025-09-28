#include <amxmodx>
#include <json>
#include <ItemsController>
#include <ParamsController>
#include <ModularWallet>
#include <ExtendedShop>

#include "ExtendedShop/Objects/Product"
#include "ExtendedShop/DefaultObjects/Registrar"

public stock const PluginName[] = "Extended Shop";
public stock const PluginVersion[] = "1.0.0";
public stock const PluginAuthor[] = "ArKaNeMaN";

public plugin_precache() {
    PluginInit();
}

PluginInit() {
    static bool:inited = false;
    if (inited) {
        return;
    }
    inited = true;

    register_plugin(PluginName, PluginVersion, PluginAuthor);

    Product_Init();
    Product_LoadFromFolder(PCPath_iMakePath(EXSHOP_PRODUCTS_FOLDER_PATH));

    register_clcmd(EXSHOP_BUY_CMD, "@Cmd_Buy");
}

@Cmd_Buy(const playerIndex) {
    enum { Arg_ProductKey = 1 }

    static productKey[EXSHOP_PRODUCT_KEY_MAX_LEN];
    read_argv(Arg_ProductKey, productKey, charsmax(productKey));

    new T_ExShop_Product:product = Product_Find(productKey, .orFail = false);
    if (product == Invalid_ExShop_Product) {
        client_print(playerIndex, print_chat, "Product not found: %s", productKey);
        return PLUGIN_HANDLED;
    }

    switch (Product_Buy(playerIndex, product)) {
        case ExShop_Buy_NotEnoughMoney: {
            client_print(playerIndex, print_chat, "Not enough money");
        }
        case ExShop_Buy_CantGiveItems: {
            client_print(playerIndex, print_chat, "Can't give items");
        }
        case ExShop_Buy_Success: {
            client_print(playerIndex, print_chat, "Bought: %s", Product_iGetName(product));
        }
    }

    return PLUGIN_HANDLED;
}

#include "ExtendedShop/API/Main"

public plugin_natives() {
    API_Main_RegisterNatives();
}
