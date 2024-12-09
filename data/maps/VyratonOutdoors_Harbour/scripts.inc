VyratonOutdoors_Harbour_MapScripts::
# 3 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonOutdoors_Harbour_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 2, VyratonOutdoors_Harbour_EventScript_IkigaiIntro
	.2byte 0


VyratonOutdoors_Harbour_EventScript_IkigaiIntro::
# 14 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 3
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
	applymovement 9, VyratonOutdoors_Harbour_Movement_Captain_Exit
# 33 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay, FALSE
# 34 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance
# 35 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_NORTH
# 36 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	emote 2, EMOTE_QUESTION
# 37 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymoveplayer VyratonOutdoors_Harbour_Movement_Player_MeetSamuel, FALSE
# 38 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	applymove 2, VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance
# 39 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	mnmbox VyratonOutdoors_Harbour_Text_WelcomeToVyraton, nameKnown = FALSE, localId = 2
# 40 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnplayer DIR_SOUTH
# 41 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	turnobject 2, DIR_SOUTH
# 42 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	removeobject 9
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
	setflag FLAG_HIDE_VYRATON_HARBOR_CAPTAIN
# 55 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	waitstate
	end


# 59 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Captain_Exit:
# 61 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_2
# 62 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 63 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 66 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MoveOutOfWay:
# 68 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
# 69 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	lock_facing_direction
# 70 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 71 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	unlock_facing_direction
# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 72 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 73 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_left
# 74 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_up
	step_end

# 77 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_BeginEntrance:
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 79 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 80 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
	step_end

# 83 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_MeetSamuel:
# 85 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 85 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 86 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_8
# 88 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_left
	step_end

# 91 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_ContinueEntrance:
# 93 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 93 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_down
# 94 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	face_right
	step_end

# 97 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Samuel_Exit:
# 99 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
# 100 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_right
# 101 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end

# 104 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
VyratonOutdoors_Harbour_Movement_Player_Exit:
# 106 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 106 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	delay_16
# 107 "data//maps/VyratonOutdoors_Harbour/scripts.pory"
	walk_up
	step_end
