
<p align="center">
    <img src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/9b8ebab9-7b92-4c4f-bdb2-bb589bb91194.png">
</p>

<p align="center">
    <a href="https://github.com/y0014984/GRAD-Spectator/releases/latest">
        <img src="https://img.shields.io/badge/Version-0.1.0-blue.svg?style=flat-square" alt="GRAD Spectator Version">
    </a>
    <a href="https://www.bistudio.com/community/licenses/arma-public-license-share-alike">
        <img src="https://img.shields.io/badge/License-APL-red.svg?style=flat-square" alt="GRAD Spectator License">
    </a>
</p>

# GRAD-Spectator

## Summary
GRAD Spectator is a mod for Arma Reforger that enables a spectator mode via respawn or manually.

> [!WARNING]
> Use at your own risk. This mod is only for testing and still under development.

## Features
- custom action to enable spectator (GRAD_SpectatorUserAction)
- 6 flag poles (2x US, 2x USSR, 2x FIA) will spectator action attached
- custom spectator spawn logic (enter spectator after first death)
- custom keybind to leave spectator (END - can be configured in keybinds)

## How to enable/disable spectator via script
```c#
// enable/disable spectator no matter if executed on server or client

SCR_PlayerController playerController;
playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));

playerController.EnableSpectator();

playerController.DisableSpectator();
```

## How to configure custom spectator spawn logic
1. Add a loadout to every faction in your gamemode that can be assigned to spectators (In LoadoutManager)
2. Change the Spawn Logic in the SCR_RespawnSystemComponent of your GameMode to GRAD_SpectatorSpawnLogic
3. In SpawnLogic configure the Spectator Loadout Assignments
4. In SpawnLogic enter the name of the spectator spawn point
5. Place a spawn point without faction in your world and sets its name like in the SpawnLogic (Step 4)

<img width="413" alt="Screenshot 2023-10-14 165704" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/8e4ba574-30c0-4d87-b370-b4dce302e666">
<img width="414" alt="Screenshot 2023-10-14 170208" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/4217cf45-b67b-4f1c-9b7c-03a176c63f65">
<img width="412" alt="Screenshot 2023-10-14 170308" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/002abc6a-38ce-44e3-b4f3-df79487abbee">

## Links
- Workshop: https://reforger.armaplatform.com/workshop/???
- Discord: https://discord.com/invite/ZDqp45q
- Website: https://gruppe-adler.de/home
- Forum: https://forum.gruppe-adler.de/

## License
GRAD Undead is licensed under the ([APL](https://www.bohemia.net/community/licenses/arma-public-license)).
