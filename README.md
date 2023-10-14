
<p align="center">
    <img src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/21e2db8a-ee89-42ee-b7f3-222bdaef27a5.png">
</p>

<p align="center">
    <a href="https://github.com/y0014984/GRAD-Spectator/releases/latest">
        <img src="https://img.shields.io/badge/Version-0.1.1-blue.svg?style=flat-square" alt="GRAD Spectator Version">
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

<img width="412" alt="Screenshot 2023-10-14 170308" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/237fbdc6-25fe-45eb-82e0-31200abfedf2">
<img width="414" alt="Screenshot 2023-10-14 170208" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/a8b569e5-3c24-490e-8a2b-90008bedb9f8">
<img width="413" alt="Screenshot 2023-10-14 165704" src="https://github.com/y0014984/GRAD-Spectator/assets/50139270/3f19b207-4381-4694-9210-7e067971cc08">


## Links
- Workshop: https://reforger.armaplatform.com/workshop/5E6EE1F29BD43311-GRADSpectator
- GitHub: https://github.com/gruppe-adler/GRAD-Spectator
- Discord: https://discord.com/invite/ZDqp45q
- Website: https://gruppe-adler.de/home
- Forum: https://forum.gruppe-adler.de

## License
GRAD Undead is licensed under the ([APL](https://www.bohemia.net/community/licenses/arma-public-license)).
