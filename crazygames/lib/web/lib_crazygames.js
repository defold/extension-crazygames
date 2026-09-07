// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibCrazyGames = {

    $CrazyGamesJs: {
        _luaCallback: null,
        _luaShowAuthPromptCallback: null,
        _luaShowAccountLinkPromptCallback: null,
        _luaGetUserTokenCallback: null,
        _luaGetXsollaUserTokenCallback: null,
        _luaAuthCallback: null,
        _luaGetUserCallback: null,
        _luaJoinRoomCallback: null,
        _luaSettingsChangeCallback: null,
        _inviteLinkParams: null,
        _roomData: null,

        _successCallback: function(success) {
            console.log("_successCallback", success, CrazyGamesJs._luaCallback);
            if (CrazyGamesJs._luaCallback == null) return;
            var msg = success ? 1 : 0;
            {{{ makeDynCall("vi", "CrazyGamesJs._luaCallback")}}}(msg);
        },

        _errorCallback: function(error) {
            console.log("_errorCallback", error, CrazyGamesJs._luaCallback);
            if (CrazyGamesJs._luaCallback == null) return;
            {{{ makeDynCall("vi", "CrazyGamesJs._luaCallback")}}}(0);
        },

        _callAuthCallback: function(user, token) {
            console.log("_callAuthCallback", user, token);
            if (CrazyGamesJs._luaAuthCallback == null) return;
            if (user != null) {
                const userJson = JSON.stringify(user);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaAuthCallback")}}}(stringToUTF8OnStack(userJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaAuthCallback")}}}(0);
            }
        },

        _callShowAuthPromptCallback: function(user) {
            console.log("_callShowAuthPromptCallback", user);
            if (CrazyGamesJs._luaShowAuthPromptCallback == null) return;
            if (user != null) {
                const userJson = JSON.stringify(user);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaShowAuthPromptCallback")}}}(stringToUTF8OnStack(userJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaShowAuthPromptCallback")}}}(0);
            }
        },

        _callShowAccountLinkPromptCallback: function(response) {
            if (CrazyGamesJs._luaShowAccountLinkPromptCallback == null) return;
            if (response != null) {
                const responseJson = JSON.stringify(response);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaShowAccountLinkPromptCallback")}}}(stringToUTF8OnStack(responseJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaShowAccountLinkPromptCallback")}}}(0);
            }
        },

        _callGetUserTokenCallback: function(token) {
            console.log("_callGetUserTokenCallback", token);
            if (CrazyGamesJs._luaGetUserTokenCallback == null) return;
            if (token != null) {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetUserTokenCallback")}}}(stringToUTF8OnStack(token));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetUserTokenCallback")}}}(0);
            }
        },

        _callGetXsollaUserTokenCallback: function(token) {
            console.log("_callGetXsollaUserTokenCallback", token);
            if (CrazyGamesJs._luaGetXsollaUserTokenCallback == null) return;
            if (token != null) {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetXsollaUserTokenCallback")}}}(stringToUTF8OnStack(token));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetXsollaUserTokenCallback")}}}(0);
            }
        },

        _callGetUserCallback: function(user) {
            console.log("_callGetUserCallback", user);
            if (CrazyGamesJs._luaGetUserCallback == null) return;
            if (user != null) {
                const userJson = JSON.stringify(user);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetUserCallback")}}}(stringToUTF8OnStack(userJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaGetUserCallback")}}}(0);
            }
        },

        _authCallback: function(user) {
            console.log("_authCallback", user);
            CrazyGamesJs._callAuthCallback(user);
        },

        _joinRoomCallback: function(inviteParams) {
            if (CrazyGamesJs._luaJoinRoomCallback == null) return;
            if (inviteParams != null) {
                const inviteParamsJson = JSON.stringify(inviteParams);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaJoinRoomCallback")}}}(stringToUTF8OnStack(inviteParamsJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaJoinRoomCallback")}}}(0);
            }
        },

        _settingsChangeCallback: function(settings) {
            if (CrazyGamesJs._luaSettingsChangeCallback == null) return;
            if (settings != null) {
                const settingsJson = JSON.stringify(settings);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaSettingsChangeCallback")}}}(stringToUTF8OnStack(settingsJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaSettingsChangeCallback")}}}(0);
            }
        }
    },

    CrazyGamesJs_GetEnvironment: function() {
        const environment = window.CrazyGames.SDK.environment;
        return environment != null ? stringToUTF8OnStack(environment) : null;
    },

    CrazyGamesJs_Finalize: function() {
        const hasAuthListener = CrazyGamesJs._luaAuthCallback != null;
        const hasJoinRoomListener = CrazyGamesJs._luaJoinRoomCallback != null;
        const hasSettingsChangeListener = CrazyGamesJs._luaSettingsChangeCallback != null;

        // Prevent pending promises and SDK listeners from dispatching into Lua
        // after the extension has released its callback references.
        CrazyGamesJs._luaCallback = null;
        CrazyGamesJs._luaShowAuthPromptCallback = null;
        CrazyGamesJs._luaShowAccountLinkPromptCallback = null;
        CrazyGamesJs._luaGetUserTokenCallback = null;
        CrazyGamesJs._luaGetXsollaUserTokenCallback = null;
        CrazyGamesJs._luaAuthCallback = null;
        CrazyGamesJs._luaGetUserCallback = null;
        CrazyGamesJs._luaJoinRoomCallback = null;
        CrazyGamesJs._luaSettingsChangeCallback = null;
        CrazyGamesJs._inviteLinkParams = null;
        CrazyGamesJs._roomData = null;

        try {
            if (hasAuthListener) {
                window.CrazyGames.SDK.user.removeAuthListener(CrazyGamesJs._authCallback);
            }
        } catch (e) {
            console.log("Failed to remove CrazyGames auth listener during shutdown", e);
        }
        try {
            if (hasJoinRoomListener) {
                window.CrazyGames.SDK.game.removeJoinRoomListener(CrazyGamesJs._joinRoomCallback);
            }
        } catch (e) {
            console.log("Failed to remove CrazyGames join-room listener during shutdown", e);
        }
        try {
            if (hasSettingsChangeListener) {
                window.CrazyGames.SDK.game.removeSettingsChangeListener(CrazyGamesJs._settingsChangeCallback);
            }
        } catch (e) {
            console.log("Failed to remove CrazyGames settings-change listener during shutdown", e);
        }
    },

    CrazyGamesJs_ShowMidgameAd: function(callback) {
        CrazyGamesJs._luaCallback = callback;
        const callbacks = {
            adFinished: () => {
                // console.log("CrazyGamesJs_ShowMidgameAd adFinished");
                CrazyGamesJs._successCallback(true);
            },
            adError: (error) => {
                // console.log("CrazyGamesJs_ShowMidgameAd adError", error);
                CrazyGamesJs._errorCallback(error);
            },
            adStarted: () => {
                // console.log("CrazyGamesJs_ShowMidgameAd adStarted");
            },
        };
        window.CrazyGames.SDK.ad.requestAd("midgame", callbacks);
    },

    CrazyGamesJs_ShowRewardedAd: function(callback) {
        CrazyGamesJs._luaCallback = callback;
        const callbacks = {
            adFinished: () => {
                // console.log("CrazyGamesJs_ShowRewardedAd adFinished");
                CrazyGamesJs._successCallback(true);
            },
            adError: (error) => {
                // console.log("CrazyGamesJs_ShowRewardedAd adError", error);
                CrazyGamesJs._errorCallback(error);
            },
            adStarted: () => {
                // console.log("CrazyGamesJs_ShowRewardedAd adStarted");
            },
        };
        window.CrazyGames.SDK.ad.requestAd("rewarded", callbacks);
    },

    CrazyGamesJs_HasAdBlock: function(callback) {
        CrazyGamesJs._luaCallback = callback;
        window.CrazyGames.SDK.ad.hasAdblock().then((result) => {
            CrazyGamesJs._successCallback(result);
        }).catch((error) => {
            CrazyGamesJs._errorCallback(error);
        });
    },

    CrazyGamesJs_ClearInviteLinkParams: function() {
        CrazyGamesJs._inviteLinkParams = {};
    },

    CrazyGamesJs_AddInviteLinkParamString: function(key, value) {
        CrazyGamesJs._inviteLinkParams[UTF8ToString(key)] = UTF8ToString(value);
    },

    CrazyGamesJs_AddInviteLinkParamNumber: function(key, value) {
        CrazyGamesJs._inviteLinkParams[UTF8ToString(key)] = value;
    },

    CrazyGamesJs_AddInviteLinkParamBoolean: function(key, value) {
        CrazyGamesJs._inviteLinkParams[UTF8ToString(key)] = value;
    },

    CrazyGamesJs_InviteLink: function() {
        const link = window.CrazyGames.SDK.game.inviteLink(CrazyGamesJs._inviteLinkParams);
        return stringToUTF8OnStack(link);
    },

    CrazyGamesJs_ShowInviteButton: function() {
        const link = window.CrazyGames.SDK.game.showInviteButton(CrazyGamesJs._inviteLinkParams);
        return stringToUTF8OnStack(link);
    },

    CrazyGamesJs_HideInviteButton: function() {
        window.CrazyGames.SDK.game.hideInviteButton();
    },

    CrazyGamesJs_GetInviteParam: function(key) {
        const value = window.CrazyGames.SDK.game.getInviteParam(UTF8ToString(key));
        return value != null ? stringToUTF8OnStack(value) : null;
    },

    CrazyGamesJs_IsInstantMultiplayer: function() {
        return window.CrazyGames.SDK.game.isInstantMultiplayer;
    },

    CrazyGamesJs_ClearRoomData: function() {
        CrazyGamesJs._roomData = {};
    },

    CrazyGamesJs_SetRoomId: function(roomId) {
        CrazyGamesJs._roomData.roomId = UTF8ToString(roomId);
    },

    CrazyGamesJs_SetRoomIsJoinable: function(isJoinable) {
        CrazyGamesJs._roomData.isJoinable = !!isJoinable;
    },

    CrazyGamesJs_SetRoomInviteParams: function() {
        CrazyGamesJs._roomData.inviteParams = CrazyGamesJs._inviteLinkParams;
    },

    CrazyGamesJs_UpdateRoom: function() {
        window.CrazyGames.SDK.game.updateRoom(CrazyGamesJs._roomData);
        CrazyGamesJs._roomData = null;
    },

    CrazyGamesJs_LeftRoom: function() {
        window.CrazyGames.SDK.game.leftRoom();
    },

    CrazyGamesJs_GetInviteParams: function() {
        const inviteParams = window.CrazyGames.SDK.game.inviteParams;
        return inviteParams != null ? stringToUTF8OnStack(JSON.stringify(inviteParams)) : null;
    },

    CrazyGamesJs_AddJoinRoomListener: function(callback) {
        CrazyGamesJs._luaJoinRoomCallback = callback;
        window.CrazyGames.SDK.game.addJoinRoomListener(CrazyGamesJs._joinRoomCallback);
    },

    CrazyGamesJs_RemoveJoinRoomListener: function() {
        CrazyGamesJs._luaJoinRoomCallback = null;
        window.CrazyGames.SDK.game.removeJoinRoomListener(CrazyGamesJs._joinRoomCallback);
    },

    CrazyGamesJs_HappyTime: function() {
        window.CrazyGames.SDK.game.happytime();
    },

    CrazyGamesJs_GetGameSettings: function() {
        const settings = window.CrazyGames.SDK.game.settings;
        return settings != null ? stringToUTF8OnStack(JSON.stringify(settings)) : null;
    },

    CrazyGamesJs_AddSettingsChangeListener: function(callback) {
        CrazyGamesJs._luaSettingsChangeCallback = callback;
        window.CrazyGames.SDK.game.addSettingsChangeListener(CrazyGamesJs._settingsChangeCallback);
    },

    CrazyGamesJs_RemoveSettingsChangeListener: function() {
        CrazyGamesJs._luaSettingsChangeCallback = null;
        window.CrazyGames.SDK.game.removeSettingsChangeListener(CrazyGamesJs._settingsChangeCallback);
    },

    CrazyGamesJs_GameplayStart: function() {
        window.CrazyGames.SDK.game.gameplayStart();
    },

    CrazyGamesJs_GameplayStop: function() {
        window.CrazyGames.SDK.game.gameplayStop();
    },

    CrazyGamesJs_LoadingStart: function() {
        window.CrazyGames.SDK.game.loadingStart();
    },

    CrazyGamesJs_LoadingStop: function() {
        window.CrazyGames.SDK.game.loadingStop();
    },

    CrazyGamesJs_ClearData: function() {
        window.CrazyGames.SDK.data.clear();
    },

    CrazyGamesJs_GetItem: function(key) {
        var value = window.CrazyGames.SDK.data.getItem(UTF8ToString(key));
        return value != null ? stringToUTF8OnStack(value) : null;
    },

    CrazyGamesJs_RemoveItem: function(key) {
        window.CrazyGames.SDK.data.removeItem(UTF8ToString(key));
    },

    CrazyGamesJs_SetItem: function(key, value) {
        window.CrazyGames.SDK.data.setItem(UTF8ToString(key), UTF8ToString(value));
    },

    CrazyGamesJs_RequestBanner: async function(div, width, height) {
        try {
            const name = UTF8ToString(div);
            const element = document.getElementById(name);
            element.style.width = width;
            element.style.height = height;
            element.style.display = "block";
            // await is not mandatory when requesting banners, but it will allow you to catch errors
            await window.CrazyGames.SDK.banner.requestBanner({
                id: name,
                width: width,
                height: height,
            });
        } catch (e) {
            console.log("Banner request error", e);
        }
    },

    CrazyGamesJs_RequestResponsiveBanner: async function(div) {
        try {
            const name = UTF8ToString(div);
            const element = document.getElementById(name);
            element.style.display = "block";
            // await is not mandatory when requesting banners, but it will allow you to catch errors
            await window.CrazyGames.SDK.banner.requestResponsiveBanner(name);
        } catch (e) {
            console.log("Responsive banner request error", e);
        }
    },

    CrazyGamesJs_ClearBanner: function(div) {
        const name = UTF8ToString(div);
        const element = document.getElementById(name);
        element.style.display = "none";
        window.CrazyGames.SDK.banner.clearBanner(name);
    },

    CrazyGamesJs_ClearAllBanners: function() {
        window.CrazyGames.SDK.banner.clearAllBanners();
    },

    CrazyGamesJs_IsUserAccountAvailable: function() {
        return window.CrazyGames.SDK.user.isUserAccountAvailable;
    },

    CrazyGamesJs_GetUser: function(callback) {
        CrazyGamesJs._luaGetUserCallback = callback;
        window.CrazyGames.SDK.user.getUser().then((user) => {
            CrazyGamesJs._callGetUserCallback(user);
        }).catch((e) => {
            console.log("Returning empty user");
            CrazyGamesJs._callGetUserCallback(null);
        });
    },

    CrazyGamesJs_GetUserToken: function(callback) {
        CrazyGamesJs._luaGetUserTokenCallback = callback;
        window.CrazyGames.SDK.user.getUserToken().then((token) => {
            console.log("token", token);
            CrazyGamesJs._callGetUserTokenCallback(token);
        }).catch ((e) => {
            console.log("Error:", e);
            CrazyGamesJs._callGetUserTokenCallback(null);
        });
    },

    CrazyGamesJs_GetXsollaUserToken: function(callback) {
        CrazyGamesJs._luaGetXsollaUserTokenCallback = callback;
        window.CrazyGames.SDK.user.getXsollaUserToken().then((token) => {
            console.log("token", token);
            CrazyGamesJs._callGetXsollaUserTokenCallback(token);
        }).catch ((e) => {
            console.log("Error:", e);
            CrazyGamesJs._callGetXsollaUserTokenCallback(null);
        });
    },

    CrazyGamesJs_ShowAuthPrompt: function(callback) {
        CrazyGamesJs._luaShowAuthPromptCallback = callback;
        window.CrazyGames.SDK.user.showAuthPrompt().then((user) => {
            CrazyGamesJs._callShowAuthPromptCallback(user);
        }).catch((e) => {
            console.log("Returning empty user");
            CrazyGamesJs._callShowAuthPromptCallback(null);
        });
    },

    CrazyGamesJs_SetAuthListener: function(callback) {
        CrazyGamesJs._luaAuthCallback = callback;
        window.CrazyGames.SDK.user.addAuthListener(CrazyGamesJs._authCallback);
    },

    CrazyGamesJs_RemoveAuthListener: function() {
        CrazyGamesJs._luaAuthCallback = null;
        window.CrazyGames.SDK.user.removeAuthListener(CrazyGamesJs._authCallback);
    },

    CrazyGamesJs_ShowAccountLinkPrompt: function(callback) {
        CrazyGamesJs._luaShowAccountLinkPromptCallback = callback;
        window.CrazyGames.SDK.user.showAccountLinkPrompt().then((response) => {
            CrazyGamesJs._callShowAccountLinkPromptCallback(response);
        }).catch((e) => {
            console.log("Show account link prompt error:", e);
            CrazyGamesJs._callShowAccountLinkPromptCallback(null);
        });
    }
}

autoAddDeps(LibCrazyGames, '$CrazyGamesJs');
addToLibrary(LibCrazyGames);
