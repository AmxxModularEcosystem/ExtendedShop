#include <ParamsController>

#include "ExtendedShop/DefaultObjects/ParamType/MWallet-Currency"
#include "ExtendedShop/DefaultObjects/ParamType/ProductObject"
#include "ExtendedShop/DefaultObjects/ParamType/Product"
#include "ExtendedShop/DefaultObjects/ParamType/MenuObject"

public ParamsController_OnRegisterTypes() {
    log_amx("[INFO] Registering ExShop's param types.");

    DefaultObjects_ParamType_MWallet_Currency_Regsiter();
    DefaultObjects_ParamType_Product_Regsiter();
    DefaultObjects_ParamType_ProductObject_Regsiter();
    DefaultObjects_ParamType_MenuObject_Regsiter();
    
    log_amx("[INFO] ExShop's param types registered.");
}


#include <ItemsController>

#include "ExtendedShop/DefaultObjects/ItemType/Buy"

public IC_ItemType_OnInited() {
    log_amx("[INFO] Registering ExShop's item types.");
    
    DefaultObjects_ItemType_Buy_Regsiter();

    log_amx("[INFO] ExShop's item types registered.");
}
