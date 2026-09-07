---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to show banner ads using the CrazyGames SDK in Defold.
---

# Banners

Please be sure to read CrazyGames' [advertisement requirements](https://docs.crazygames.com/requirements/ads/), since your game will be rejected without any feedback if it doesn't follow them.

The banner module contains functionality for displaying banners in game.


## Request banner

To begin, you need to have an HTML container of the banner size present on the screen. The extension will automatically add one to the bottom of your screen with id `banner-container`. If you need more than one banner or wish to position the banner differently it is possible to do so by using a [custom HTML5 page template](https://defold.com/manuals/html5/#customizing-html5-bundle).

Available sizes are:

* 728x90
* 300x250
* 320x50
* 468x60
* 320x100

```lua
crazygames.request_banner("banner-container", 728, 90)
```

Note: You can display no more than 2 in-game banners on the same screen at once.


## Request responsive banner

The responsive banners feature will request ads that fit into your container, without the need to specify or select a size beforehand. The resulting banners will have one of the following sizes: 970x90, 320x50, 160x600, 336x280, 728x90, 300x600, 468x60, 970x250, 300x250, 250x250 and 120x600. Only banners that fit into your container will be displayed, if your container cannot fit any of these sizes no ad will be rendered. The rendered banner is automatically vertically and horizontally centered into your container.

Your container size must be set to a non-null value:

```html
<div id="responsive-banner-container" style="width: 500px; height: 500px"></div>
```

```lua
crazygames.request_responsive_banner("responsive-banner-container")
```



### Refreshing banners and limitations
To refresh the banners, simply call the `request_banner` or `request_responsive_banner` functions again with the same container id.

The banners have the following limitations:

* There is a minimum delay of 30 seconds between banner refreshes. Requests made more frequently will fail.
* During a gaming session, banners can be refreshed up to 120 times. This limit applies separately to each banner size.


### Clearing the banners

The SDK provides 2 methods for clearing the banners:

```lua
crazygames.clear_banner("banner-container")
-- or
crazygames.clear_all_banners()
```

CrazyGames recommend that you clear the banners after hiding them. Otherwise, when you request new banners again, the old banners may still appear for a fraction of a second, which negatively impacts the user experience.
