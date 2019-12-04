//
// Surf callbacks 0.1 by Markski
// mrks.cf - Markski#7243 - immarkski@pm.me
//
// Installation
//
// 		If you have y_hooks, simply include this file and everything will work by itself.
//
//		If you don't have y_hooks, then, besides from including the file you must:
//			- Call 'ocbSurf_InitializePlayer(playerid)' at the beggining of OnPlayerConnect
//			- Call 'ocbSurf_InitializeTimer()' at any point in OnGameModeInit
//
// Usage
//
// 		Define the callbacks as a public function anywhere in your script, they're forwarded here already.
//
// 		OnPlayerSurfVehicle
//			Called when a player "playerid" starts surfing vehicle "vehicleid".
//
// 		OnPlayerSurfObject
//			Called when a player "playerid" starts surfing object "objectid".
//
//		OnPlayerStopSurfingVehicle 
// 			Called when player "playerid" "stops surfing vehicle "vehicleid".
//
//		OnPlayerStopSurfingObject 
// 			Called when player "playerid" "stops surfing object "objectid".
//
//		OnPlayerJumpVehicleToVehicle
// 			Called when player "playerid" stops surfing vehicle "oldvehicleid" and starts surfing 
//			vehicle "newvehicleid", without ever touching the ground.
//
//		OnPlayerJumpObjectToObject
// 			Called when player "playerid" stops surfing object "oldobjectid" and starts surfing 
//			object "newobjectid", without ever touching the ground.
//
//		OnPlayerJumpVehicleToObject
// 			Called when player "playerid" stops surfing vehicle "vehicleid" and starts surfing 
//			object "objectid", without ever touching the ground.
//
//		OnPlayerJumpObjectToVehicle
// 			Called when player "playerid" stops surfing object "objectid" and starts surfing 
//			vehicle "vehicleid", without ever touching the ground.
//

// Function forwarding
forward OnPlayerSurfVehicle(playerid, vehicleid);
forward OnPlayerSurfObject(playerid, objectid);
forward OnPlayerStopSurfingVehicle(playerid, vehicleid);
forward OnPlayerStopSurfingObject(playerid, objectid);
forward OnPlayerJumpVehicleToVehicle(playerid, oldvehicleid, newvehicleid);
forward OnPlayerJumpObjectToObject(playerid, oldobjectid, newobjectid);
forward OnPlayerJumpVehicleToObject(playerid, vehicleid, objectid);
forward OnPlayerJumpObjectToVehicle(playerid, objectid, vehicleid);


// Macros
#define SURFING_NONE			(0)
#define SURFING_VEHICLE			(1)
#define SURFING_OBJECT			(2)

// Include gvars
static ocb_SurfID[MAX_PLAYERS];
static ocb_SurfType[MAX_PLAYERS];

// Functions

/*
	GetPlayerSurfing

	Receives a player ID and an integer by reference.
	The reference integer will contain what type of surfing the player is doing.
	The return value will be the ID of the object or vehicle that the player is surfing.
*/
stock GetPlayerSurfing(playerid, &type) {
	type = ocb_SurfType[playerid];
	return ocb_SurfID[playerid];
}

//////////////////////////////////
// Timer and internal functions //
//////////////////////////////////

forward ocb_CheckSurfers();
public ocb_CheckSurfers() {
	new auxInt;
	for (new i = 0; i < MAX_PLAYERS; i++) {
		if (!IsPlayerConnected(i)) continue;
		auxInt = GetPlayerSurfingVehicleID(i);
		if (ocb_HandleVehicleSurfing(i, auxInt)) continue;
		auxInt = GetPlayerSurfingObjectID(i);
		if (ocb_HandleObjectSurfing(i, auxInt)) continue;
		ocb_SurfType[i] = SURFING_NONE;
	}
}

/*
	ocb_HandleVehicleSurfing

	Internal function to handle the case in which a player might be surfing a vehicle.
	If the player's state changes to surfing it returns 1, causing the timer function to continue and avoid
	unnecesarily checking for object surf.
*/
static ocb_HandleVehicleSurfing(i, auxInt) {
	if (auxInt == INVALID_VEHICLE_ID) {
		if (ocb_SurfType[i] == SURFING_VEHICLE) {
			CallLocalFunction("OnPlayerStopSurfingVehicle", "ii", i, ocb_SurfID[i]);
			ocb_SurfID[i] = INVALID_VEHICLE_ID;
		}
	} else {
		if (ocb_SurfType[i] == SURFING_NONE) {
			CallLocalFunction("OnPlayerSurfVehicle", "ii", i, auxInt);
			ocb_SurfID[i] = auxInt;
			ocb_SurfType[i] = SURFING_VEHICLE;
			return 1;
		} else if (ocb_SurfType[i] == SURFING_VEHICLE && ocb_SurfID[i] != auxInt) {
			CallLocalFunction("OnPlayerJumpVehicleToVehicle", "iii", i, ocb_SurfID[i], auxInt);
			ocb_SurfID[i] = auxInt;
			return 1;
		} else if (ocb_SurfType[i] == SURFING_OBJECT) {
			CallLocalFunction("OnPlayerJumpObjectToVehicle", "iii", i, ocb_SurfID[i], auxInt);
			ocb_SurfID[i] = auxInt;
			ocb_SurfType[i] = SURFING_VEHICLE;
			return 1;
		}
	}
	return 0;
}

/*
	ocb_HandleVehicleSurfing

	Internal function to handle the case in which a player might be surfing an object.
	Return 1 if a player is surfing an object, no matter if the state changed or not
	To avoid setting their surf type to none
*/
static ocb_HandleObjectSurfing(i, auxInt) {
	if (auxInt == INVALID_OBJECT_ID) {
		if (ocb_SurfType[i] == SURFING_OBJECT) {
			CallLocalFunction("OnPlayerStopSurfingObject", "ii", i, ocb_SurfID[i]);
			ocb_SurfID[i] = INVALID_OBJECT_ID;
		}
	} else {
		if (ocb_SurfType[i] == SURFING_NONE) {
			CallLocalFunction("OnPlayerSurfObject", "ii", i, auxInt);
			ocb_SurfID[i] = auxInt;
			ocb_SurfType[i] = SURFING_OBJECT;
		} else if (ocb_SurfType[i] == SURFING_OBJECT && ocb_SurfID[i] != auxInt) {
			CallLocalFunction("OnPlayerJumpObjectToObject", "iii", i, ocb_SurfID[i], auxInt);
			ocb_SurfID[i] = auxInt;
		} else if (ocb_SurfType[i] == SURFING_VEHICLE) {
			CallLocalFunction("OnPlayerJumpVehicleToObject", "iii", i, ocb_SurfID[i], auxInt);
			ocb_SurfID[i] = auxInt;
			ocb_SurfType[i] = SURFING_OBJECT;
		}
		return 1;
	}
	return 0;
}

// HOOKS, if applicable

#if defined _INC_y_hooks

hook OnPlayerConnect(playerid) {
	ocbSurf_InitializePlayer(playerid);
	return 1;
}

hook OnGameModeInit() {
	ocbSurf_InitializeTimer();
	return 1;
}

#endif

ocbSurf_InitializePlayer(playerid) {
	ocb_SurfID[playerid] = INVALID_VEHICLE_ID;
	ocb_SurfType[playerid] = SURFING_NONE;
	return 1;
}

ocbSurf_InitializeTimer() {
	SetTimer("ocb_CheckSurfers", 1000, true);
	return 1;
}