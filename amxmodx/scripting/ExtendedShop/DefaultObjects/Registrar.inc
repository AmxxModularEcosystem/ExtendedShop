#include <ParamsController>

// #include "ExtendedShop/DefaultObjects/ParamType/"
#include "ExtendedShop/DefaultObjects/ParamType/MWallet-Currency"
#include "ExtendedShop/DefaultObjects/ParamType/ProductObject"
#include "ExtendedShop/DefaultObjects/ParamType/Product"

public ParamsController_OnRegisterTypes() {
    DefaultObjects_ParamType_MWallet_Currency_Regsiter();
    DefaultObjects_ParamType_ProductObject_Regsiter();
    DefaultObjects_ParamType_Product_Regsiter();
}


#include <ItemsController>

// #include "ExtendedShop/DefaultObjects/ItemType/"

public IC_ItemType_OnInited() {

}
