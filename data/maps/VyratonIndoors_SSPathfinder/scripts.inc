VyratonIndoors_SSPathfinder_MapScripts::
	.byte 0


VyratonIndoors_SSPathfinder_IkigaiIntroSetup::
# 7 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 8 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setflag FLAG_DONT_TRANSITION_MUSIC
# 9 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setdynamicwarp MAP_VYRATON_OUTDOORS_HARBOUR, 15, 25
	return


VyratonIndoors_SSPathfinder_EventScript_MeetCaptain::
# 14 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_IkigaiIntroSetup
# 15 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	setvar VAR_TEMP_0, 1
# 16 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playerface DIR_NORTH
# 17 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	opendoor 4, 1
# 18 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playse SE_DOOR
# 19 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 20 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_NoExit, FALSE
# 21 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, Common_Movement_DoorEntrance
# 22 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION, wait = FALSE
# 23 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	closedoor 4, 1
# 24 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 25 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_IntroduceCaptain, nameKnown = FALSE, localId = 1
# 26 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymoveplayer VyratonIndoors_SSPathfinder_Movement_Player_LeadIn, FALSE
# 27 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn
# 28 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_WhatBringsYou, nameKnown = FALSE, waitbuttonpress = FALSE, remain = TRUE
# 29 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 30 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue
# 31 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 32 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice, waitbuttonpress = FALSE, remain = TRUE
# 33 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_QUESTION
# 34 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	call VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue
# 35 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 36 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_SocialMedia
# 37 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playse SE_RG_SS_ANNE_HORN
# 38 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote 1, EMOTE_EXCLAM, FALSE, FALSE
# 39 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_EXCLAM, FALSE, FALSE
# 40 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitse
# 41 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	mnmbox VyratonIndoors_SSPathfinder_Text_SoundOfDocking
# 42 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, Common_Movement_WalkUp2
# 43 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	opendoor 5, 1
# 44 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	playse SE_DOOR
# 45 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 46 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	applymove 1, Common_Movement_DoorExit
# 47 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	closedoor 5, 1
# 48 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	waitdooranim
# 49 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	removeobject 1
# 50 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	releaseall
	end


VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue::
# 56 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	attitudemultipush ATTITUDE_NEUTRAL, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1
# 59 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	attitudechoice
# 60 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 62 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 0, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3
# 63 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4
# 64 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case 2, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_5
VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_1:
# 66 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_WhateverItMayBe
	return

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_3:
# 62 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_FreshStart
	goto VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_1

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_4:
# 63 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_NewChallenge
	goto VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_1

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_5:
# 64 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_WhatBringsYou_Business
	goto VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_1


VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue::
# 71 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	attitudechoice VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3
# 76 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	switch VAR_RESULT
# 78 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case ATTITUDE_INSPIRED, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2
# 79 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case ATTITUDE_HUMBLE, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3
# 80 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case ATTITUDE_DOMINANT, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4
# 81 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	case ATTITUDE_CYNICAL, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_2:
# 78 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Inspired
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_3:
# 79 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Humble
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_4:
# 80 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Dominant
	return

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_5:
# 81 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	bufferstring STR_VAR_1, VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_Cynical
	return


# 85 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_NoExit:
# 87 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	lock_facing_direction
# 88 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 89 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	unlock_facing_direction
	step_end

# 92 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Player_LeadIn:
# 94 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 94 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 95 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_up
	step_end

# 98 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
VyratonIndoors_SSPathfinder_Movement_Captain_FollowIn:
# 100 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 101 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	walk_down
# 102 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_2
# 103 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 104 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_left
# 105 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 105 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 106 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_right
# 107 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 107 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	delay_16
# 108 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	face_down
	step_end

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_0:
# 57 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I wanted a fresh start.$"

VyratonIndoors_SSPathfinder_Text_WhatBringsYou_PlayerDialogue_Text_1:
# 58 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'm moving for a new challenge.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_0:
# 72 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I'll make the most of your advice!$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_1:
# 73 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I need all the help I can get.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_2:
# 74 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "I don't care, I'll handle myself.$"

VyratonIndoors_SSPathfinder_Text_GranddaughtersAdvice_PlayerDialgue_Text_3:
# 75 "data/maps/VyratonIndoors_SSPathfinder/scripts.pory"
	.string "Sure, like that means anything…$"
