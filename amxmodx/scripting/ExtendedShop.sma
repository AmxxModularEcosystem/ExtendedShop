#include <amxmodx>
#include <json>
#include <ItemsController>
#include <ParamsController>
#include <ModularWallet>
#include <ExtendedShop>

#include "ExtendedShop/Objects/Product"
#include "ExtendedShop/Objects/Menu"
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
    
    log_amx("[INFO] Initialize ExShop.");

    register_plugin(PluginName, PluginVersion, PluginAuthor);

    Product_Init();
    Product_LoadFromFolder(PCPath_iMakePath(EXSHOP_PRODUCTS_FOLDER_PATH));
    
    Menu_Init();
    Menu_LoadFromFolder(PCPath_iMakePath(EXSHOP_MENUS_FOLDER_PATH));

    register_clcmd(EXSHOP_BUY_CMD, "@Cmd_Buy");
    register_clcmd(EXSHOP_MENU_CMD, "@Cmd_Menu");

    log_amx("[INFO] ExShop initialized.");
}

@Cmd_Buy(const playerIndex) {
    enum { Arg_ProductKey = 1 }

    static productKey[EXSHOP_PRODUCT_KEY_MAX_LEN];
    read_argv(Arg_ProductKey, productKey, charsmax(productKey));

    new T_ExShop_Product:product = Product_Find(productKey, .orFail = false);
    if (product == Invalid_ExShop_Product) {
        client_print(playerIndex, print_chat, "Товар не найден: %s", productKey);
        return PLUGIN_HANDLED;
    }

    switch (Product_Buy(playerIndex, product)) {
        case ExShop_Buy_NotEnoughMoney: {
            client_print(playerIndex, print_chat, "У вас недостаточно средств.");
        }
        case ExShop_Buy_CantGiveItems: {
            client_print(playerIndex, print_chat, "Не удалось выдать предмет.");
        }
        case ExShop_Buy_Success: {
            client_print(playerIndex, print_chat, "Вы купили: %s", Product_iGetName(product));
        }
    }

    return PLUGIN_HANDLED;
}

@Cmd_Menu(const playerIndex) {
    enum { Arg_MenuKey = 1 }

    static menuKey[EXSHOP_MENU_KEY_MAX_LEN];
    read_argv(Arg_MenuKey, menuKey, charsmax(menuKey));

    new T_ExShop_Menu:menu = Menu_Find(menuKey, .orFail = false);
    if (menu == Invalid_ExShop_Menu) {
        client_print(playerIndex, print_chat, "Меню не найдено: %s", menuKey);
        return PLUGIN_HANDLED;
    }

    menu_display(playerIndex, Menu_Build(menu, playerIndex));

    return PLUGIN_HANDLED;
}

#include "ExtendedShop/API/Main"
public plugin_natives() {
    API_Main_RegisterNatives();
}
