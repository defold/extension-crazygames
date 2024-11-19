---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to integrate and use the CrazyGames SDK in Defold.
---

# Defold CrazyGames SDK extension API documentation

This extension provides a CrazyGames SDK integration for Defold. [CrazyGames](https://www.crazygames.com/) is a popular browser games platform. Submit your own games via [developer.crazygames.com](https://developer.crazygames.com/).


![CrazyGames.com landing page](crazygames.jpg)

# Installation
To use CrazyGames SDK in your Defold project, add a version of the CrazyGames SDK extension to your `game.project` dependencies from the list of available [Releases](https://github.com/defold/extension-crazygames/releases). Find the version you want, copy the URL to ZIP archive of the release and add it to the project dependencies.

![](add-dependency.png)

Select `Project->Fetch Libraries` once you have added the version to `game.project` to download the version and make it available in your project.

# Modules

The SDK has the following modules:

* [`ad`](ad) - display video ads, detect adblockers
* [`banner`](banner) - display banners
* [`game`](game) - various game events
* [`user`](user) - for interacting with the currently logged in user
* [`data`](data) - new module in v3, that allows you to store user data that persists across devices


## Example

[Refer to the example project](https://github.com/defold/extension-crazygames/blob/master/main/crazygames.gui_script) to see a complete example of how the intergation works.


## Source code

The source code is available on [GitHub](https://github.com/defold/extension-crazygames)


## API reference
