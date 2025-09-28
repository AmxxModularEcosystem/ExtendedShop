#include <ParamsController>

#include "ExtendedShop/DefaultObjects/ParamType/MWallet-Currency"
#include "ExtendedShop/DefaultObjects/ParamType/ProductObject"
#include "ExtendedShop/DefaultObjects/ParamType/Product"

public ParamsController_OnRegisterTypes() {
    ExShop_Init();

    DefaultObjects_ParamType_MWallet_Currency_Regsiter();
    DefaultObjects_ParamType_ProductObject_Regsiter();
    DefaultObjects_ParamType_Product_Regsiter();
}


#include <ItemsController>

#include "ExtendedShop/DefaultObjects/ItemType/Buy"

public IC_ItemType_OnInited() {
    ExShop_Init();
    
    DefaultObjects_ItemType_Buy_Regsiter();
}
