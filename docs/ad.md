---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to show video ads using the CrazyGames SDK in Defold.
---

# Video ads

Please be sure to read CrazyGames' [advertisement requirements](https://docs.crazygames.com/requirements/ads/), since your game will be rejected without any feedback if it doesn't follow them.


## Video ads

CrazyGames support two different types of video ads: `midgame` and `rewarded`.

Midgame advertisements can happen when a user died, a level has been completed, etc.

Rewarded advertisements can be requested by the user in exchange for a reward (An additional life, a retry when the user died, a bonus starting item, extra starting health, etc.). Rewarded ads should be shown when users explicitly consent to watch an advertisement.

Midgame ad example:

```lua
crazygames.show_midgame_ad(function(self, result)
  if result then
    print("Midgame ad was successfully shown")
  else
    print("Request for midgame ad was unfilled")
  end
end)
```

Rewarded ad example:

```lua
crazygames.show_rewarded_ad(function(self, result)
  if result then
    print("Rewarded ad was successfully shown")
  else
    print("Rewarded ad was not shown due to an error")
  end
end)
```


Don't forget to mute the audio and pause the game when the ad starts and to unmute the audio and continue the game when the ad finishes/fails to load


## Adblock detection

You can use the code below to detect if the user has an adblocker.

```lua
local result = crazygames.has_adblock()
print("Adblock usage fetched", result)
```