#include <json>
#include <ParamsController>
#include <ModularWallet>
#include <ExtendedShop>

// TODO: Надо бы в MWallet такое регать.

DefaultObjects_ParamType_MWallet_Currency_Regsiter() {
    ParamsController_RegSimpleType(EXSHOP_MWALLET_CURRENCY_PARAM_TYPE, "@DefaultObjects_ParamType_MWallet_Currency_OnRead");
}

@DefaultObjects_ParamType_MWallet_Currency_OnRead(const JSON:valueJson) {
    new currencyName[MWALLET_CURRENCY_MAX_NAME_LEN];
    json_get_string(valueJson, currencyName, charsmax(currencyName));
    
    new T_Currency:currency = MWallet_Currency_Find(currencyName);
    if (currency == Invalid_Currency) {
        PCJson_LogForFile(valueJson, "WARNING", "Currency not found: %s", currencyName);
        return false;
    }

    return ParamsController_SetCell(currency);
}
