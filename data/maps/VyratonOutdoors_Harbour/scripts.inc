VyratonOutdoors_Harbour_MapScripts::
# 3 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 0, VyratonOutdoors_Harbour_EventScript_IkigaiIntro
	.2byte 0


VyratonOutdoors_Harbour_EventScript_IkigaiIntro::
# 14 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 1
# 15 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 16 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setselectedobjectevent 2
# 18 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet
# 19 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet::
# 24 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	playse SE_RG_SS_ANNE_HORN
# 25 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance
# 26 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_NORTH
# 27 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_EXCLAM
# 28 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MeetSamuel, FALSE
# 29 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance
# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE
# 31 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures::
# 37 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	ssannedeparturecutcene 1
# 38 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_WEST
# 39 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_HeadToOffice
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_Exit, FALSE
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_Exit
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	warp MAP_VYRATON_INDOORS_MAYORS_OFFICE, 4, 9
# 43 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	return


# 46 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance:
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
	step_end

# 51 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MeetSamuel:
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 54 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 56 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 59 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance:
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 65 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_Exit:
# 67 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 68 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 69 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_Exit:
# 74 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 74 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 75 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
