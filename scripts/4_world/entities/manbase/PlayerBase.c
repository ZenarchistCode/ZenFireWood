modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(ActionMineZenWoodStackHand, InputActionMap);
	}

	void ZenWood_SendMsg(string message)
	{
		if (!GetIdentity() || !IsAlive())
			return;

		GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(message), true, GetIdentity());
	}
};
