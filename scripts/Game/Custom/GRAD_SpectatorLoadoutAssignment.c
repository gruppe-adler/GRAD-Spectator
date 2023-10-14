[BaseContainerProps(), SCR_BaseContainerCustomTitleField("m_sFactionKey")]
class GRAD_SpectatorLoadoutAssignment
{
	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Faction key of the faction the loadout will be assigned to", params: "", category: "GRAD Spectator")];
	protected string m_sFactionKey;
	
	[Attribute(defvalue: "", uiwidget: UIWidgets.EditBox, desc: "Name of the loadout to assign to spectator", params: "", category: "GRAD Spectator")]
	protected string m_sLoadoutName;
	
	//------------------------------------------------------------------------------------------------
	string GetLoadoutName()
	{
		return m_sLoadoutName;
	}

	//------------------------------------------------------------------------------------------------
	string GetFactionKey()
	{
		return m_sFactionKey;
	}
}