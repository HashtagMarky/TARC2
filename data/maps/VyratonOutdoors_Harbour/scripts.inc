VyratonOutdoors_Harbour_MapScripts::
# 3 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 0, VyratonOutdoors_Harbour_EventScript_ShipLeave
	.2byte 0


VyratonOutdoors_Harbour_EventScript_ShipLeave::
# 14 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	addvar VAR_IKIGAI_INTRO_STATE, 1
# 15 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	playse SE_RG_SS_ANNE_HORN
# 16 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Samuel_BeginEntrance
# 17 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_NORTH
# 18 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_EXCLAM, SE_PIN
# 19 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Player_BeginExit, FALSE
# 20 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Samuel_ContinueEntrance
# 22 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	ssannedeparturecutcene 1
	return


# 25 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Samuel_BeginEntrance:
# 27 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 27 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 27 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
	step_end

# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Player_BeginExit:
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 33 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 34 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 38 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Samuel_ContinueEntrance:
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end
