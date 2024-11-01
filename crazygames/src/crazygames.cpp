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
    void CrazyGamesJs_GameplayStart();
    void CrazyGamesJs_GameplayStop();
    void CrazyGamesJs_ShowMidgameAd(MidgameAdCallback callback);
    void CrazyGamesJs_ShowRewardedAd(RewardedAdCallback callback);
     int CrazyGamesJs_IsAdBlocked(IsAdBlockedCallback callback);
    void CrazyGamesJs_HappyTime();
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

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"gameplay_start",   CrazyGames_GameplayStart},
    {"gameplay_stop",    CrazyGames_GameplayStop},
    {"show_midgame_ad",  CrazyGames_ShowMidgameAd},
    {"show_rewarded_ad", CrazyGames_ShowRewardedAd},
    {"happytime",        CrazyGames_HappyTime},
    {"is_ad_blocked",    CrazyGames_IsAdBlocked},
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
