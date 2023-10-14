// this class modification adds a new member variable to store the faction key of the 
// current controlled entity; The key is set every time the controlled entity changes
// This modification is needed in the GRAD_SpectatorSpawnLogic to determine the faction
// of the player for his spectator respawn

//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController : PlayerController
{
	protected FactionKey m_previousFactionKey;
	protected IEntity m_previousEntity;
	protected IEntity m_previousSpectator;
	protected bool m_bIsSpectator = false;
	
	private RplComponent m_RplComponent;

	//------------------------------------------------------------------------------------------------
	FactionKey GetPreviousFactionKey()
	{
		return m_previousFactionKey;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPreviousFactionKey(FactionKey factionKey)
	{
		m_previousFactionKey = factionKey; 
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetPreviousEntity()
	{
		return m_previousEntity;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPreviousEntity(IEntity entity)
	{
		m_previousEntity = entity; 
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity GetPreviousSpectator()
	{
		return m_previousSpectator;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPreviousSpectator(IEntity spectator)
	{
		m_previousSpectator = spectator; 
		if(spectator)
			m_bIsSpectator = true;
		else
			m_bIsSpectator = false;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsSpectator()
	{
		return m_bIsSpectator;
	}

	//------------------------------------------------------------------------------------------------	
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(to);
		
		if (!character)
		{
			Print("GRAD Spectator - SCR_ChimeraCharacter missing");
			return;
		}
		
		SetPreviousFactionKey(character.GetFaction().GetFactionKey());
		
		SetPreviousEntity(to);
		
		EnableSpectatorHotkey();
		
		super.OnControlledEntityChanged(from, to);
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableSpectatorHotkey()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			//Print("GRAD Spectator - Input Manager available");

			inputManager.AddActionListener("DisableSpectator", EActionTrigger.DOWN, DisableSpectator);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableSpectator()
	{
		Print("GRAD Spectator - DisableSpectator()", LogLevel.NORMAL);
		
		// Disable spectator mode for given player
		DisableSpectator_2(GetGame().GetPlayerController().GetPlayerId());
		
		PrintFormat("GRAD Spectator - playerId %1", GetGame().GetPlayerController().GetPlayerId());
		
		// Disable the spectator context for keybind input to leave the spectator
		InputManager inputManager = GetGame().GetInputManager();
		inputManager.ResetContext("SpectatorContext");
		
		SCR_HintManagerComponent.GetInstance().ShowCustomHint( "is now disabled", "GRAD Spectator", 10.0);
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableSpectator_2(int playerId)
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
	
	//------------------------------------------------------------------------------------------------
	void EnableSpectator()
	{
		Print("GRAD Spectator - EnableSpectator()", LogLevel.NORMAL);
		
		// If this is executed on the server, remote execute it on the client
		if (Replication.IsServer())
		{
			Print("GRAD Spectator - is server", LogLevel.NORMAL);
			Rpc(RpcDo_Broadcast_EnableSpectator, GetPlayerId());
			return;
		}
		
		// Get entities position
		vector pos = GetControlledEntity().GetOrigin();
		
		// Change position to be above and behind the entity
		pos[2] = pos[2] - 4;
		pos[1] = pos[1] + 3;
		
		// Spawn the spectator
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform[3] = pos;
		Resource r = Resource.Load("{E1FF38EC8894C5F3}Prefabs/Editor/Camera/ManualCameraSpectate.et");
		IEntity spectator = GetGame().SpawnEntityPrefab(r, GetGame().GetWorld(), params);
		
		// No matter which camera i choose, the effect is always the same
		// I should investigate further
		//{E1FF38EC8894C5F3}Prefabs/Editor/Camera/ManualCameraSpectate.et
		//{0D9A6EC9C72BF397}Prefabs/Editor/Camera/ManualCameraStrategy.et
		//{127C64F4E93A82BC}Prefabs/Editor/Camera/ManualCameraPhoto.et
		
		// Enable the spectator context for keybind input to leave the spectator
		InputManager inputManager = GetGame().GetInputManager();
		inputManager.ActivateContext("SpectatorContext", 3600000);
		
		SetPreviousSpectator(spectator);
		
		// Show hint
		SCR_HintManagerComponent.GetInstance().ShowCustomHint( "is now enabled", "GRAD Spectator", 10.0);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_Broadcast_EnableSpectator(int playerId)
	{
		// This function does not seem to be executed on every client but only on the client that I need to.
		// Perhaps player controller is not replicated from server to every client but only to the needed client
		
		Print("GRAD Spectator - RpcDo_Broadcast_EnableSpectator()", LogLevel.NORMAL);
		
		PrintFormat("GRAD Spectator - local playerId: %1", GetGame().GetPlayerController().GetPlayerId());
		
		if (playerId != GetGame().GetPlayerController().GetPlayerId())
			return;
		
		EnableSpectator();
	}
};
