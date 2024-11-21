---
title: Defold CrazyGames SDK extension API documentation
brief: This manual covers how to use in-game purchase in the CrazyGames SDK in Defold.
---

# In-game purchases

CrazyGames have partnered with Xsolla to offer you the possibility to integrate in-game purchases more conveniently. Learn more about how to use Xsolla on the [CrazyGames developer pages](https://docs.crazygames.com/sdk/html5-v3/in-game-purchases/).


## Get Xsolla token

If you would like to use Xsolla via CrazyGames' custom-generated token, you can retrieve the Xsolla token like this:

```lua
crazygames.get_xsolla_user_token(function(self, token)
  print(token)
end)
```

CrazyGames recommend that you retrieve the token every time before using it, since the tokens are usually short-lived, for example only 1 hour. The SDK handles the token refresh.

