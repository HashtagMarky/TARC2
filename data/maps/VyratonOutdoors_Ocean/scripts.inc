VyratonOutdoors_Ocean_MapScripts::
# 3 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	map_script MAP_SCRIPT_ON_RESUME, VyratonOutdoors_Ocean_MapScripts_MAP_SCRIPT_ON_RESUME
# 8 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonOutdoors_Ocean_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonOutdoors_Ocean_MapScripts_MAP_SCRIPT_ON_RESUME:
# 5 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	setstepcallback STEP_CB_SS_PATHFINDER
	return

VyratonOutdoors_Ocean_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 10 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 0, VyratonOutdoors_Ocean_MapScripts_WarpIkigaiIntro
	.2byte 0


VyratonOutdoors_Ocean_MapScripts_WarpIkigaiIntro::
# 16 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 1
# 17 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	msgbox VyratonOutdoors_Ocean_Text_PleasePrepare, MSGBOX_DEFAULT
# 18 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	closemessage
# 19 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	playse SE_RG_SS_ANNE_HORN
# 20 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	waitse
# 21 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	warpsilent MAP_VYRATON_INDOORS_SSPATHFINDER, 5, 5
# 22 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	callnative DestroySSPathfinderSmokeTask
# 23 "data/maps/VyratonOutdoors_Ocean/scripts.pory"
	waitstate
	return

