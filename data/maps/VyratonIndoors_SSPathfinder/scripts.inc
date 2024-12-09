VyratonIndoors_SSPathfinder_MapScripts::
	.byte 0


VyratonIndoors_SSPathfinder_IkigaiIntroSetup::
# 7 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 8 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setdynamicwarp MAP_VYRATON_OUTDOORS_HARBOUR, 15, 25
	return


VyratonIndoors_SSPathfinder_EventScript_MeetCaptain::
# 13 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_IkigaiIntroSetup
# 14 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_TEMP_0, 1
# 15 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	turnplayer DIR_NORTH
# 16 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	opendoor 4, 1
# 17 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playse SE_DOOR
# 18 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 19 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_NoExit, FALSE
# 20 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, Common_Movement_DoorEntrance
# 21 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	closedoor 4, 1
# 22 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 23 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_IntroduceCaptain, nameKnown = FALSE, localId = 1
# 24 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_LeadIn, FALSE
# 25 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn
# 26 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_WhatBringsYou, nameKnown = FALSE, waitbuttonpress = TRUE, remain = TRUE
# 27 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 28 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue
# 29 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 30 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice, waitbuttonpress = TRUE, remain = TRUE
# 31 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 32 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue
# 33 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 34 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_SocialMedia
	return


VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue::
# 39 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	dynmultichoice 0, 0, TRUE, 1, 0, DYN_MULTICHOICE_CB_NONE, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2
# 43 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 45 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2
# 46 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3
# 47 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_2:
# 45 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_FreshStart
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3:
# 46 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_NewChallenge
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4:
# 47 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_Business
	return


VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue::
# 53 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	dynmultichoice 0, 0, TRUE, 1, 0, DYN_MULTICHOICE_CB_NONE, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3
# 58 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 60 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2
# 61 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3
# 62 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4
# 63 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 3, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2:
# 60 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Inspired
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3:
# 61 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Humble
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4:
# 62 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Dominant
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5:
# 63 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Cynical
	return


# 67 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_NoExit:
# 69 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	lock_facing_direction
# 70 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 71 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	unlock_facing_direction
	step_end

# 74 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_LeadIn:
# 76 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 76 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 77 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_up
	step_end

# 80 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn:
# 82 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 83 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 84 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_2
# 85 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 86 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_left
# 87 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 87 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 88 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_right
# 89 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 89 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 90 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_down
	step_end

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0:
# 40 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I wanted a fresh start.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1:
# 41 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm moving for a new challenge.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_2:
# 42 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm going for business.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0:
# 54 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'll make the most of your advice!$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1:
# 55 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I need all the help I can get.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2:
# 56 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I don't care, I'll handle myself.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3:
# 57 "data//maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "Sure, like that means anything…$"
