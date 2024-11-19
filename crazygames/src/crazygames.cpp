#define EXTENSION_NAME crazygames
#define LIB_NAME "CrazyGames"
#define MODULE_NAME "crazygames"

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_HTML5)

typedef void (*MidgameAdCallback)(int success);
typedef void (*RewardedAdCallback)(int success);
typedef void (*HasAdBlockCallback)(int success);
typedef void (*UserAuthCallback)(char* user, char* token);

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
    void  CrazyGamesJs_HasAdBlock(HasAdBlockCallback callback);
    void  CrazyGamesJs_RequestBanner(const char* div, int32_t width, int32_t height);
    void  CrazyGamesJs_RequestResponsiveBanner(const char* div);
    void  CrazyGamesJs_ClearBanner(const char* div);
    void  CrazyGamesJs_ClearAllBanners();

    // Data module
    void  CrazyGamesJs_ClearData();
    char* CrazyGamesJs_GetItem(const char* key);
    void  CrazyGamesJs_RemoveItem(const char* key);
    void  CrazyGamesJs_SetItem(const char* key, const char* value);

    // User module
    bool  CrazyGamesJs_IsUserAccountAvailable();
    void  CrazyGamesJs_ShowAuthPrompt();
    void  CrazyGamesJs_GetUser();
    void  CrazyGamesJs_GetUserToken();
    void  CrazyGamesJs_SetAuthListener(UserAuthCallback callback);
    void  CrazyGamesJs_RemoveAuthListener();
    bool  CrazyGamesJs_ShowAccountLinkPrompt();
}



static dmScript::LuaCallbackInfo* CrazyGames_CreateCallback(lua_State* L, int index, char* funcname)
{
    if (!lua_isfunction(L, index))
    {
        luaL_error(L, "Expected argument %d when calling %s to be a callback function.", index, funcname);
        return 0;
    }
    return dmScript::CreateCallback(L, index);
}

/***********/
/*** Ads ***/
/***********/

static dmScript::LuaCallbackInfo* crazyGames_AdCallback = 0x0;
static void CrazyGames_InvokeAdCallback(bool success)
{
    if (!dmScript::IsCallbackValid(crazyGames_AdCallback))
    {
        dmLogError("CrazyGames ad callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(crazyGames_AdCallback);

    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(crazyGames_AdCallback))
    {
        dmLogError("CrazyGames ad callback setup failed.");
        return;
    }

    lua_pushboolean(L, success);
    int ret = dmScript::PCall(L, 2, 0);
    (void)ret;

    dmScript::TeardownCallback(crazyGames_AdCallback);
    dmScript::DestroyCallback(crazyGames_AdCallback);
    crazyGames_AdCallback = 0x0;
}

static void CrazyGames_MidgameAdCallback(int success)
{
    CrazyGames_InvokeAdCallback(success);
}

static void CrazyGames_RewardedAdCallback(int success)
{
    CrazyGames_InvokeAdCallback(success);
}

static void CrazyGames_HasAdBlockCallback(int success)
{
    CrazyGames_InvokeAdCallback(success);
}

static int CrazyGames_ShowMidgameAd(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_AdCallback = CrazyGames_CreateCallback(L, 1, "show_midgame_ad"))
    {
        CrazyGamesJs_ShowMidgameAd((MidgameAdCallback)CrazyGames_MidgameAdCallback);
    }
    return 0;
}

static int CrazyGames_ShowRewardedAd(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_AdCallback = CrazyGames_CreateCallback(L, 1, "show_rewarded_ad"))
    {
        CrazyGamesJs_ShowRewardedAd((RewardedAdCallback)CrazyGames_RewardedAdCallback);
    }
    return 0;
}

static int CrazyGames_HasAdBlock(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_AdCallback = CrazyGames_CreateCallback(L, 1, "has_ad_block"))
    {
        CrazyGamesJs_HasAdBlock((HasAdBlockCallback)CrazyGames_HasAdBlockCallback);
    }
    return 0;
}

/************/
/*** Auth ***/
/************/

static dmScript::LuaCallbackInfo* crazyGames_AuthCallback = 0x0;
static void CrazyGames_InvokeAuthCallback(const char* user, const char* token)
{
    if (!dmScript::IsCallbackValid(crazyGames_AuthCallback))
    {
        dmLogError("CrazyGames auth callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(crazyGames_AuthCallback);

    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(crazyGames_AuthCallback))
    {
        dmLogError("CrazyGames auth callback setup failed.");
        return;
    }

    if (user)
    {
        const size_t user_length = strlen(user);
        dmScript::JsonToLua(L, user, user_length);
    }
    else {
        lua_pushnil(L);
    }
    
    if (token)
    {
        lua_pushstring(L, token);
    }
    else
    {
        lua_pushnil(L);
    }

    int ret = dmScript::PCall(L, 3, 0);
    (void)ret;

    dmScript::TeardownCallback(crazyGames_AuthCallback);
}

static void CrazyGames_UserAuthCallback(char* user, char* token)
{
    CrazyGames_InvokeAuthCallback(user, token);
}

static int CrazyGames_IsUserAccountAvailable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool available = CrazyGamesJs_IsUserAccountAvailable();
    lua_pushboolean(L, available);
    return 1;
}

static int CrazyGames_GetUser(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_GetUser();
    return 0;
}

static int CrazyGames_ShowAuthPrompt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_ShowAuthPrompt();
    return 0;
}

static int CrazyGames_GetUserToken(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_GetUserToken();
    return 0;
}

static int CrazyGames_SetAuthListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_AuthCallback = CrazyGames_CreateCallback(L, 1, "set_auth_listener"))
    {
        CrazyGamesJs_SetAuthListener((UserAuthCallback)CrazyGames_UserAuthCallback);
    }
    return 0;
}

static int CrazyGames_RemoveAuthListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_RemoveAuthListener();
    return 0;
}

static int CrazyGames_ShowAccountLinkPrompt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool result = CrazyGamesJs_ShowAccountLinkPrompt();
    lua_pushboolean(L, result);
    return 1;
}

/**************/
/*** Events ***/
/**************/

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

/***************/
/*** Invites ***/
/***************/

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

/**************/
/*** Data ***/
/**************/

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

static int CrazyGames_RequestBanner(lua_State* L) {
    DM_LUA_STACK_CHECK(L, 0);
    const char* div = luaL_checkstring(L, 1);
    int width = luaL_checknumber(L, 2);
    int height = luaL_checknumber(L, 3);
    CrazyGamesJs_RequestBanner(div, width, height);
    return 0;
}

static int CrazyGames_RequestResponsiveBanner(lua_State* L) {
    DM_LUA_STACK_CHECK(L, 0);
    const char* div = luaL_checkstring(L, 1);
    CrazyGamesJs_RequestResponsiveBanner(div);
    return 0;
}

static int CrazyGames_ClearBanner(lua_State* L) {
    DM_LUA_STACK_CHECK(L, 0);
    const char* div = luaL_checkstring(L, 1);
    CrazyGamesJs_ClearBanner(div);
    return 0;
}

static int CrazyGames_ClearAllBanners(lua_State* L) {
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_ClearAllBanners();
    return 0;
}


// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    // game
    {"gameplay_start",             CrazyGames_GameplayStart},
    {"gameplay_stop",              CrazyGames_GameplayStop},
    {"loading_start",              CrazyGames_LoadingStart},
    {"loading_stop",               CrazyGames_LoadingStop},
    {"happytime",                  CrazyGames_HappyTime},
    {"show_invite_button",         CrazyGames_ShowInviteButton},
    {"hide_invite_button",         CrazyGames_HideInviteButton},
    {"get_invite_param",           CrazyGames_GetInviteParam},
    {"invite_link",                CrazyGames_InviteLink},
    // ads
    {"show_midgame_ad",            CrazyGames_ShowMidgameAd},
    {"show_rewarded_ad",           CrazyGames_ShowRewardedAd},
    {"has_ad_block",               CrazyGames_HasAdBlock},
    {"request_banner",             CrazyGames_RequestBanner},
    {"request_responsive_banner",  CrazyGames_RequestResponsiveBanner},
    {"clear_banner",               CrazyGames_ClearBanner},
    {"clear_all_banners",          CrazyGames_ClearAllBanners},
    // data
    {"clear_data",                 CrazyGames_Clear},
    {"get_item",                   CrazyGames_GetItem},
    {"remove_item",                CrazyGames_RemoveItem},
    {"set_item",                   CrazyGames_SetItem},
    // user
    {"is_user_account_available",  CrazyGames_IsUserAccountAvailable},
    {"get_user",                   CrazyGames_GetUser},
    {"get_user_token",             CrazyGames_GetUserToken},
    {"show_auth_prompt",           CrazyGames_ShowAuthPrompt},
    {"set_auth_listener",          CrazyGames_SetAuthListener},
    {"remove_auth_listener",       CrazyGames_RemoveAuthListener},
    {"show_account_link_prompt",   CrazyGames_ShowAccountLinkPrompt},
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
