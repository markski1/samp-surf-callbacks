# SA-MP Surf Callbacks
Callbacks for SA-MP when players start/stop surfing a vehicle, or jump from a vehicle to another.

### Install

Simply include surf-callbacks.inc into your script and it'll work as if by magic.

### Usage

To use, you must define each callback as a public function anywhere in your script. The code within will be run when the callback's given condition is met. They're already forwarded in the include file.

Function | Behaviour
--- | ---
OnPlayerSurfVehicle(playerid, vehicleid) | Called when `playerid` starts surfing vehicle `vehicleid`.
OnPlayerStopSurfingVehicle(playerid, vehicleid) | Called when `playerid` "stops surfing vehicle `vehicleid`.
OnPlayerJumpVehicleToVehicle(playerid, oldvehicleid, newvehicleid) | Called when player `playerid` jumps from `oldvehicleid` to `newvehicleid`.

### Problems

Only known problem is that currently, because of a SA-MP design flaw or limitation, OnPlayerStopSurfingVehicle will be called if the player is still surfing a vehicle but it stops.
This is because SA-MP stops handling artificial surfing when a vehicle isn't moving.

I currently have a few ideas on how this can be solved and I'll be looking into applying them when I have time. Suggestions on this are welcome.

### Collaborating
Open issues or pull requests as you see fit.
