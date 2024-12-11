VyratonOutdoors_Harbour_MapScripts::
# 3 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_TRANSITION
# 10 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_TRANSITION:
# 5 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	compare VAR_IKIGAI_INTRO_STATE, 2
	goto_if_eq VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
# 7 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 12 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 2, VyratonOutdoors_Harbour_EventScript_IkigaiIntro
	.2byte 0


VyratonOutdoors_Harbour_EventScript_IkigaiIntro::
# 21 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 3
# 23 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Disembarkment
# 24 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet
# 25 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Disembarkment::
# 30 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Meet::
# 35 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	speakername known = FALSE
# 36 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	msgbox VyratonOutdoors_Harbour_Text_ExcuseMe, MSGBOX_DEFAULT
# 37 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	closemessage
# 38 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	addobject 9
# 39 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymovement 9, VyratonOutdoors_Harbour_Movement_Captain_Exit
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay, FALSE
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_NORTH
# 43 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_QUESTION
# 44 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MeetSamuel, FALSE
# 45 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance
# 46 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE, localId = 2
# 47 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 48 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
# 49 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	removeobject 9
	return


VyratonOutdoors_Harbour_EventScript_IkigaiIntro_Departures::
# 54 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure
# 55 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_WEST
# 56 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_HeadToOffice
# 57 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_Exit, FALSE
# 58 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_Exit
# 59 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	warp MAP_VYRATON_INDOORS_MAYORS_OFFICE, 4, 9
# 60 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_VYRATON_HARBOR_SAMUEL
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setflag FLAG_HIDE_VYRATON_HARBOR_CAPTAIN
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	end


# 66 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Captain_Exit:
# 68 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_2
# 69 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 73 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay:
# 75 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
# 76 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	lock_facing_direction
# 77 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 78 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	unlock_facing_direction
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 80 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_left
# 81 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_up
	step_end

# 84 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance:
# 86 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 86 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 86 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 86 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 87 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
	step_end

# 90 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MeetSamuel:
# 92 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 92 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 93 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 95 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 98 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance:
# 100 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 100 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 101 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 104 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_Exit:
# 106 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 107 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 108 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 111 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_Exit:
# 113 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 113 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 114 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
