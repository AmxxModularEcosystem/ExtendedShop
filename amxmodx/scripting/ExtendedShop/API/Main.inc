#include <amxmodx>

API_Main_RegisterNatives() {
    register_native("ExShop_Init", "@ExShop_Init");
}

@ExShop_Init() {
    PluginInit();
}
