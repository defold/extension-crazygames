// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibCrazyGames = {

    $CrazyGamesJs: {
        _luaCallback: null,
        _luaAuthCallback: null,
        _inviteLinkParams: null,

        _successCallback: function(success) {
            console.log("_successCallback", success, CrazyGamesJs._callback);
            if (_luaCallback == null) return;
            var msg = success ? 1 : 0;
            {{{ makeDynCall("vi", "CrazyGamesJs._luaCallback")}}}(msg);
        },

        _errorCallback: function(error) {
            console.log("_errorCallback", error, CrazyGamesJs._callback);
            if (_luaCallback == null) return;
            {{{ makeDynCall("vi", "CrazyGamesJs._luaCallback")}}}(0);
        },

        _authCallback: function(user) {
            console.log("_authCallback", user);
            if (_luaAuthCallback == null) return;
            if (user != null) {
                const userJson = JSON.stringify(user);
                {{{ makeDynCall("vi", "CrazyGamesJs._luaAuthCallback")}}}(stringToUTF8OnStack(userJson));
            }
            else {
                {{{ makeDynCall("vi", "CrazyGamesJs._luaAuthCallback")}}}(stringToUTF8OnStack(""));
            }
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
        CrazyGamesJs._callback = callback;
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

    CrazyGamesJs_IsAdBlocked: function(callback) {
        CrazyGamesJs._luaCallback = callback;
        window.CrazyGames.SDK.ad.hasAdblock().then((result) => {
            // console.log("CrazyGamesJs_IsAdBlocked", result)
            CrazyGamesJs._successCallback(result);
        }).catch((error) => {
            // console.log("CrazyGamesJs_IsAdBlocked error", error)
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
        return stringToUTF8OnStack(value);
    },

    CrazyGamesJs_HappyTime: function() {
        window.CrazyGames.SDK.game.happytime();
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
        return value ? stringToUTF8OnStack(value) : null;
    },

    CrazyGamesJs_RemoveItem: function(key) {
        window.CrazyGames.SDK.data.removeItem(UTF8ToString(key));
    },

    CrazyGamesJs_SetItem: function(key, value) {
        window.CrazyGames.SDK.data.setItem(UTF8ToString(key), UTF8ToString(value));
    },

    CrazyGamesJs_IsUserAccountAvailable: function() {
        return window.CrazyGames.SDK.user.isUserAccountAvailable;
    },

    CrazyGamesJs_GetUser: function() {
        window.CrazyGames.SDK.user.getUser().then(user != null) {
            const userJson = JSON.stringify(user);
            console.log("Returning user as JSON", userJson);
            CrazyGamesJs._authCallback(userJson);
        }.catch((e) => {
            console.log("Returning empty user");
            CrazyGamesJs._authCallback(null);
        });
    },

    CrazyGamesJs_GetUserToken: function() {
        window.CrazyGames.SDK.user.getUserToken().then((token) => {
            console.log("token", token);
            return stringToUTF8OnStack(token);
        }).catch ((e) => {
            console.log("Error:", e);
            return 0;
        });
    },

    CrazyGamesJs_ShowAuthPrompt: function() {
        window.CrazyGames.SDK.user.showAuthPrompt().then((user) = {
            if (user != null) {
                const userJson = JSON.stringify(user);
                return stringToUTF8OnStack(userJson);
            }
            else {
                return 0;
            }
        }).catch ((e) {
            console.log("Error:", e);
            return 0;
        });
    },
    CrazyGamesJs_AddAuthListener: function(callback) {
        CrazyGamesJs._luaAuthCallback = callback;
        window.CrazyGames.SDK.user.addAuthListener(CrazyGamesJs._authCallback);
    },

    CrazyGamesJs_RemoveAuthListener: function() {
        CrazyGamesJs._luaAuthCallback = null;
        window.CrazyGames.SDK.user.removeAuthListener(CrazyGamesJs._authCallback);
    },

    CrazyGamesJs_ShowAccountLinkPrompt: function() {
        window.CrazyGames.SDK.user.showAccountLinkPrompt().then((response) => {
            console.log("response:", response);
        }).then((e) => {
            console.log("Error:", e);
        });
    }
}

autoAddDeps(LibCrazyGames, '$CrazyGamesJs');
addToLibrary(LibCrazyGames);
