#if defined __exshop_src__objects_product_included
    #endinput
#endif
#define __exshop_src__objects_product_included

#include <amxmodx>
#include <json>
#include <regex>
#include <ModularWallet>
#include <ItemsController>
#include <ParamsController>
#include <ExtendedShop>

enum _:S_Product {
    Product_Key[EXSHOP_PRODUCT_KEY_MAX_LEN],
    Product_Name[EXSHOP_PRODUCT_NAME_MAX_LEN],
    T_Currency:Product_Currency,
    Float:Product_Price,
    Array:Product_Items,
}

static Array:Products = Invalid_Array;
static Trie:ProductsMap = Invalid_Trie;

Product_Init() {
    IC_Init();
    MWallet_Init();
    ParamsController_Init();

    if (Products == Invalid_Array) {
        Products = ArrayCreate(S_Product, 1);
    }

    if (ProductsMap == Invalid_Trie) {
        ProductsMap = TrieCreate();
    }
}

Product_LoadFromFolder(const path[]) {
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
            Product_LoadFromFolder(PCPath_iMakePath(file, path));
        } else if (fileType == FileType_File) {
            if (regex_match_c(file, iRegEx_FileName) > 0) {
                regex_substr(iRegEx_FileName, 1, file, charsmax(file));

                Product_LoadFromFile(PCPath_iMakePath(file, path, "json"));
            }
        }
    } while (next_file(dir, file, charsmax(file), fileType));

    regex_free(iRegEx_FileName);
    close_dir(dir);
}

Product_LoadFromFile(const path[]) {
    new JSON:productsJson = PCJson_ParseFile(path, PCPath_iMakePath(EXSHOP_ROOT_CONFIG_PATH));
    if (productsJson == Invalid_JSON) {
        abort(AMX_ERR_PARAMS, "[ERROR] Can't parse file '%s'.", path);
        return;
    }

    Product_LoadFromJson(productsJson);

    PCJson_Free(productsJson);
}

Product_LoadFromJson(const JSON:productsJson) {
    if (json_is_object(productsJson)) {
        new Trie:p = PCSingle_Read(productsJson, EXSHOP_PRODUCT_OBJECT_PARAM_TYPE);
        if (p == Invalid_Trie || !TrieKeyExists(p, PCSINGLE_PARAM_KEY)) {
            PCJson_LogForFile(productsJson, "WARNING", "Error while reading product object.");
        }

        TrieDestroy(p);
    } else if (json_is_array(productsJson)) {
        for (new i = 0, ii = json_array_get_count(productsJson); i < ii; ++i) {
            new JSON:productJson = json_array_get_value(productsJson, i);
            Product_LoadFromJson(productJson);
            json_free(productJson);
        }
    } else {
        PCJson_LogForFile(productsJson, "WARNING", "Products file must contain an object or array of objects.");
    }
}

E_ExShop_BuyResult:Product_Buy(const playerIndex, const T_ExShop_Product:product) {
    new productObject[S_Product];
    Product_Get(product, productObject);
    
    if (!MWallet_Currency_IsEnough(productObject[Product_Currency], playerIndex, productObject[Product_Price])) {
        return ExShop_Buy_NotEnoughMoney;
    }
    
    if (!IC_Item_GiveArray(playerIndex, productObject[Product_Items])) {
        return ExShop_Buy_CantGiveItems;
    }
    
    MWallet_Currency_Credit(productObject[Product_Currency], playerIndex, productObject[Product_Price]);
    return ExShop_Buy_Success;
}

Product_iGetName(const T_ExShop_Product:product) {
    new productObject[S_Product];
    Product_Get(product, productObject);

    new productName[EXSHOP_PRODUCT_NAME_MAX_LEN];
    copy(productName, charsmax(productName), productObject[Product_Name]);
    
    return productName;
}

Product_Get(const T_ExShop_Product:product, productObject[S_Product]) {
    ArrayGetArray(Products, _:product, productObject);
}

T_ExShop_Product:Product_Find(const key[], const bool:orFail = true) {
    if (!TrieKeyExists(ProductsMap, key)) {
        if (orFail) {
            abort(AMX_ERR_PARAMS, "Product not found: %s", key);
        }

        return Invalid_ExShop_Product;
    }

    new T_ExShop_Product:product;
    TrieGetCell(ProductsMap, key, product);

    return product;
}

T_ExShop_Product:Product_Construct(productObject[S_Product]) {
    if (productObject[Product_Name][0] == EOS) {
        copy(productObject[Product_Name], charsmax(productObject[Product_Name]), productObject[Product_Key]);
    }

    new T_ExShop_Product:product = T_ExShop_Product:ArrayPushArray(Products, productObject);
    TrieSetCell(ProductsMap, productObject[Product_Key], product);

    return product;
}
