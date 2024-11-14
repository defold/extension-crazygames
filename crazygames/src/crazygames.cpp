#define EXTENSION_NAME crazygames
#define LIB_NAME "CrazyGames"
#define MODULE_NAME "crazygames"

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

enum CrazyGamesCallbackType
{
    TYPE_MIDGAME,
    TYPE_REWARDED,
    TYPE_ISADBLOCKED
};

typedef void (*MidgameAdCallback)(int success);
typedef void (*RewardedAdCallback)(int success);
typedef void (*IsAdBlockedCallback)(int success);

extern "C" {

    // Game module
    void  CrazyGamesJs_GameplayStart();
    void  CrazyGamesJs_GameplayStop();
    void  CrazyGamesJs_LoadingStart();
    void  CrazyGamesJs_LoadingStop();
    void  CrazyGamesJs_HappyTime();
    void  CrazyGamesJs_ClearInviteLinkParams();
    void  CrazyGamesJs_AddInviteLinkParamString(const char* key, const char* value);
    void  CrazyGamesJs_AddInviteLinkParamNumber(const char* key, int32_t value);
    void  CrazyGamesJs_AddInviteLinkParamBoolean(const char* key, bool value);
    char* CrazyGamesJs_ShowInviteButton();
    void  CrazyGamesJs_HideInviteButton();
    char* CrazyGamesJs_InviteLink();
    char* CrazyGamesJs_GetInviteParam(const char* key);

    // Ads module
    void  CrazyGamesJs_ShowMidgameAd(MidgameAdCallback callback);
    void  CrazyGamesJs_ShowRewardedAd(RewardedAdCallback callback);
    void  CrazyGamesJs_IsAdBlocked(IsAdBlockedCallback callback);

    // Data module
    void  CrazyGamesJs_ClearData();
    char* CrazyGamesJs_GetItem(const char* key);
    void  CrazyGamesJs_RemoveItem(const char* key);
    void  CrazyGamesJs_SetItem(const char* key, const char* value);
}

static dmScript::LuaCallbackInfo* crazyGames_Callback = 0x0;

static void CrazyGames_InvokeCallback(CrazyGamesCallbackType callbackType, int intArg, const char* charArg)
{
    if (!dmScript::IsCallbackValid(crazyGames_Callback))
    {
        dmLogError("CrazyGames callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(crazyGames_Callback);

    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(crazyGames_Callback))
    {
        dmLogError("CrazyGames callback setup failed.");
        return;
    }

    int numOfArgs = 0;

    if ((callbackType == TYPE_MIDGAME)
        || (callbackType == TYPE_REWARDED)
        || (callbackType == TYPE_ISADBLOCKED))
    {
        lua_pushboolean(L, intArg);
        numOfArgs += 1;
    }

    numOfArgs += 1;
    int ret = dmScript::PCall(L, numOfArgs, 0);
    (void)ret;

    dmScript::TeardownCallback(crazyGames_Callback);

    if (crazyGames_Callback != 0x0)
    {
        dmScript::DestroyCallback(crazyGames_Callback);
        crazyGames_Callback = 0x0;
    }
}



static int check_callback(lua_State* L, int index, char* funcname)
{
    if (!lua_isfunction(L, index))
    {
        luaL_error(L, "Expected argument %d when calling %s to be a callback function.", index, funcname);
        return 0;
    }
    if (crazyGames_Callback != 0x0) {
        dmLogWarning("CrazyGames callback already set when calling %s. Overwriting existing callback.", funcname);
    }
    crazyGames_Callback = dmScript::CreateCallback(L, index);
    return 1;
}


static void CrazyGames_MidgameAdCallback(int success)
{
    CrazyGames_InvokeCallback(TYPE_MIDGAME, success, 0);
}

static void CrazyGames_RewardedAdCallback(int success)
{
    CrazyGames_InvokeCallback(TYPE_REWARDED, success, 0);
}

static void CrazyGames_IsAdBlockedCallback(int success)
{
    CrazyGames_InvokeCallback(TYPE_ISADBLOCKED, success, 0);
}

static int CrazyGames_LoadingStart(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_LoadingStart();
    return 0;
}

static int CrazyGames_LoadingStop(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_LoadingStop();
    return 0;
}

static int CrazyGames_GameplayStart(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_GameplayStart();
    return 0;
}

static int CrazyGames_GameplayStop(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_GameplayStop();
    return 0;
}

static int CrazyGames_HappyTime(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_HappyTime();
    return 0;
}

static int CrazyGames_ShowMidgameAd(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (check_callback(L, 1, "show_midgame_ad"))
    {
        CrazyGamesJs_ShowMidgameAd((MidgameAdCallback)CrazyGames_MidgameAdCallback);
    }
    return 0;
}

static int CrazyGames_ShowRewardedAd(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (check_callback(L, 1, "show_rewarded_ad"))
    {
        CrazyGamesJs_ShowRewardedAd((RewardedAdCallback)CrazyGames_RewardedAdCallback);
    }
    return 0;
}

static int CrazyGames_IsAdBlocked(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (check_callback(L, 1, "is_ad_blocked"))
    {
        CrazyGamesJs_IsAdBlocked((IsAdBlockedCallback)CrazyGames_IsAdBlockedCallback);
    }
    return 0;
}

static void CrazyGames_SetInviteLinkParams(lua_State* L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);
    CrazyGamesJs_ClearInviteLinkParams();
    lua_pushvalue(L, index);
    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        const char* param_name = lua_tostring(L, -2);
        int t = lua_type(L, -1);
        switch (t) {
            case LUA_TSTRING:
                CrazyGamesJs_AddInviteLinkParamString(param_name, lua_tostring(L, -1));
            break;
            case LUA_TBOOLEAN:
                CrazyGamesJs_AddInviteLinkParamBoolean(param_name, lua_toboolean(L, -1));
            break;
            case LUA_TNUMBER:
                CrazyGamesJs_AddInviteLinkParamNumber(param_name, lua_tonumber(L, -1));
            break;
            default:  /* other values */
                lua_pop(L, 3);
                luaL_error(L, "Wrong type for table attribute '%s', type: '%s'", param_name, luaL_typename(L, -1));
                return;
            break;
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

static int CrazyGames_InviteLink(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    CrazyGames_SetInviteLinkParams(L, 1);
    const char* link = CrazyGamesJs_InviteLink();
    lua_pushstring(L, link);
    return 1;
}
static int CrazyGames_ShowInviteButton(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    CrazyGames_SetInviteLinkParams(L, 1);
    const char* link = CrazyGamesJs_ShowInviteButton();
    lua_pushstring(L, link);
    return 1;
}

static int CrazyGames_HideInviteButton(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_HideInviteButton();
    return 0;
}


static int CrazyGames_GetInviteParam(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* key = luaL_checkstring(L, 1);
    char* value = CrazyGamesJs_GetInviteParam(key);
    lua_pushstring(L, value);
    return 1;
}

static int CrazyGames_Clear(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_ClearData();
    return 0;
}

static int CrazyGames_GetItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* key = luaL_checkstring(L, 1);
    char* value = CrazyGamesJs_GetItem(key);
    if (value)
    {
        lua_pushstring(L, value);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int CrazyGames_RemoveItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* key = luaL_checkstring(L, 1);
    CrazyGamesJs_RemoveItem(key);
    return 0;
}

static int CrazyGames_SetItem(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* key = luaL_checkstring(L, 1);
    const char* value = luaL_checkstring(L, 2);
    CrazyGamesJs_SetItem(key, value);
    return 0;
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"gameplay_start",     CrazyGames_GameplayStart},
    {"gameplay_stop",      CrazyGames_GameplayStop},
    {"loading_start",      CrazyGames_LoadingStart},
    {"loading_stop",       CrazyGames_LoadingStop},
    {"show_midgame_ad",    CrazyGames_ShowMidgameAd},
    {"show_rewarded_ad",   CrazyGames_ShowRewardedAd},
    {"happytime",          CrazyGames_HappyTime},
    {"is_ad_blocked",      CrazyGames_IsAdBlocked},
    {"show_invite_button", CrazyGames_ShowInviteButton},
    {"hide_invite_button", CrazyGames_HideInviteButton},
    {"get_invite_param",   CrazyGames_GetInviteParam},
    {"invite_link",        CrazyGames_InviteLink},
    {"clear_data",         CrazyGames_Clear},
    {"get_item",           CrazyGames_GetItem},
    {"remove_item",        CrazyGames_RemoveItem},
    {"set_item",           CrazyGames_SetItem},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result InitializeCrazyGames(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeCrazyGames(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static void LogListener(LogSeverity severity, const char* domain, const char* formatted_string)
{
    switch (severity)
    {
        case LOG_SEVERITY_ERROR:
        case LOG_SEVERITY_FATAL:
        break;
        case LOG_SEVERITY_WARNING:
        case LOG_SEVERITY_DEBUG:
        case LOG_SEVERITY_USER_DEBUG:
        case LOG_SEVERITY_INFO:
        // do nothing
        break;
        default:
        // do nothing
        break;
    }
}

static dmExtension::Result AppInitializeCrazyGames(dmExtension::AppParams *params)
{
    dmLogRegisterListener(&LogListener);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeCrazyGames(dmExtension::AppParams *params)
{
    dmLogUnregisterListener(&LogListener);
    return dmExtension::RESULT_OK;
}

#else // unsupported platforms

static dmExtension::Result InitializeCrazyGames(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeCrazyGames(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppInitializeCrazyGames(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeCrazyGames(dmExtension::AppParams *params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeCrazyGames, AppFinalizeCrazyGames, InitializeCrazyGames, 0, 0, FinalizeCrazyGames)
