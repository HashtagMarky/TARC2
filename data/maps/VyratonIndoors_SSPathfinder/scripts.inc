VyratonIndoors_SSPathfinder_MapScripts::
# 3 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_SSPathfinder_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_SSPathfinder_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 1, VyratonIndoors_SSPathfinder_IkigaiIntroSetup
	.2byte 0


VyratonIndoors_SSPathfinder_IkigaiIntroSetup::
# 13 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 14 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setdynamicwarp MAP_VYRATON_OUTDOORS_HARBOUR, 15, 25
	return


VyratonIndoors_SSPathfinder_EventScript_MeetCaptain::
# 19 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_TEMP_0, 1
# 20 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	turnplayer DIR_NORTH
# 21 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	opendoor 4, 1
# 22 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playse SE_DOOR
# 23 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 24 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_NoExit, FALSE
# 25 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, Common_Movement_DoorEntrance
# 26 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	closedoor 4, 1
# 27 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 28 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_IntroduceCaptain, nameKnown = FALSE, localId = 1
# 29 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_LeadIn, FALSE
# 30 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn
# 31 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_WhatBringsYou, nameKnown = FALSE, remain = TRUE
# 32 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue
# 33 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice
	return


VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue::
# 38 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	dynmultichoice 0, 0, TRUE, 1, 0, DYN_MULTICHOICE_CB_NONE, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2
# 39 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 41 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2
# 42 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3
# 43 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2:
# 41 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_FreshStart
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3:
# 42 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_NewChallenge
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4:
# 43 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_Business
	return


# 47 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_NoExit:
# 49 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	lock_facing_direction
# 50 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 51 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	unlock_facing_direction
	step_end

# 54 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_LeadIn:
# 56 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 56 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 57 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_up
	step_end

# 60 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn:
# 62 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 63 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 64 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_2
# 65 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 66 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_left
# 67 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 67 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 68 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_right
# 69 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 69 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 70 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_down
	step_end

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0:
# 38 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I wanted a fresh start.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1:
# 38 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm moving for a new challenge.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2:
# 38 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm going for business.$"
