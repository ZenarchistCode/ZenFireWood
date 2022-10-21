modded class MissionServer
{
	// Called when server mission initializes
	override void OnInit()
	{
		super.OnInit();

		// Dump objects / spawn firewood piles
		if (GetGame().IsMultiplayer() && !GetGame().IsClient() && GetGame().IsServer())
		{
			// Dump object locations
			if (GetZenFirewoodConfig().DumpObjectLocations)
			{
				// Wait 20 secs to ensure all vanilla + modded items are loaded in
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpObjects, 20000, false);
			}
			else
			if (GetZenFirewoodConfig().SpawnFirewoodObjects || GetZenFirewoodConfig().DebugOn)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 20000, false);
			}
		}
	}

	// Sets up wood piles across the map
	void SetupWoodPiles()
	{
		ZenFirewoodLogger.Log("Setting up wood piles...");
		string woodModel = "Zen_WoodReserve";
		int minWood;
		int maxWood;
		vector objOffset;
		vector objOrient;

		if (GetZenFirewoodConfig().DebugOn)
			woodModel = "Zen_WoodReserve_Debug";

		foreach (ZenFirewoodObject wood : GetZenFirewoodConfig().WoodPositions)
		{
			objOffset = GetZenFirewoodConfig().GetOffset(wood.TypeName);
			objOrient = GetZenFirewoodConfig().GetOrient(wood.TypeName);

			foreach (vector loc : wood.Locations)
			{
				// Get objects within 1 meter of the config'd wood pile vector
				array<Object> objectsNearWoodpile = new array<Object>;
				GetGame().GetObjectsAtPosition(loc, 1, objectsNearWoodpile, null);

				// Prepare reused variables
				string debugName = "";
				string className = "";

				// Check to make sure we haven't got an existing interactive wood pile on this location due to persistence issues
				foreach(Object z_obj : objectsNearWoodpile)
				{
					className = z_obj.ClassName();
					className.ToLower();

					if (className.Contains("zen_wood"))
					{
						GetGame().ObjectDelete(z_obj);
						break;
					}
				}

				foreach (Object obj : objectsNearWoodpile)
				{
					// Get classname(s)
					debugName = obj.GetDebugNameNative();
					className = obj.ClassName();

					// Convert to lower
					debugName.ToLower();
					className.ToLower();

					// Skip any existing zen wood piles
					if (className.Contains("zen_wood"))
					{
						continue;
					}

					// If map object is what we're looking for, attach an interactive wood pile object to it
					if (debugName.Contains(wood.TypeName) || className.Contains(wood.TypeName))
					{
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateWoodPile, 1, false, obj, woodModel, objOffset, objOrient);
					}
				}
			}
		}

		ZenFirewoodLogger.Log("Done.");
	}

	// Creates an interactive object on the static wood piles
	static void CreateWoodPile(Object parentObj, string childObjType, vector offset, vector orient)
	{
		if (!parentObj)
			return;

		childObjType.ToLower();
		Object newObj = GetGame().CreateObject(childObjType, vector.Zero);
		newObj.SetPosition(parentObj.GetPosition() + offset);
		newObj.SetOrientation(parentObj.GetOrientation() + orient);
		newObj.Update();
	}

	// Dump all objects around the map that match our typenames.
	void DumpObjects()
	{
		ZenFirewoodLogger.Log("Start object dump.");

		// Get all objects on the map in a 20km radius from the center of that 20km radius (enough for most maps?)
		array<Object> objectsOnMap = new array<Object>;
		GetGame().GetObjectsAtPosition(Vector(10000, 0, 10000), 20000, objectsOnMap, null);
		int objCount = 0;

		foreach(ZenFirewoodType woodType : GetZenFirewoodConfig().WoodTypes)
		{
			string type = woodType.TypeName;
			type.ToLower();

			// Prepare this object
			ZenFirewoodObject woodObj = new ZenFirewoodObject;
			woodObj.TypeName = type;
			array<vector> objLocations = new array<vector>;

			// Cycle through all map objects
			for (int x = 0; x < objectsOnMap.Count(); x++)
			{
				// Prepare classname(s)
				string debugName = "";
				string className = "";

				// Get classname(s)
				debugName = objectsOnMap[x].GetDebugNameNative();
				className = objectsOnMap[x].ClassName();

				// Convert to lower
				debugName.ToLower();
				className.ToLower();

				// If map object is what we're looking for, add it to the array
				if (debugName.Contains(type) || className.Contains(type))
				{
					objLocations.Insert(objectsOnMap[x].GetPosition());
					objCount++;
				}
			}

			// Save this wood object type & all its locations
			woodObj.Locations = objLocations;
			GetZenFirewoodConfig().WoodPositions.Insert(woodObj);
		}

		// Save objects and disable object dump for next restart
		ZenFirewoodLogger.Log("End object dump - found " + objCount + " relevant objects out of a total of " + objectsOnMap.Count() + " map objects.");
		GetZenFirewoodConfig().DumpObjectLocations = false;
		GetZenFirewoodConfig().Save();

		// Clear array & delete from memory
		objectsOnMap.Clear();
		objectsOnMap = NULL;

		// Setup wood piles
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 5000, false);
	}
}