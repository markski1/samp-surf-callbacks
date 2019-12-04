# SA-MP Surf Callbacks
Callbacks for SA-MP when players start/stop surfing a vehicle, or jump from a vehicle to another.

### Install

If you have y_hooks, simply include the .inc file and everything will work as if by magic.

If you don't have y_hooks, then, besides from including the file you must:
	- Call 'mrksSurf_InitializePlayer(playerid)' at the beggining of OnPlayerConnect
	- Call 'mrksSurf_InitializeTimer()' at any point in OnGameModeInit

### Usage

To use, you must define each callback as a public function anywhere in your script. The code within will be run when the callback's given condition is met. They're already forwarded in the include file.

Description and instructions per each callback are in the comments of the .inc itself.
    
### Collaborating
Just make a PR m8
