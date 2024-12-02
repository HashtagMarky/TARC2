VyratonIndoors_SSPathfinder_MapScripts::
# 3 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_SSPathfinder_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_SSPathfinder_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 1, VyratonIndoors_SSPathfinder_EventScript_MeetGrandparent
	.2byte 0


VyratonIndoors_SSPathfinder_EventScript_MeetGrandparent::
# 11 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 12 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setdynamicwarp MAP_VYRATON_OUTDOORS_HARBOUR, 15, 25
	return

