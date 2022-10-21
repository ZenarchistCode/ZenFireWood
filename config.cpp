class CfgPatches
{
	class ZenFireWood
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Structures_Residential"
		};
		units[] =
		{
			"Zen_FireWood1",
		};
	};
};

class CfgMods
{
	class ZenFireWood
	{
		dir = "ZenFireWood";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "ZenFireWood";
		credits = "";
		author = "Zenarchist";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game","World","Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "ZenFireWood/scripts/3_Game"; };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "ZenFireWood/scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "ZenFireWood/scripts/5_Mission" };
			};
		};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	// Create invisible model that we can interact with and will envelope the wood reserve static object
	class Zen_WoodReserve : HouseNoDestruct
	{
		scope = 1;
		model = "ZenFireWood\data\objects\zen_woodreserve.p3d";
	};
	class Zen_WoodReserve_Debug : HouseNoDestruct
	{
		scope = 1;
		model = "ZenFireWood\data\objects\zen_woodreserve_debug.p3d";
	};
};