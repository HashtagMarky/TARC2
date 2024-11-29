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
	speakername known = FALSE
# 29 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	msgbox VyratonOutdoors_Harbour_Text_ExcuseMe, MSGBOX_DEFAULT
# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	closemessage
# 31 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	addobject 9
# 32 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymovement 9, VyratonOutdoors_Harbour_Movement_Grandparent_Exit
# 33 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay, FALSE
# 34 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance
# 35 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_NORTH
# 36 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_QUESTION
# 37 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay 40
# 38 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MeetSamuel, FALSE
# 39 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE, localId = 2
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures::
# 47 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_WEST
# 49 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_HeadToOffice
# 50 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_Exit, FALSE
# 51 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_Exit
# 52 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	warp MAP_VYRATON_INDOORS_MAYORS_OFFICE, 4, 9
# 53 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_VYRATON_HARBOR_SAMUEL
# 54 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_VYRATON_HARBOR_GRANDPARENT
# 55 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	return


# 58 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Grandparent_Exit:
# 60 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_2
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 65 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay:
# 67 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
# 68 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	lock_facing_direction
# 69 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	unlock_facing_direction
# 71 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 71 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_left
# 73 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_up
	step_end

# 76 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance:
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
	step_end

# 82 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MeetSamuel:
# 84 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 84 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 85 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 87 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 90 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance:
# 92 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 92 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 93 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 96 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_Exit:
# 98 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 99 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 100 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 103 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_Exit:
# 105 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 105 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 106 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
