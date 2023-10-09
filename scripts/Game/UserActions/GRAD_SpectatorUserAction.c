
//------------------------------------------------------------------------------------------------
class GRAD_SpectatorUserAction : ScriptedUserAction
{
	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
	    return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBroadcastScript()
	{
	    return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		// Get playerId of the player controlling the entity
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity);
		
		// Get position of the entity
		vector pos = pUserEntity.GetOrigin();
		
		// Change position to be above and behind the entity
		pos[2] = pos[2] - 4;
		pos[1] = pos[1] + 3;
		
		// Enable spectator mode for given player
		//SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		//gameMode.EnableSpectator(playerId, pos);
		GRAD_Spectator specComp = GRAD_Spectator.GetInstance();
		specComp.EnableSpectator(playerId, pos);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Spectator";

		return true;
	}
}