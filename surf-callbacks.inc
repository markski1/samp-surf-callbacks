//
// Surf callbacks 0.1 by Markski
// mrks.cf - Markski#7243 - immarkski@pm.me
//
// Installation
//
// 		If you have y_hooks, simply include this file and everything will work by itself.
//
//		If you don't have y_hooks, then, besides from including the file you must:
//			- Call 'mrksSurf_InitializePlayer(playerid)' at the beggining of OnPlayerConnect
//			- Call 'mrksSurf_InitializeTimer()' at any point in OnGameModeInit
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

forward OnPlayerSurfVehicle(playerid, vehicleid);
forward OnPlayerStopSurfingVehicle(playerid, vehicleid);
forward OnPlayerJumpVehicleToVehicle(playerid, oldvehicleid, newvehicleid);

new mrks_SurfStatus[MAX_PLAYERS];

// timer func

forward mrks_CheckSurfers();
public mrks_CheckSurfers() {
	new auxInt;
	for (new i = 0; i < MAX_PLAYERS; i++) {
		if (!IsPlayerConnected(i)) continue;
		auxInt = GetPlayerSurfingVehicleID(i);
		if (auxInt == INVALID_VEHICLE_ID) {
			if (mrks_SurfStatus[i] != INVALID_VEHICLE_ID) {
				CallLocalFunction("OnPlayerStopSurfingVehicle", "ii", i, mrks_SurfStatus[i]);
				mrks_SurfStatus[i] = INVALID_VEHICLE_ID;
			}
		} else {
			if (mrks_SurfStatus[i] == INVALID_VEHICLE_ID) {
				CallLocalFunction("OnPlayerSurfVehicle", "ii", i, auxInt);
				mrks_SurfStatus[i] = auxInt;
			} else if (mrks_SurfStatus[i] != auxInt) {
				CallLocalFunction("OnPlayerJumpVehicleToVehicle", "iii", i, mrks_SurfStatus[i], auxInt);
				mrks_SurfStatus[i] = auxInt;
			}
		}
	}
}

#if defined _INC_y_hooks

hook OnPlayerConnect(playerid) {
	mrks_SurfStatus[playerid] = INVALID_VEHICLE_ID;
	return 1;
}

hook OnGameModeInit() {
	SetTimer("mrks_CheckSurfers", 1000, true);
	return 1;
}

#else

stock mrksSurf_InitializePlayer(playerid) {
	mrks_SurfStatus[playerid] = INVALID_VEHICLE_ID;
	return 1;
}

stock mrksSurf_InitializeTimer() {
	SetTimer("mrks_CheckSurfers", 1000, true);
	return 1;
}

#endif
