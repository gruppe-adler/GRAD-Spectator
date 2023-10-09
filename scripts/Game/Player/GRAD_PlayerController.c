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
	static void DisableSpectator()
	{
		Print("GRAD Spectator - disabled");
		
		// Disable spectator mode for given player
		//SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		//gameMode.DisableSpectator(GetGame().GetPlayerController().GetPlayerId());
		GRAD_Spectator specComp = GRAD_Spectator.GetInstance();
		specComp.DisableSpectator(GetGame().GetPlayerController().GetPlayerId());
		
		PrintFormat("GRAD Spectator - playerId %1", GetGame().GetPlayerController().GetPlayerId());
		
		// Disable the spectator context for keybind input to leave the spectator
		InputManager inputManager = GetGame().GetInputManager();
		inputManager.ResetContext("SpectatorContext");
		
		SCR_HintManagerComponent.GetInstance().ShowCustomHint( "is now disabled", "GRAD Spectator", 10.0);
	}
};
