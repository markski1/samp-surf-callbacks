//
// Surf callbacks 0.3 by Markski
// mrks.cf - Markski#7243 - immarkski@pm.me
//
// Installation
//
// 		If you have y_hooks, simply include this file after including y_hooks and everything will work by itself.
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
//		OnPlayerStopSurfingVehicle
// 			Called when player "playerid" "stops surfing vehicle "vehicleid".
//
//		OnPlayerJumpVehicleToVehicle
// 			Called when player "playerid" stops surfing vehicle "oldvehicleid" and starts surfing
//			vehicle "newvehicleid", without ever touching the ground.
//

// Function forwarding
forward OnPlayerSurfVehicle(playerid, vehicleid);
forward OnPlayerStopSurfingVehicle(playerid, vehicleid);
forward OnPlayerJumpVehicleToVehicle(playerid, oldvehicleid, newvehicleid);


// Macros
#define SURFING_NONE			(0)
#define SURFING_VEHICLE			(1)
//#define SURFING_OBJECT			(2)

// Include gvars
static ocb_SurfID[MAX_PLAYERS];
static ocb_SurfType[MAX_PLAYERS];

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
		ocb_SurfType[i] = SURFING_NONE;
	}
}

/*
	ocb_HandleVehicleSurfing

	Internal function to handle the case in which a player might be surfing a vehicle.
	If the player's state changes to surfing it returns 1, causing the timer function to continue
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
		} else if (ocb_SurfType[i] == SURFING_VEHICLE && ocb_SurfID[i] != auxInt) {
			CallLocalFunction("OnPlayerJumpVehicleToVehicle", "iii", i, ocb_SurfID[i], auxInt);
			ocb_SurfID[i] = auxInt;
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