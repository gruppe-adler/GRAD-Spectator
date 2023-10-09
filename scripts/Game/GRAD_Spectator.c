// this class adds a new function to enable the spectator mode
// for any player; I found this on the Arma Discord Server. 

[EntityEditorProps(category: "GRAD Entities", description: "Allows players to enter spectator mode")]
class GRAD_SpectatorClass : GenericEntityClass
{
}

class GRAD_Spectator : GenericEntity
{
	protected static GRAD_Spectator s_Instance;
	
	private RplComponent m_RplComponent;

	//------------------------------------------------------------------------------------------------
	void GRAD_Spectator(IEntitySource src, IEntity parent)
	{
		if (s_Instance)
		{
			Print("Only one instance of GRAD_Spectator is allowed in the world!", LogLevel.WARNING);
			delete this;
			return;
		}

		s_Instance = this;
	}
	
	//------------------------------------------------------------------------------------------------	
	static GRAD_Spectator GetInstance()
	{
		return s_Instance;
	}
	
	//------------------------------------------------------------------------------------------------

	override protected void EOnInit(IEntity owner)
	{
		m_RplComponent = RplComponent.Cast(FindComponent(RplComponent));
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableSpectator(int playerId, vector pos)
	{
		if (!m_RplComponent)
			m_RplComponent = RplComponent.Cast(FindComponent(RplComponent));
		
		PrintFormat("GRAD Spectator - RplComponent: %1", m_RplComponent);

		if (m_RplComponent.Role() == RplRole.Authority)
		{
			Print("GRAD Spectator - EnableSpectator triggered on authority");
		}
		else
		{
			Print("GRAD Spectator - EnableSpectator triggered NOT on authority");
		}
		
		Rpc(RpcDo_EnableSpectator, playerId, pos);
		
		// Printing the new spectator to the console
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		PrintFormat("GRAD Spectator - player '%1' is now spectator", GetGame().GetPlayerManager().GetPlayerName(playerId));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_EnableSpectator(int playerId, vector pos)
	{
		// This function is broadcasted to all clients (not to the server), but it is only executed on the client
		// that has the same playerId as the local player controller
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController || playerController.GetPlayerId() != playerId) return;
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform[3] = pos;
		Resource r = Resource.Load("{E1FF38EC8894C5F3}Prefabs/Editor/Camera/ManualCameraSpectate.et");
		IEntity spectator = GetGame().SpawnEntityPrefab(r, GetGame().GetWorld(), params);
		
		playerController.SetPreviousSpectator(spectator);
		
		// both works: setControlledEntity and setPossessedEntity
		//if (ce) playerController.SetControlledEntity(ce);
		
		//if (ce) playerController.SetPossessedEntity(ce);
		//PrintFormat("GRAD Spectator - is possessing: %1", playerController.IsPossessing());
		
		//playerController.SetPossessedEntity(null);
		//PrintFormat("GRAD Spectator - is possessing: %1", playerController.IsPossessing());

		// Enable the spectator context for keybind input to leave the spectator
		InputManager inputManager = GetGame().GetInputManager();
		inputManager.ActivateContext("SpectatorContext", 3600000);
		
		Print("GRAD Spectator - (Broadcast) EnableSpectator");
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableSpectator(int playerId)
	{
		if (!m_RplComponent)
			m_RplComponent = RplComponent.Cast(FindComponent(RplComponent));
		
		PrintFormat("GRAD Spectator - RplComponent: %1", m_RplComponent);

		if (m_RplComponent.Role() == RplRole.Authority)
		{
			Print("GRAD Spectator - DisableSpectator triggered on authority");
		}
		else
		{
			Print("GRAD Spectator - DisableSpectator triggered NOT on authority");
		}
		
		Rpc(RpcAsk_DisableSpectator, playerId);
		
		// Printing the new spectator to the console
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		PrintFormat("GRAD Spectator - player '%1' is no more spectator", GetGame().GetPlayerManager().GetPlayerName(playerId));
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_DisableSpectator(int playerId)
	{
		// This function is only executed on the server
		
		Print("GRAD Spectator - (Broadcast) DisableSpectator");
		
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		
		IEntity controlledEntity = playerController.GetPreviousEntity();
		IEntity spectator = playerController.GetPreviousSpectator();
		
		PrintFormat("GRAD Spectator - PlayerController: %1", playerController);
		PrintFormat("GRAD Spectator - ControlledEntity: %1", controlledEntity);
		PrintFormat("GRAD Spectator - Spectator: %1", spectator);

		playerController.SetPreviousSpectator(null);
		
		// both works: setControlledEntity and setPossessedEntity
		//if (controlledEntity) playerController.SetControlledEntity(controlledEntity);
		//if (controlledEntity) playerController.SetPossessedEntity(controlledEntity);
		
		SCR_EntityHelper.DeleteEntityAndChildren(spectator);
				
		//Vielleicht auch das hier zum löschen
		//RplComponent.DeleteRplEntity(controlledEntity, false);
	}
}

// No matter which camera i choose, the effect is always the same
// I should investigate further

//{E1FF38EC8894C5F3}Prefabs/Editor/Camera/ManualCameraSpectate.et

//{0D9A6EC9C72BF397}Prefabs/Editor/Camera/ManualCameraStrategy.et

//{127C64F4E93A82BC}Prefabs/Editor/Camera/ManualCameraPhoto.et