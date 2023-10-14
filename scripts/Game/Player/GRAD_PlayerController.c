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
		PrintFormat("GRAD Spectator - OnControlledEntityChanged() - FROM: %1 | TO: %2", from, to);
		
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(to);
		
		if (!character)
		{
			Print("GRAD Spectator - SCR_ChimeraCharacter missing");
			return;
		}
		
		m_previousFactionKey = character.GetFaction().GetFactionKey();
		
		m_previousEntity = to;
		
		super.OnControlledEntityChanged(from, to);
	}
	
	//------------------------------------------------------------------------------------------------
	void DisableSpectator()
	{
		Print("GRAD Spectator - DisableSpectator()", LogLevel.NORMAL);
		
		// If this is executed on the server, remote execute it on the client
		if (Replication.IsServer())
		{
			Print("GRAD Spectator - is server", LogLevel.NORMAL);
			Rpc(RpcDo_Broadcast_EnableSpectator, GetPlayerId());
			return;
		}
		
		/* ---------- */
		
		// Delete the spectator
		SCR_EntityHelper.DeleteEntityAndChildren(m_previousSpectator);
		
		SetPreviousSpectator(null);
				
		/* ---------- */
		
		// Disable the spectator context for keybind input to leave the spectator
		InputManager inputManager = GetGame().GetInputManager();
		if (!inputManager)
			return;
		
		// Disable the spectator context for keybind input to leave the spectator
		inputManager.ActivateContext("SpectatorContext", 0); // setting duration to 0 disables the context
		inputManager.RemoveActionListener("DisableSpectator", EActionTrigger.DOWN, DisableSpectator);
		
		// Show hint
		SCR_HintManagerComponent.GetInstance().ShowCustomHint( "is now disabled", "GRAD Spectator", 10.0);
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

		/* ---------- */
		
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
		
		SetPreviousSpectator(spectator);
		
		/* ---------- */
		
		InputManager inputManager = GetGame().GetInputManager();
		if (!inputManager)
			return;
		
		// Enable the spectator context for keybind input to leave the spectator
		inputManager.AddActionListener("DisableSpectator", EActionTrigger.DOWN, DisableSpectator);
		inputManager.ActivateContext("SpectatorContext", 3600000);
		
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
		
		PrintFormat("GRAD Spectator - playerId: %1", GetGame().GetPlayerController().GetPlayerId());
		
		if (playerId != GetGame().GetPlayerController().GetPlayerId())
			return;
		
		EnableSpectator();
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_Broadcast_DisableSpectator(int playerId)
	{
		// This function does not seem to be executed on every client but only on the client that I need to.
		// Perhaps player controller is not replicated from server to every client but only to the needed client
		
		Print("GRAD Spectator - RpcDo_Broadcast_DisableSpectator()", LogLevel.NORMAL);
		
		PrintFormat("GRAD Spectator - playerId: %1", GetGame().GetPlayerController().GetPlayerId());
		
		if (playerId != GetGame().GetPlayerController().GetPlayerId())
			return;
		
		DisableSpectator();
	}
};
