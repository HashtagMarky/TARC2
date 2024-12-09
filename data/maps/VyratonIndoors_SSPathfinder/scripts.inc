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
	mnmbox VyratonIndoors_SSPathfinder_Text_WhatBringsYou, nameKnown = FALSE, waitbuttonpress = TRUE, remain = TRUE
# 32 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 33 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue
# 34 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 35 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice, waitbuttonpress = TRUE, remain = TRUE
# 36 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 37 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue
# 38 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 39 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_SocialMedia
	return


VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue::
# 44 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	dynmultichoice 0, 0, TRUE, 1, 0, DYN_MULTICHOICE_CB_NONE, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2
# 48 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 50 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2
# 51 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3
# 52 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2:
# 50 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_FreshStart
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3:
# 51 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_NewChallenge
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4:
# 52 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_Business
	return


VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue::
# 58 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	dynmultichoice 0, 0, TRUE, 1, 0, DYN_MULTICHOICE_CB_NONE, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3
# 63 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 65 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2
# 66 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3
# 67 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4
# 68 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 3, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2:
# 65 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Inspired
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3:
# 66 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Humble
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4:
# 67 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Dominant
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5:
# 68 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Cynical
	return


# 72 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_NoExit:
# 74 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	lock_facing_direction
# 75 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 76 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	unlock_facing_direction
	step_end

# 79 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_LeadIn:
# 81 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 81 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 82 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_up
	step_end

# 85 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn:
# 87 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 88 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 89 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_2
# 90 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 91 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_left
# 92 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 92 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 93 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_right
# 94 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 94 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 95 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_down
	step_end

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0:
# 45 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I wanted a fresh start.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1:
# 46 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm moving for a new challenge.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2:
# 47 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm going for business.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0:
# 59 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'll make the most of your advice!$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1:
# 60 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I need all the help I can get.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2:
# 61 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I don't care, I'll handle myself.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3:
# 62 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "Sure, like that means anything…$"
