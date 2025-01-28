IkigaiCharacterBox_InitialDialogue::
# 7 "data//scripts/ikigai_characters/scripts.pory"
	charemotedefault
# 8 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_InitialDialogue_Text_0, waitbuttonpress = FALSE, remain = TRUE
# 10 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_1, DIALOGUE_OPTION_TALK
# 11 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_2, DIALOGUE_OPTION_OPINION
# 12 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_3, DIALOGUE_OPTION_ROMANCE
# 13 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_4, DIALOGUE_OPTION_QUESTS
# 14 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_5, DIALOGUE_OPTION_BATTLE
# 15 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_6, DIALOGUE_OPTION_GIFT
# 16 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_7, DIALOGUE_OPTION_GOODBYE
# 17 "data//scripts/ikigai_characters/scripts.pory"
	dynmultistack 0, 0, FALSE, 2, TRUE, 0, DYN_MULTICHOICE_CB_DIALOGUE_OPTIONS
# 18 "data//scripts/ikigai_characters/scripts.pory"
	switch VAR_RESULT
# 20 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_TALK, IkigaiCharacterBox_InitialDialogue_2
# 22 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_OPINION, IkigaiCharacterBox_InitialDialogue_3
# 24 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_ROMANCE, IkigaiCharacterBox_InitialDialogue_4
# 26 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_QUESTS, IkigaiCharacterBox_InitialDialogue_5
# 27 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_BATTLE, IkigaiCharacterBox_InitialDialogue_5
# 28 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_GIFT, IkigaiCharacterBox_InitialDialogue_5
# 30 "data//scripts/ikigai_characters/scripts.pory"
	case DIALOGUE_OPTION_GOODBYE, IkigaiCharacterBox_InitialDialogue_6
IkigaiCharacterBox_InitialDialogue_6:
# 32 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_Goodbye
	return

IkigaiCharacterBox_InitialDialogue_2:
# 21 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_Talk
	return

IkigaiCharacterBox_InitialDialogue_3:
# 23 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_Debug_GetOpinion
	return

IkigaiCharacterBox_InitialDialogue_4:
# 25 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_RomanceMenu
	return

IkigaiCharacterBox_InitialDialogue_5:
# 29 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_Talk
	return


IkigaiCharacterBox_Goodbye::
# 38 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_Goodbye_Text_0
# 39 "data//scripts/ikigai_characters/scripts.pory"
	charemotedefault
# 40 "data//scripts/ikigai_characters/scripts.pory"
	release
	end


IkigaiCharacterBox_Talk::
# 46 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_Talk_Text_0, waitbuttonpress = FALSE, remain = TRUE
# 47 "data//scripts/ikigai_characters/scripts.pory"
	attitudechoice IkigaiCharacterBox_Talk_Text_1, IkigaiCharacterBox_Talk_Text_2, IkigaiCharacterBox_Talk_Text_3, IkigaiCharacterBox_Talk_Text_4, IkigaiCharacterBox_Talk_Text_5, bPress = FALSE
# 48 "data//scripts/ikigai_characters/scripts.pory"
	callnative IkigaiCharacter_HandleDialogue_Attitudes
# 49 "data//scripts/ikigai_characters/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 50 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq IkigaiCharacterBox_Talk_2
IkigaiCharacterBox_Talk_1:
# 54 "data//scripts/ikigai_characters/scripts.pory"
	charemoteresult
# 55 "data//scripts/ikigai_characters/scripts.pory"
	release
	return

IkigaiCharacterBox_Talk_2:
# 52 "data//scripts/ikigai_characters/scripts.pory"
	goto IkigaiCharacterBox_Goodbye
	goto IkigaiCharacterBox_Talk_1


IkigaiCharacterBox_Debug_GetOpinion::
# 60 "data//scripts/ikigai_characters/scripts.pory"
	callnative DEBUG_IkigaiCharacter_CharacterOpinions
# 61 "data//scripts/ikigai_characters/scripts.pory"
	msgbox IkigaiCharacterBox_Debug_GetOpinion_Text_0, MSGBOX_DEFAULT
# 62 "data//scripts/ikigai_characters/scripts.pory"
	closemessage
	end


IkigaiCharacterBox_RomanceMenu::
# 68 "data//scripts/ikigai_characters/scripts.pory"
	getromanticflag
# 69 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq IkigaiCharacterBox_RomanceMenu_2
# 75 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_1, SET_CLEAR_ROMANCE
IkigaiCharacterBox_RomanceMenu_1:
# 77 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_2, ROMANCE_EXCLUSIVE
# 78 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_3, CHECK_ROMANCE
# 79 "data//scripts/ikigai_characters/scripts.pory"
	dynmultistack 0, 0, TRUE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
# 81 "data//scripts/ikigai_characters/scripts.pory"
	switch VAR_RESULT
# 83 "data//scripts/ikigai_characters/scripts.pory"
	case SET_CLEAR_ROMANCE, IkigaiCharacterBox_RomanceMenu_6
# 95 "data//scripts/ikigai_characters/scripts.pory"
	case ROMANCE_EXCLUSIVE, IkigaiCharacterBox_RomanceMenu_7
# 99 "data//scripts/ikigai_characters/scripts.pory"
	case CHECK_ROMANCE, IkigaiCharacterBox_RomanceMenu_8
IkigaiCharacterBox_RomanceMenu_8:
# 101 "data//scripts/ikigai_characters/scripts.pory"
	checkrelationships
# 102 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, CHARACTER_DEFAULT
	goto_if_eq IkigaiCharacterBox_RomanceMenu_13
# 106 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, CHARACTER_COUNT_TOTAL
	goto_if_eq IkigaiCharacterBox_RomanceMenu_14
# 112 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, VAR_0x8004
	goto_if_eq IkigaiCharacterBox_RomanceMenu_18
# 118 "data//scripts/ikigai_characters/scripts.pory"
	buffercharactername
# 119 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_10
	return

IkigaiCharacterBox_RomanceMenu_2:
# 71 "data//scripts/ikigai_characters/scripts.pory"
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_0, SET_CLEAR_ROMANCE
	goto IkigaiCharacterBox_RomanceMenu_1

IkigaiCharacterBox_RomanceMenu_6:
# 84 "data//scripts/ikigai_characters/scripts.pory"
	toggleromanticflag
# 85 "data//scripts/ikigai_characters/scripts.pory"
	getromanticflag
# 86 "data//scripts/ikigai_characters/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq IkigaiCharacterBox_RomanceMenu_10
# 92 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_5
IkigaiCharacterBox_RomanceMenu_9:
	end

IkigaiCharacterBox_RomanceMenu_7:
# 96 "data//scripts/ikigai_characters/scripts.pory"
	setromanticflagexclusive
# 97 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_6
	end

IkigaiCharacterBox_RomanceMenu_10:
# 88 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_4
	goto IkigaiCharacterBox_RomanceMenu_9

IkigaiCharacterBox_RomanceMenu_13:
# 104 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_7
	return

IkigaiCharacterBox_RomanceMenu_14:
# 108 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_8
	return

IkigaiCharacterBox_RomanceMenu_18:
# 114 "data//scripts/ikigai_characters/scripts.pory"
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_9
	return


IkigaiCharacterBox_InitialDialogue_Text_0:
# 8 "data//scripts/ikigai_characters/scripts.pory"
	.string "Hello there, {NICKNAME}.$"

IkigaiCharacterBox_InitialDialogue_Text_1:
# 10 "data//scripts/ikigai_characters/scripts.pory"
	.string "Talk$"

IkigaiCharacterBox_InitialDialogue_Text_2:
# 11 "data//scripts/ikigai_characters/scripts.pory"
	.string "Opinion$"

IkigaiCharacterBox_InitialDialogue_Text_3:
# 12 "data//scripts/ikigai_characters/scripts.pory"
	.string "Romance$"

IkigaiCharacterBox_InitialDialogue_Text_4:
# 13 "data//scripts/ikigai_characters/scripts.pory"
	.string "Quests$"

IkigaiCharacterBox_InitialDialogue_Text_5:
# 14 "data//scripts/ikigai_characters/scripts.pory"
	.string "Battle$"

IkigaiCharacterBox_InitialDialogue_Text_6:
# 15 "data//scripts/ikigai_characters/scripts.pory"
	.string "Gift$"

IkigaiCharacterBox_InitialDialogue_Text_7:
# 16 "data//scripts/ikigai_characters/scripts.pory"
	.string "Goodbye$"

IkigaiCharacterBox_Goodbye_Text_0:
# 38 "data//scripts/ikigai_characters/scripts.pory"
	.string "Goodbye.$"

IkigaiCharacterBox_Talk_Text_0:
# 46 "data//scripts/ikigai_characters/scripts.pory"
	.string "What response will you\ngive to me?$"

IkigaiCharacterBox_Talk_Text_1:
# 47 "data//scripts/ikigai_characters/scripts.pory"
	.string "Inspiring$"

IkigaiCharacterBox_Talk_Text_2:
# 47 "data//scripts/ikigai_characters/scripts.pory"
	.string "Humble$"

IkigaiCharacterBox_Talk_Text_3:
# 47 "data//scripts/ikigai_characters/scripts.pory"
	.string "Dominant$"

IkigaiCharacterBox_Talk_Text_4:
# 47 "data//scripts/ikigai_characters/scripts.pory"
	.string "Cynical$"

IkigaiCharacterBox_Talk_Text_5:
# 47 "data//scripts/ikigai_characters/scripts.pory"
	.string "Neutral$"

IkigaiCharacterBox_Debug_GetOpinion_Text_0:
# 61 "data//scripts/ikigai_characters/scripts.pory"
	.string "{STR_VAR_1}{STR_VAR_2}$"

IkigaiCharacterBox_RomanceMenu_Text_0:
# 71 "data//scripts/ikigai_characters/scripts.pory"
	.string "End Romance$"

IkigaiCharacterBox_RomanceMenu_Text_1:
# 75 "data//scripts/ikigai_characters/scripts.pory"
	.string "Start Romance$"

IkigaiCharacterBox_RomanceMenu_Text_2:
# 77 "data//scripts/ikigai_characters/scripts.pory"
	.string "Set Exclusive Romance$"

IkigaiCharacterBox_RomanceMenu_Text_3:
# 78 "data//scripts/ikigai_characters/scripts.pory"
	.string "Check Romance$"

IkigaiCharacterBox_RomanceMenu_Text_4:
# 88 "data//scripts/ikigai_characters/scripts.pory"
	.string "You're so sweet, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_5:
# 92 "data//scripts/ikigai_characters/scripts.pory"
	.string "That's a shame, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_6:
# 97 "data//scripts/ikigai_characters/scripts.pory"
	.string "You're the sweetest, nobody compares\nto you {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_7:
# 104 "data//scripts/ikigai_characters/scripts.pory"
	.string "It seems you're single, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_8:
# 108 "data//scripts/ikigai_characters/scripts.pory"
	.string "It seems you're in multiple\nrelationships, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_9:
# 114 "data//scripts/ikigai_characters/scripts.pory"
	.string "We are in an exclusive relationship.$"

IkigaiCharacterBox_RomanceMenu_Text_10:
# 119 "data//scripts/ikigai_characters/scripts.pory"
	.string "You are in an exclusive relationship\nwith {STR_VAR_1}.$"
