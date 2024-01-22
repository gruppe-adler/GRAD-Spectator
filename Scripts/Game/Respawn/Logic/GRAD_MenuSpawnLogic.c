[BaseContainerProps(category: "Respawn")]
modded class SCR_MenuSpawnLogic : SCR_SpawnLogic
{
	[Attribute(defvalue: "", uiwidget: UIWidgets.Object, desc: "Spectator Loadout Assignments", params: "", category: "GRAD Spectator")];
	protected ref array<ref GRAD_SpectatorLoadoutAssignment> m_aSpectatorLoadoutAssignments;
	
	[Attribute("", uiwidget: UIWidgets.EditBox, category: "Respawn", desc: "Default spawn point for players to spawn in or empty if none", category: "GRAD Spectator")]
	protected string m_sForcedSpawnPoint;

	//------------------------------------------------------------------------------------------------
	override void OnPlayerEntityLost_S(int playerId)
	{
		//super.OnPlayerEntityLost_S(playerId);
		
		// Spawn the player at the spectator spawn point and enable spectator 5 sec later
		Spawn(playerId);
		
		Print("GRAD spectator - entity lost; create spectator", LogLevel.NORMAL);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool GetForcedLoadout(out SCR_BasePlayerLoadout loadout, Faction faction)
	{
		if (m_aSpectatorLoadoutAssignments.Count() == 0)
		{
			Print("GRAD Spectator - no spectator loadouts configured", LogLevel.WARNING);
			return false;
		}
		
		string loadoutName;
		
		foreach (GRAD_SpectatorLoadoutAssignment spectatorLoadoutAssignment : m_aSpectatorLoadoutAssignments)
		{
			if (spectatorLoadoutAssignment.GetFactionKey() == faction.GetFactionKey())
			{
				loadoutName = spectatorLoadoutAssignment.GetLoadoutName();
				Print(string.Format("GRAD Spectator - loadout name: %1", loadoutName), LogLevel.NORMAL);
				break;
			}
		}
		
		loadout = GetGame().GetLoadoutManager().GetLoadoutByName(loadoutName);
		
		if (!loadout)
		{
			Print(string.Format("GRAD Spectator - spawn logic did not find loadout by name: %1", loadoutName), LogLevel.WARNING);
			return false;
		}
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool GetForcedSpawnPoint(out SCR_SpawnPoint spawnPoint, string spawnPointName)
	{
		if (m_sForcedSpawnPoint.IsEmpty())
			return false;
		
		array<SCR_SpawnPoint> spawnPoints = SCR_SpawnPoint.GetSpawnPoints();
		
		foreach (SCR_SpawnPoint sp : spawnPoints)
		{
			if (sp.GetSpawnPointName() == spawnPointName)
			{
				spawnPoint = sp;
				break;
			}
		}
		
		if (!spawnPoint)
		{
			Print(string.Format("GRAD spectator - spectator spawn logic did not find spawn point by name: %1", m_sForcedSpawnPoint), LogLevel.WARNING);
			return false;
		}
		else
		{
			Print("GRAD Spectator - spectator spawn point found.");
		}
		
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void Spawn(int playerId, bool initial = false)
	{
		// Get the faction stored in the player controller of current player
		Faction targetFaction;
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		targetFaction = GetGame().GetFactionManager().GetFactionByKey(pc.GetPreviousFactionKey());
		
		// Request the server to set the faction
		GetPlayerFactionComponent_S(playerId).RequestFaction(targetFaction);

		// Get the loadout named in the settings of this spawn logic
		SCR_BasePlayerLoadout targetLoadout;
		if (!GetForcedLoadout(targetLoadout, targetFaction))
			targetLoadout = GetGame().GetLoadoutManager().GetRandomFactionLoadout(targetFaction);		

		// Request the server to set the loadout
		GetPlayerLoadoutComponent_S(playerId).RequestLoadout(targetLoadout);

		
		// Test if faction is set else re-run this spawn function
		Faction faction = GetPlayerFactionComponent_S(playerId).GetAffiliatedFaction();
		if (!faction)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		// Test if loadout is set else re-run this spawn function
		SCR_BasePlayerLoadout loadout = GetPlayerLoadoutComponent_S(playerId).GetLoadout();
		if (!loadout)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		// Get the spawn point named "Spectator" else select random one from the same faction		
		SCR_SpawnPoint spawnPoint;
		if (!GetForcedSpawnPoint(spawnPoint, "Spectator"))
			spawnPoint = SCR_SpawnPoint.GetRandomSpawnPointForFaction(faction.GetFactionKey());

		// Test if spawn point is set else re-run this spawn function
		if (!spawnPoint)
		{
			OnPlayerSpawnFailed_S(playerId);
			return;
		}

		// combine the spawn data (loadout and spawn point) and do the spawn
		SCR_SpawnPointSpawnData data = new SCR_SpawnPointSpawnData(loadout.GetLoadoutResource(), spawnPoint.GetRplId());
		if (GetPlayerRespawnComponent_S(playerId).CanSpawn(data))
			DoSpawn(playerId, data);
		else
			OnPlayerSpawnFailed_S(playerId);
		
		// enable the spectator after 5 sec of spawning; could be optimised to a test if the spawn is done
		if(!pc.IsSpectator())
			GetGame().GetCallqueue().CallLater(EnableSpectator, 5000, false, playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerSpawnFailed_S(int playerId)
	{
		super.OnPlayerSpawnFailed_S(playerId);

		int delay = Math.RandomFloat(900, 1100);
		GetGame().GetCallqueue().CallLater(Spawn, delay, false, playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DoSpawn(int playerId, SCR_SpawnData data)
	{
		// Request the server to do the spawn
		GetPlayerRespawnComponent_S(playerId).RequestSpawn(data);
	}

	//------------------------------------------------------------------------------------------------
	protected void EnableSpectator(int playerId)
	{
		// Enable spectator mode for given player
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		playerController.EnableSpectator();
	}
};