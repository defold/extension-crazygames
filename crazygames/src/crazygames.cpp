#define EXTENSION_NAME crazygames
#define LIB_NAME "CrazyGames"
#define MODULE_NAME "crazygames"

#include <dmsdk/sdk.h>
#include <stdlib.h>

#if defined(DM_PLATFORM_HTML5)

typedef void (*MidgameAdCallback)(int success);
typedef void (*RewardedAdCallback)(int success);
typedef void (*HasAdBlockCallback)(int success);
typedef void (*UserCallback)(char* user);
typedef void (*TokenCallback)(char* token);
typedef void (*JoinRoomCallback)(char* invite_params);
typedef void (*SettingsChangeCallback)(char* settings);

extern "C" {

    char* CrazyGamesJs_GetEnvironment();
    void  CrazyGamesJs_Finalize();

    // Game module
    void  CrazyGamesJs_GameplayStart();
    void  CrazyGamesJs_GameplayStop();
    void  CrazyGamesJs_LoadingStart();
    void  CrazyGamesJs_LoadingStop();
    void  CrazyGamesJs_HappyTime();
    void  CrazyGamesJs_ReportGameCompletedPercentage(double percentage);
    void  CrazyGamesJs_SetGameContext(const char* context);
    void  CrazyGamesJs_ClearGameContext();
    void  CrazyGamesJs_ClearInviteLinkParams();
    void  CrazyGamesJs_AddInviteLinkParamString(const char* key, const char* value);
    void  CrazyGamesJs_AddInviteLinkParamNumber(const char* key, int32_t value);
    void  CrazyGamesJs_AddInviteLinkParamBoolean(const char* key, bool value);
    char* CrazyGamesJs_ShowInviteButton();
    void  CrazyGamesJs_HideInviteButton();
    char* CrazyGamesJs_InviteLink();
    char* CrazyGamesJs_GetInviteParam(const char* key);
    bool  CrazyGamesJs_IsInstantMultiplayer();
    void  CrazyGamesJs_ClearRoomData();
    void  CrazyGamesJs_SetRoomId(const char* room_id);
    void  CrazyGamesJs_SetRoomIsJoinable(bool is_joinable);
    void  CrazyGamesJs_SetRoomInviteParams();
    void  CrazyGamesJs_UpdateRoom();
    void  CrazyGamesJs_LeftRoom();
    char* CrazyGamesJs_GetInviteParams();
    void  CrazyGamesJs_AddJoinRoomListener(JoinRoomCallback callback);
    void  CrazyGamesJs_RemoveJoinRoomListener();
    char* CrazyGamesJs_GetGameSettings();
    void  CrazyGamesJs_AddSettingsChangeListener(SettingsChangeCallback callback);
    void  CrazyGamesJs_RemoveSettingsChangeListener();

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
    char* CrazyGamesJs_GetSystemInfo();
    void  CrazyGamesJs_ListFriends(int32_t page, int32_t size, UserCallback callback);
    void  CrazyGamesJs_SubmitScore(const char* encrypted_score, double score);
    void  CrazyGamesJs_GetXsollaUserToken(TokenCallback callback);
    void  CrazyGamesJs_GetUserToken(TokenCallback callback);
    void  CrazyGamesJs_ShowAuthPrompt(UserCallback callback);
    void  CrazyGamesJs_GetUser(UserCallback callback);
    void  CrazyGamesJs_SetAuthListener(UserCallback callback);
    void  CrazyGamesJs_RemoveAuthListener();
    void  CrazyGamesJs_ShowAccountLinkPrompt(UserCallback callback);

    // Analytics module
    void  CrazyGamesJs_TrackOrder(const char* provider, const char* order);
}


static int CrazyGames_GetEnvironment(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    char* environment = CrazyGamesJs_GetEnvironment();
    if (environment)
    {
        lua_pushstring(L, environment);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}


static dmScript::LuaCallbackInfo* CrazyGames_CreateCallback(lua_State* L, int index, const char* funcname)
{
    if (!lua_isfunction(L, index))
    {
        luaL_error(L, "Expected argument %d when calling %s to be a callback function.", index, funcname);
        return 0;
    }
    return dmScript::CreateCallback(L, index);
}

static char* CrazyGames_LuaTableToJson(lua_State* L, int index, const char* table_name)
{
    luaL_checktype(L, index, LUA_TTABLE);

    // Preserve an empty Lua table as a JavaScript object rather than an array.
    lua_newtable(L);
    lua_pushboolean(L, 1);
    lua_setfield(L, -2, "encode_empty_table_as_object");

    char* json = 0x0;
    size_t json_length = 0;
    const int encoded = dmScript::LuaToJson(L, index, lua_gettop(L), &json, &json_length);
    lua_pop(L, 1);
    if (!encoded)
    {
        luaL_error(L, "Unable to encode %s as JSON.", table_name);
        return 0x0;
    }
    return json;
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

static void CrazyGames_InvokeUserTokenCallback(dmScript::LuaCallbackInfo* callback, const char* token)
{
    if (!dmScript::IsCallbackValid(callback))
    {
        dmLogError("CrazyGames user token callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);

    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(callback))
    {
        dmLogError("CrazyGames user token callback setup failed.");
        return;
    }

    if (token)
    {
        lua_pushstring(L, token);
    }
    else
    {
        lua_pushnil(L);
    }

    dmScript::PCall(L, 2, 0);

    dmScript::TeardownCallback(callback);
}

static dmScript::LuaCallbackInfo* crazyGames_GetUserTokenCallback = 0x0;
static void CrazyGames_GetUserTokenCallback(char* token)
{
    CrazyGames_InvokeUserTokenCallback(crazyGames_GetUserTokenCallback, token);
    dmScript::DestroyCallback(crazyGames_GetUserTokenCallback);
    crazyGames_GetUserTokenCallback = 0x0;
}
static int CrazyGames_GetUserToken(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_GetUserTokenCallback = CrazyGames_CreateCallback(L, 1, "get_user_token"))
    {
        CrazyGamesJs_GetUserToken((TokenCallback)CrazyGames_GetUserTokenCallback);
    }
    return 0;
}


static dmScript::LuaCallbackInfo* crazyGames_GetXsollaUserTokenCallback = 0x0;
static void CrazyGames_GetXsollaUserTokenCallback(char* token)
{
    CrazyGames_InvokeUserTokenCallback(crazyGames_GetXsollaUserTokenCallback, token);
    dmScript::DestroyCallback(crazyGames_GetXsollaUserTokenCallback);
    crazyGames_GetXsollaUserTokenCallback = 0x0;
}
static int CrazyGames_GetXsollaUserToken(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_GetXsollaUserTokenCallback = CrazyGames_CreateCallback(L, 1, "get_xsolla_user_token"))
    {
        CrazyGamesJs_GetXsollaUserToken((TokenCallback)CrazyGames_GetXsollaUserTokenCallback);
    }
    return 0;
}

static int CrazyGames_IsUserAccountAvailable(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool available = CrazyGamesJs_IsUserAccountAvailable();
    lua_pushboolean(L, available);
    return 1;
}

static int CrazyGames_GetSystemInfo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* system_info = CrazyGamesJs_GetSystemInfo();
    if (system_info)
    {
        dmScript::JsonToLua(L, system_info, strlen(system_info));
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static int CrazyGames_SubmitScore(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* encrypted_score = luaL_checkstring(L, 1);
    const lua_Number score = luaL_checknumber(L, 2);
    if (encrypted_score[0] == '\0')
    {
        return luaL_error(L, "Expected submit_score encrypted_score to be non-empty.");
    }
    CrazyGamesJs_SubmitScore(encrypted_score, score);
    return 0;
}


static void CrazyGames_InvokeJsonCallback(dmScript::LuaCallbackInfo* callback, const char* json)
{
    if (!dmScript::IsCallbackValid(callback))
    {
        dmLogError("CrazyGames JSON callback is invalid.");
        return;
    }

    lua_State* L = dmScript::GetCallbackLuaContext(callback);

    DM_LUA_STACK_CHECK(L, 0);

    if (!dmScript::SetupCallback(callback))
    {
        dmLogError("CrazyGames JSON callback setup failed.");
        return;
    }

    if (json)
    {
        const size_t json_length = strlen(json);
        dmScript::JsonToLua(L, json, json_length);
    }
    else {
        lua_pushnil(L);
    }

    dmScript::PCall(L, 2, 0);

    dmScript::TeardownCallback(callback);
}


static dmScript::LuaCallbackInfo* crazyGames_GetUserCallback = 0x0;
static void CrazyGames_GetUserCallback(char* user)
{
    CrazyGames_InvokeJsonCallback(crazyGames_GetUserCallback, user);
    dmScript::DestroyCallback(crazyGames_GetUserCallback);
    crazyGames_GetUserCallback = 0x0;
}
static int CrazyGames_GetUser(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_GetUserCallback = CrazyGames_CreateCallback(L, 1, "get_user"))
    {
        CrazyGamesJs_GetUser((UserCallback)CrazyGames_GetUserCallback);
    }
    return 0;
}

static dmScript::LuaCallbackInfo* crazyGames_ListFriendsCallback = 0x0;
static void CrazyGames_ListFriendsCallback(char* friends_page)
{
    CrazyGames_InvokeJsonCallback(crazyGames_ListFriendsCallback, friends_page);
    dmScript::DestroyCallback(crazyGames_ListFriendsCallback);
    crazyGames_ListFriendsCallback = 0x0;
}
static int CrazyGames_ListFriends(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const int32_t page = luaL_checkinteger(L, 1);
    const int32_t size = luaL_checkinteger(L, 2);
    if (page < 1)
    {
        return luaL_error(L, "Expected list_friends page to be at least 1.");
    }
    if (size < 1 || size > 50)
    {
        return luaL_error(L, "Expected list_friends size to be between 1 and 50.");
    }
    if (crazyGames_ListFriendsCallback)
    {
        return luaL_error(L, "A list_friends request is already in progress.");
    }

    crazyGames_ListFriendsCallback = CrazyGames_CreateCallback(L, 3, "list_friends");
    if (crazyGames_ListFriendsCallback)
    {
        CrazyGamesJs_ListFriends(page, size, (UserCallback)CrazyGames_ListFriendsCallback);
    }
    return 0;
}


static dmScript::LuaCallbackInfo* crazyGames_ShowAuthPromptCallback = 0x0;
static void CrazyGames_ShowAuthPromptCallback(char* user)
{
    CrazyGames_InvokeJsonCallback(crazyGames_ShowAuthPromptCallback, user);
    dmScript::DestroyCallback(crazyGames_ShowAuthPromptCallback);
    crazyGames_ShowAuthPromptCallback = 0x0;
}
static int CrazyGames_ShowAuthPrompt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_ShowAuthPromptCallback = CrazyGames_CreateCallback(L, 1, "show_auth_prompt"))
    {
        CrazyGamesJs_ShowAuthPrompt((UserCallback)CrazyGames_ShowAuthPromptCallback);
    }
    return 0;
}

static dmScript::LuaCallbackInfo* crazyGames_AuthListenerCallback = 0x0;
static void CrazyGames_AuthListenerCallback(char* user)
{
    CrazyGames_InvokeJsonCallback(crazyGames_AuthListenerCallback, user);
}
static int CrazyGames_SetAuthListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_AuthListenerCallback = CrazyGames_CreateCallback(L, 1, "set_auth_listener"))
    {
        CrazyGamesJs_SetAuthListener((UserCallback)CrazyGames_AuthListenerCallback);
    }
    return 0;
}


static int CrazyGames_RemoveAuthListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_RemoveAuthListener();
    if (crazyGames_AuthListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_AuthListenerCallback);
        crazyGames_AuthListenerCallback = 0x0;
    }
    return 0;
}

static dmScript::LuaCallbackInfo* crazyGames_ShowAccountLinkPromptCallback = 0x0;
static void CrazyGames_ShowAccountLinkPromptCallback(char* response)
{
    CrazyGames_InvokeJsonCallback(crazyGames_ShowAccountLinkPromptCallback, response);
    dmScript::DestroyCallback(crazyGames_ShowAccountLinkPromptCallback);
    crazyGames_ShowAccountLinkPromptCallback = 0x0;
}
static int CrazyGames_ShowAccountLinkPrompt(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    if (crazyGames_ShowAccountLinkPromptCallback = CrazyGames_CreateCallback(L, 1, "show_account_link_prompt"))
    {
        CrazyGamesJs_ShowAccountLinkPrompt((UserCallback)CrazyGames_ShowAccountLinkPromptCallback);
    }
    return 0;
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

static int CrazyGames_ReportGameCompletedPercentage(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const lua_Number percentage = luaL_checknumber(L, 1);
    if (percentage != percentage || percentage < 0 || percentage > 100)
    {
        return luaL_error(L, "Expected report_game_completed_percentage to be between 0 and 100.");
    }
    CrazyGamesJs_ReportGameCompletedPercentage(percentage);
    return 0;
}

static int CrazyGames_SetGameContext(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    char* context = CrazyGames_LuaTableToJson(L, 1, "game context");
    CrazyGamesJs_SetGameContext(context);
    free(context);
    return 0;
}

static int CrazyGames_ClearGameContext(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_ClearGameContext();
    return 0;
}

static int CrazyGames_TrackOrder(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    const char* provider = luaL_checkstring(L, 1);
    if (provider[0] == '\0')
    {
        return luaL_error(L, "Expected track_order provider to be non-empty.");
    }
    char* order = CrazyGames_LuaTableToJson(L, 2, "order");
    CrazyGamesJs_TrackOrder(provider, order);
    free(order);
    return 0;
}

static int CrazyGames_GetGameSettings(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* settings = CrazyGamesJs_GetGameSettings();
    if (settings)
    {
        dmScript::JsonToLua(L, settings, strlen(settings));
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static dmScript::LuaCallbackInfo* crazyGames_SettingsChangeListenerCallback = 0x0;
static void CrazyGames_SettingsChangeListenerCallback(char* settings)
{
    CrazyGames_InvokeJsonCallback(crazyGames_SettingsChangeListenerCallback, settings);
}

static int CrazyGames_AddSettingsChangeListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    // The Defold API exposes one settings listener. Re-registering replaces it.
    if (crazyGames_SettingsChangeListenerCallback)
    {
        CrazyGamesJs_RemoveSettingsChangeListener();
        dmScript::DestroyCallback(crazyGames_SettingsChangeListenerCallback);
        crazyGames_SettingsChangeListenerCallback = 0x0;
    }

    crazyGames_SettingsChangeListenerCallback = CrazyGames_CreateCallback(L, 1, "add_settings_change_listener");
    if (crazyGames_SettingsChangeListenerCallback)
    {
        CrazyGamesJs_AddSettingsChangeListener((SettingsChangeCallback)CrazyGames_SettingsChangeListenerCallback);
    }
    return 0;
}

static int CrazyGames_RemoveSettingsChangeListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_RemoveSettingsChangeListener();
    if (crazyGames_SettingsChangeListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_SettingsChangeListenerCallback);
        crazyGames_SettingsChangeListenerCallback = 0x0;
    }
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
        if (lua_type(L, -2) != LUA_TSTRING)
        {
            luaL_error(L, "Expected invite parameter keys to be strings, got '%s'", luaL_typename(L, -2));
            return;
        }
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
                luaL_error(L, "Wrong type for invite parameter '%s', type: '%s'", param_name, luaL_typename(L, -1));
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

static int CrazyGames_IsInstantMultiplayer(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    bool instant = CrazyGamesJs_IsInstantMultiplayer();
    lua_pushboolean(L, instant);
    return 1;
}

static int CrazyGames_UpdateRoom(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    luaL_checktype(L, 1, LUA_TTABLE);
    CrazyGamesJs_ClearRoomData();

    lua_getfield(L, 1, "roomId");
    if (!lua_isnil(L, -1))
    {
        if (lua_type(L, -1) != LUA_TSTRING)
        {
            return luaL_error(L, "Expected update_room.roomId to be a string, got '%s'", luaL_typename(L, -1));
        }
        CrazyGamesJs_SetRoomId(lua_tostring(L, -1));
    }
    lua_pop(L, 1);

    lua_getfield(L, 1, "isJoinable");
    if (!lua_isnil(L, -1))
    {
        if (lua_type(L, -1) != LUA_TBOOLEAN)
        {
            return luaL_error(L, "Expected update_room.isJoinable to be a boolean, got '%s'", luaL_typename(L, -1));
        }
        CrazyGamesJs_SetRoomIsJoinable(lua_toboolean(L, -1));
    }
    lua_pop(L, 1);

    lua_getfield(L, 1, "inviteParams");
    if (!lua_isnil(L, -1))
    {
        if (lua_type(L, -1) != LUA_TTABLE)
        {
            return luaL_error(L, "Expected update_room.inviteParams to be a table, got '%s'", luaL_typename(L, -1));
        }
        CrazyGames_SetInviteLinkParams(L, -1);
        CrazyGamesJs_SetRoomInviteParams();
    }
    lua_pop(L, 1);

    CrazyGamesJs_UpdateRoom();
    return 0;
}

static int CrazyGames_LeftRoom(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_LeftRoom();
    return 0;
}

static int CrazyGames_GetInviteParams(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* invite_params = CrazyGamesJs_GetInviteParams();
    if (invite_params)
    {
        dmScript::JsonToLua(L, invite_params, strlen(invite_params));
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

static dmScript::LuaCallbackInfo* crazyGames_JoinRoomListenerCallback = 0x0;
static void CrazyGames_JoinRoomListenerCallback(char* invite_params)
{
    CrazyGames_InvokeJsonCallback(crazyGames_JoinRoomListenerCallback, invite_params);
}

static int CrazyGames_AddJoinRoomListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);

    // The Defold API exposes one join-room listener. Re-registering replaces it.
    if (crazyGames_JoinRoomListenerCallback)
    {
        CrazyGamesJs_RemoveJoinRoomListener();
        dmScript::DestroyCallback(crazyGames_JoinRoomListenerCallback);
        crazyGames_JoinRoomListenerCallback = 0x0;
    }

    crazyGames_JoinRoomListenerCallback = CrazyGames_CreateCallback(L, 1, "add_join_room_listener");
    if (crazyGames_JoinRoomListenerCallback)
    {
        CrazyGamesJs_AddJoinRoomListener((JoinRoomCallback)CrazyGames_JoinRoomListenerCallback);
    }
    return 0;
}

static int CrazyGames_RemoveJoinRoomListener(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    CrazyGamesJs_RemoveJoinRoomListener();
    if (crazyGames_JoinRoomListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_JoinRoomListenerCallback);
        crazyGames_JoinRoomListenerCallback = 0x0;
    }
    return 0;
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
    {"get_environment",            CrazyGames_GetEnvironment},
    // game
    {"gameplay_start",             CrazyGames_GameplayStart},
    {"gameplay_stop",              CrazyGames_GameplayStop},
    {"loading_start",              CrazyGames_LoadingStart},
    {"loading_stop",               CrazyGames_LoadingStop},
    {"happytime",                  CrazyGames_HappyTime},
    {"report_game_completed_percentage", CrazyGames_ReportGameCompletedPercentage},
    {"set_game_context",           CrazyGames_SetGameContext},
    {"clear_game_context",         CrazyGames_ClearGameContext},
    {"get_game_settings",          CrazyGames_GetGameSettings},
    {"add_settings_change_listener", CrazyGames_AddSettingsChangeListener},
    {"remove_settings_change_listener", CrazyGames_RemoveSettingsChangeListener},
    {"show_invite_button",         CrazyGames_ShowInviteButton},
    {"hide_invite_button",         CrazyGames_HideInviteButton},
    {"get_invite_param",           CrazyGames_GetInviteParam},
    {"get_invite_params",          CrazyGames_GetInviteParams},
    {"invite_link",                CrazyGames_InviteLink},
    {"is_instant_multiplayer",     CrazyGames_IsInstantMultiplayer},
    {"update_room",                CrazyGames_UpdateRoom},
    {"left_room",                  CrazyGames_LeftRoom},
    {"add_join_room_listener",     CrazyGames_AddJoinRoomListener},
    {"remove_join_room_listener",  CrazyGames_RemoveJoinRoomListener},
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
    {"get_system_info",            CrazyGames_GetSystemInfo},
    {"list_friends",               CrazyGames_ListFriends},
    {"submit_score",               CrazyGames_SubmitScore},
    {"get_user",                   CrazyGames_GetUser},
    {"get_user_token",             CrazyGames_GetUserToken},
    {"get_xsolla_user_token",      CrazyGames_GetXsollaUserToken},
    {"show_auth_prompt",           CrazyGames_ShowAuthPrompt},
    {"set_auth_listener",          CrazyGames_SetAuthListener},
    {"remove_auth_listener",       CrazyGames_RemoveAuthListener},
    {"show_account_link_prompt",   CrazyGames_ShowAccountLinkPrompt},
    // analytics
    {"track_order",                CrazyGames_TrackOrder},
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
    // Stop JavaScript from dispatching any pending asynchronous results before
    // releasing the Lua callbacks they reference.
    CrazyGamesJs_Finalize();

    if (crazyGames_AdCallback)
    {
        dmScript::DestroyCallback(crazyGames_AdCallback);
        crazyGames_AdCallback = 0x0;
    }
    if (crazyGames_GetUserTokenCallback)
    {
        dmScript::DestroyCallback(crazyGames_GetUserTokenCallback);
        crazyGames_GetUserTokenCallback = 0x0;
    }
    if (crazyGames_GetXsollaUserTokenCallback)
    {
        dmScript::DestroyCallback(crazyGames_GetXsollaUserTokenCallback);
        crazyGames_GetXsollaUserTokenCallback = 0x0;
    }
    if (crazyGames_GetUserCallback)
    {
        dmScript::DestroyCallback(crazyGames_GetUserCallback);
        crazyGames_GetUserCallback = 0x0;
    }
    if (crazyGames_ListFriendsCallback)
    {
        dmScript::DestroyCallback(crazyGames_ListFriendsCallback);
        crazyGames_ListFriendsCallback = 0x0;
    }
    if (crazyGames_ShowAuthPromptCallback)
    {
        dmScript::DestroyCallback(crazyGames_ShowAuthPromptCallback);
        crazyGames_ShowAuthPromptCallback = 0x0;
    }
    if (crazyGames_AuthListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_AuthListenerCallback);
        crazyGames_AuthListenerCallback = 0x0;
    }
    if (crazyGames_ShowAccountLinkPromptCallback)
    {
        dmScript::DestroyCallback(crazyGames_ShowAccountLinkPromptCallback);
        crazyGames_ShowAccountLinkPromptCallback = 0x0;
    }
    if (crazyGames_JoinRoomListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_JoinRoomListenerCallback);
        crazyGames_JoinRoomListenerCallback = 0x0;
    }
    if (crazyGames_SettingsChangeListenerCallback)
    {
        dmScript::DestroyCallback(crazyGames_SettingsChangeListenerCallback);
        crazyGames_SettingsChangeListenerCallback = 0x0;
    }

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
