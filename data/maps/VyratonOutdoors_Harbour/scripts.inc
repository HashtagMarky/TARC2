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
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Setup
# 15 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet
# 16 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Setup::
# 21 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
# 22 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 23 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setselectedobjectevent 2
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet::
# 28 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	addvar VAR_IKIGAI_INTRO_STATE, 1
# 29 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	playse SE_RG_SS_ANNE_HORN
# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Samuel_BeginEntrance
# 31 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_NORTH
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_EXCLAM, SE_PIN
# 33 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Player_MeetSamuel, FALSE
# 34 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Samuel_ContinueEntrance
# 35 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE
# 36 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 37 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures::
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	ssannedeparturecutcene 1
# 43 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_WEST
# 44 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_HeadToOffice
# 45 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Player_Exit, FALSE
# 46 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Samuel_Exit
# 47 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	warp MAP_VYRATON_INDOORS_MAYORS_OFFICE, 4, 9
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	return


# 51 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Samuel_BeginEntrance:
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
	step_end

# 56 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Player_MeetSamuel:
# 58 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 58 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 59 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 64 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Samuel_ContinueEntrance:
# 66 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 66 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 67 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Samuel_Exit:
# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 73 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 74 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 77 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Player_Exit:
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 80 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
