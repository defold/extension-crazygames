// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibCrazyGames = {

    $CrazyGamesJs: {
        _callback: null,
        _inviteLinkParams: null,
    },

    CrazyGamesJs_ShowMidgameAd: function(callback) {
        CrazyGamesJs._callback = callback;
        const callbacks = {
            adFinished: () => {
                {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(1);
            },
            adError: (error) => {
                console.log("CrazyGamesJs_ShowMidgameAd adError", error);
                {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(0);
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
                {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(1);
            },
            adError: (error) => {
                console.log("CrazyGamesJs_ShowRewardedAd adError", error);
                {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(0);
            },
            adStarted: () => {
                // console.log("CrazyGamesJs_ShowRewardedAd adStarted");
            },
        };
        window.CrazyGames.SDK.ad.requestAd("rewarded", callbacks);
    },

    CrazyGamesJs_IsAdBlocked: async function(callback) {
        CrazyGamesJs._callback = callback;
        window.CrazyGames.SDK.ad.hasAdblock().then((result) => {
            {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(result ? 1 : 0);
        }).catch((error) => {
            console.log("CrazyGamesJs_IsAdBlocked error", error)
            {{{ makeDynCall("vi", "CrazyGamesJs._callback")}}}(0);
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
    }
}

autoAddDeps(LibCrazyGames, '$CrazyGamesJs');
addToLibrary(LibCrazyGames);
