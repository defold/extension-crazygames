---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to use the game module in the CrazyGames SDK in Defold.
---

# Game

The game module contains various functionality related to the game.



## Happy time

The `happytime()`` method can be called on various player achievements (beating a boss, reaching a highscore, etc.). It makes the website celebrate (for example by launching some confetti). There is no need to call this when a level is completed, or an item is obtained.

```lua
crazygames.happytime()
```


## Gameplay start/stop

CrazyGames provide functions that enable them to track when and how users are playing your games. These can be used to ensure their site does not perform resource intensive actions while a user is playing.

The `gameplay_start()`` function has to be called whenever the player starts playing or resumes playing after a break (menu/loading/achievement screen, game paused, etc.).

The `gameplay_stop()` function has to be called on every game break (entering a menu, switching level, pausing the game, ...) don't forget to call `gameplay_start()` when the gameplay resumes

```lua
-- player pauses the game
crazygames.gameplay_stop()
-- player resumes the game
crazygames.gameplay_start()
```


## Game loading start/stop

CrazyGames provide functions that enable them to track when and how long the loading of your game takes.

The `loading_start()` function has to be called whenever you start loading your game.

The `loading_stop()` function has to be called when the loading is complete and eventually the gameplay starts.

Note: The `loading_start()` and `loading_stop()` are called automatically when the game is loading the first time. You only need to call the functions if you are loading additional assets after the game has started.


```lua
-- next level's assets are loading now
crazygames.loading_start()
-- assets are loaded
crazygames.loading_stop()
```


## Invite link

This feature lets you share the CrazyGames version of your game to the players and invite them to join a multiplayer game. You can call `invite_link()` with a map of parameters that correspond to your game or game room.

```lua
local link = crazygames.invite_link({
  roomId = 12345,
  param1 = "value1",
  param2 = "value2",
})
```


## Invite button

This feature allows you to display a button in the game footer, that opens a popup containing the invite link. The returned link is similar to the link returned from Invite link.

The invite button should only be used to invite players to a multiplayer gaming session. Please avoid using it for other use cases, such as a "Share" button for example, as this may lead to delayed submission check or even game rejection.

You can show the invite button like this:

```lua
crazygames.show_invite_button({
  roomId = 12345,
  param1 = "value1",
  param2 = "value2",
})
```

Don't forget to hide the invite button when it is no longer necessary:

```lua
crazygames.hide_invite_button()
```
