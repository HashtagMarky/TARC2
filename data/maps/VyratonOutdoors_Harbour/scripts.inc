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
# 16 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Disembarkment
# 17 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet
# 18 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Disembarkment::
# 23 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet::
# 28 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance
# 29 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_NORTH
# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_EXCLAM
# 31 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MeetSamuel, FALSE
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance
# 33 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE, localId = 2
# 34 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 35 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures::
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_WEST
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_HeadToOffice
# 43 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_Exit, FALSE
# 44 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_Exit
# 45 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	warp MAP_VYRATON_INDOORS_MAYORS_OFFICE, 4, 9
# 46 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_VYRATON_HARBOR_SAMUEL
# 47 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	return


# 50 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance:
# 52 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 52 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 52 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 52 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
	step_end

# 55 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MeetSamuel:
# 57 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 57 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 58 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 60 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance:
# 65 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 65 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 66 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 69 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_Exit:
# 71 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 73 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 76 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_Exit:
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
