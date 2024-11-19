---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to use the data module in the CrazyGames SDK in Defold.
---

# Data

The data module allows to save and retrieve user data for logged in CrazyGames users. The data will also be synced on all the devices where the user plays the game.

If the user is not logged in, the data module will store the game data in LocalStorage. If the user logs in later, the LocalStorage game data will be synced and backed up on the user's account.

WARNING - If you intend to use the data module, don't forget to select the appropriate toggle in the "Does your game save progress?" form when submitting the game. The data module will be disabled otherwise.


## Using the data module

The data module has a similar API as the [`localStorage`](https://developer.mozilla.org/en-US/docs/Web/API/Window/localStorage).

```lua
crazygames.clear_data()
local value = crazygames.get_item(key)
crazygames.remove_item(key)
crazygames.set_item(key, value)
```


## Guest user behaviour

For the guest users, the data module stores the game data in localStorage. When a guest user signs in, you don't need to do anything. Our SDK will automatically load the account game data if there is any, or if this user hasn't played your game before, the SDK will transfer the guest data to the user account.

When the user signs out, the SDK will revert back to using the guest game data.


## QA Tool

The data module works in QA Tool, however it doesn't sync the data. This means that regardless of what user you select in the User simulation modal, your game will always have the same data.


## Data saving

The SDK debounces data saving with 1 second, meaning that multiple calls to the `set_item` function will be saved after 1 second. There may be exceptions in various cases, when data saving may be debounced with more time, up to 30 seconds.

There is a 1MB data limit. If you are approaching it, you will see warnings in the browser console. The data won't be backed up anymore if it exceeds 1MB.