// https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

var LibCrazyGames = {

    $CrazyGamesJs: {
        _callback: null
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

    CrazyGamesJs_HappyTime: function() {
        window.CrazyGames.SDK.game.happytime();
    },

    CrazyGamesJs_GameplayStart: function() {
        window.CrazyGames.SDK.game.gameplayStart();
    },

    CrazyGamesJs_GameplayStop: function() {
        window.CrazyGames.SDK.game.gameplayStop();
    }
}

autoAddDeps(LibCrazyGames, '$CrazyGamesJs');
addToLibrary(LibCrazyGames);
