#if defined __exshop_src__objects_Menu_included
    #endinput
#endif
#define __exshop_src__objects_Menu_included

#include <amxmodx>
#include <json>
#include <regex>
#include <ModularWallet>
#include <ItemsController>
#include <ParamsController>
#include <ExtendedShop>

enum _:S_Menu {
    Menu_Key[EXSHOP_MENU_KEY_MAX_LEN],
    Menu_Title[EXSHOP_MENU_TITLE_MAX_LEN],
    Array:Menu_Items, // T_ExShop_Product[]

    T_Currency:Menu_AccountCurrency,
}

static Array:Menus = Invalid_Array;
static Trie:MenusMap = Invalid_Trie;

Menu_Init() {
    ParamsController_Init();
    MWallet_Init();
    IC_Init();
    
    Product_Init();

    if (Menus == Invalid_Array) {
        Menus = ArrayCreate(S_Menu, 1);
    }

    if (MenusMap == Invalid_Trie) {
        MenusMap = TrieCreate();
    }
}

Menu_Build(const T_ExShop_Menu:menu, const playerIndex) {
    new menuObject[S_Menu];
    Menu_Get(menu, menuObject);

    new title[256];
    copy(title, charsmax(title), menuObject[Menu_Title]);
    if (menuObject[Menu_AccountCurrency] != Invalid_Currency) {
        format(title, charsmax(title), "%s^n\yБаланс: \w%s", title, MWallet_Currency_AccFmt(menuObject[Menu_AccountCurrency], playerIndex));
    }

    new menuHandler = menu_create(title, "@Menu_Handle");

    for (new i = 0, ii = ArraySize(menuObject[Menu_Items]); i < ii; ++i) {
        new T_ExShop_Product:product = ArrayGetCell(menuObject[Menu_Items], i);

        new productObject[S_Product];
        Product_Get(product, productObject);

        static cmd[128];
        formatex(cmd, charsmax(cmd), "%s %s", EXSHOP_BUY_CMD, productObject[Product_Key]);

        new itemTitle[256];
        formatex(
            itemTitle, charsmax(itemTitle),
            MWallet_Currency_IsEnough(productObject[Product_Currency], playerIndex, productObject[Product_Price]) ? "\w%s \y[\w%s\y]" : "\d%s [%s]",
            productObject[Product_Name], MWallet_Currency_Fmt(productObject[Product_Currency], productObject[Product_Price])
        );

        menu_additem(menuHandler, itemTitle, cmd);
    }

    menu_setprop(menuHandler, MPROP_EXITNAME, "Выход");
    menu_setprop(menuHandler, MPROP_NEXTNAME, "Далее");
    menu_setprop(menuHandler, MPROP_BACKNAME, "Назад");
    return menuHandler;
}

@Menu_Handle(const playerIndex, const menuIndex, const itemIndex) {
    if (itemIndex == MENU_EXIT) {
        return;
    }

    static cmd[128];
    menu_item_getinfo(menuIndex, itemIndex, _, cmd, charsmax(cmd));

    if (cmd[0]) {
        client_cmd(playerIndex, cmd);
    }

    menu_destroy(menuIndex);
}

Menu_LoadFromFolder(const path[]) {
    static pathNonConst[PLATFORM_MAX_PATH];
    copy(pathNonConst, charsmax(pathNonConst), path);

    new file[PLATFORM_MAX_PATH], dir, FileType:fileType;
    dir = open_dir(pathNonConst, file, charsmax(file), fileType);
    if (!dir) {
        abort(AMX_ERR_PARAMS, "[ERROR] Can't open folder '%s'.", path);
        return;
    }

    new Regex:iRegEx_FileName, ret;
    iRegEx_FileName = regex_compile("(.+).json$", ret, "", 0, "i");

    do {
        if (file[0] == '!' || file[0] == '.') {
            continue;
        }

        if (fileType == FileType_Directory) {
            Menu_LoadFromFolder(PCPath_iMakePath(file, path));
        } else if (fileType == FileType_File) {
            if (regex_match_c(file, iRegEx_FileName) > 0) {
                regex_substr(iRegEx_FileName, 1, file, charsmax(file));

                Menu_LoadFromFile(PCPath_iMakePath(file, path, "json"));
            }
        }
    } while (next_file(dir, file, charsmax(file), fileType));

    regex_free(iRegEx_FileName);
    close_dir(dir);
}

Menu_LoadFromFile(const path[]) {
    new JSON:menusJson = PCJson_ParseFile(path, PCPath_iMakePath(EXSHOP_ROOT_CONFIG_PATH));
    if (menusJson == Invalid_JSON) {
        abort(AMX_ERR_PARAMS, "[ERROR] Can't parse file '%s'.", path);
        return;
    }

    Menu_LoadFromJson(menusJson);

    PCJson_Free(menusJson);
}

Menu_LoadFromJson(const JSON:menusJson) {
    if (json_is_object(menusJson)) {
        new Trie:p = PCSingle_Read(menusJson, EXSHOP_MENU_OBJECT_PARAM_TYPE);
        if (p == Invalid_Trie || !TrieKeyExists(p, PCSINGLE_PARAM_KEY)) {
            PCJson_LogForFile(menusJson, "WARNING", "Error while reading menu object.");
        }

        TrieDestroy(p);
    } else if (json_is_array(menusJson)) {
        for (new i = 0, ii = json_array_get_count(menusJson); i < ii; ++i) {
            new JSON:menuJson = json_array_get_value(menusJson, i);
            Menu_LoadFromJson(menuJson);
            json_free(menuJson);
        }
    } else {
        PCJson_LogForFile(menusJson, "WARNING", "Menus file must contain an object or array of objects.");
    }
}

Menu_Get(const T_ExShop_Menu:menu, menuObject[S_Menu]) {
    ArrayGetArray(Menus, _:menu, menuObject);
}

T_ExShop_Menu:Menu_Find(const key[], const bool:orFail = true) {
    if (!TrieKeyExists(MenusMap, key)) {
        if (orFail) {
            abort(AMX_ERR_PARAMS, "Menu not found: %s", key);
        }

        return Invalid_ExShop_Menu;
    }

    new T_ExShop_Menu:menu;
    TrieGetCell(MenusMap, key, menu);

    return menu;
}

T_ExShop_Menu:Menu_Construct(menuObject[S_Menu]) {
    if (menuObject[Menu_Key][0] == '#') {
        set_fail_state("Menu key can't start with '#'.");
        return Invalid_ExShop_Menu;
    }

    if (menuObject[Menu_Title][0] == EOS) {
        copy(menuObject[Menu_Title], charsmax(menuObject[Menu_Title]), menuObject[Menu_Key]);
    }

    new T_ExShop_Menu:menu = T_ExShop_Menu:ArrayPushArray(Menus, menuObject);
    TrieSetCell(MenusMap, menuObject[Menu_Key], menu);
    TrieSetCell(MenusMap, fmt("#%d", menu), menu);

    return menu;
}
