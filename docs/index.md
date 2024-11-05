---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to integrate and use the CrazyGames SDK in in Defold.
---

# Defold CrazyGames SDK extension API documentation

This extension provides a CrazyGames SDK integration for Defold. [CrazyGames](https://www.crazygames.com/) is a popular browser games platform. Submit your own games via [developer.crazygames.com](https://developer.crazygames.com/).


![CrazyGames.com landing page](crazygames.jpg)

# Installation
To use CrazyGames SDK in your Defold project, add a version of the CrazyGames SDK extension to your `game.project` dependencies from the list of available [Releases](https://github.com/defold/extension-crazygames/releases). Find the version you want, copy the URL to ZIP archive of the release and add it to the project dependencies.

![](add-dependency.png)

Select `Project->Fetch Libraries` once you have added the version to `game.project` to download the version and make it available in your project.

# Usage


## Implement the gameplay events

Use the `crazygames.gameplay_start()` event to describe when users are playing your game (e.g. on first user interaction and unpause).

Use the `crazygames.gameplay_stop()` event to describe when users aren’t playing your game (e.g. level finish, game over, pause, quit to menu).

```lua
-- first level loads, player clicks anywhere
crazygames.gameplay_start()
-- player is playing
-- player loses round
crazygames.gameplay_stop()
-- game over screen pops up
```


## Implement loading events

Use the `crazygames.loading_start()` and `crazygames.loading_stop()` events to track when and how long the loading of your game takes.

```lua
-- assets are loading
crazygames.loading_start()
-- assets are loaded
crazygames.loading_stop()
```


## Implement midgame ads

Midgame advertisements can happen when a user died, a level has been completed, etc. Do not show a midgame ad on a navigational button (e.g. when clicking the main menu icon or opening the settings or opening the shop). [Learn more about midgame ads on the official CrazyGames developer pages](https://docs.crazygames.com/requirements/ads/#requirements-for-midgame-ads).


```lua
crazygames.show_midgame_ad(function(self, result)
  if result then
    print("Midgame ad was successfully shown")
  else
    print("Request for midgame ad was unfilled")
  end
end)
```


## Implement rewarded ads

Rewarded advertisements can be requested by the user in exchange for a reward (An additional life, a retry when the user died, a bonus starting item, extra starting health, etc.). Rewarded ads should be shown when users explicitly consent to watch an advertisement. [Learn more about rewarded ads on the official CrazyGames developer pages](https://docs.crazygames.com/requirements/ads/#requirements-for-rewarded-ads).


```lua
crazygames.show_rewarded_ad(function(self, result)
  if result then
    print("Rewarded ad was successfully shown")
  else
    print("Rewarded ad was not shown due to an error")
  end
end)
```


## Invite buytt

## Example

[Refer to the example project](https://github.com/defold/extension-crazygames/blob/master/main/crazygames.gui_script) to see a complete example of how the intergation works.


## Source code

The source code is available on [GitHub](https://github.com/defold/extension-crazygames)


## API reference
