/*
	samp-surf-callbacks 1.0

	mrks.cf | github.com/markski1

	Thanks to:
		open.mp/docs contributors
		SACNR
*/

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

public OnPlayerConnect(playerid) {
	ocb_SurfID[playerid] = INVALID_VEHICLE_ID;
	ocb_SurfType[playerid] = SURFING_NONE;

	#if defined ocbSurf_OnPlayerConnect
		return ocbSurf_OnPlayerConnect(playerid);
	#endif
	return 1;
}

public OnGameModeInit() {
	SetTimer("ocb_CheckSurfers", 1000, true);

	#if defined ocbSurf_OnGameModeInit
		ocbSurf_OnGameModeInit();
	#endif
	return 1;
}


#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect ocbSurf_OnPlayerConnect

#if defined ocbSurf_OnPlayerConnect
	forward ocbSurf_OnPlayerConnect();
#endif

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit ocbSurf_OnGameModeInit

#if defined ocbSurf_OnGameModeInit
	forward ocbSurf_OnGameModeInit();
#endif