# SA-MP Surf Callbacks
Callbacks for SA-MP when players start/stop surfing a vehicle, or jump from a vehicle to another.

### Install

If you have y_hooks, simply include the .inc file and everything will work as if by magic.

If you don't have y_hooks, then, besides from including the file you must:
	- Call 'mrksSurf_InitializePlayer(playerid)' at the beggining of OnPlayerConnect
	- Call 'mrksSurf_InitializeTimer()' at any point in OnGameModeInit

### Usage

To use, you must define each callback as a public function anywhere in your script. The code within will be run when the callback's given condition is met. They're already forwarded in the include file.

- OnPlayerSurfVehicle(playerid, vehicleid);
	Called when `playerid` starts surfing vehicle `vehicleid`.
- OnPlayerStopSurfingVehicle(playerid, vehicleid);
	Called when `playerid` "stops surfing vehicle `vehicleid`.
- OnPlayerJumpVehicleToVehicle(playerid, oldvehicleid, newvehicleid);
	Called when player `playerid` jumps from `oldvehicleid` to `newvehicleid`.

### Problems

Only known problem is that currently, because of a SA-MP design flaw or limitation, OnPlayerStopSurfingVehicle will be called if the player is still surfing a vehicle but it stops.
This is because SA-MP stops handling artificial surfing when a vehicle isn't moving.

I currently have a few ideas on how this can be solved and I'll be looking into applying them when I have time. Suggestions on this are welcome.

### Collaborating
Open issues or pull requests as you see fit.