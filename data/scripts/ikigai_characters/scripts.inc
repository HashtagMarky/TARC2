IkigaiCharacterBox_InitialDialogue::
	charemotedefault
	mnmbox IkigaiCharacterBox_InitialDialogue_Text_0, waitbuttonpress = FALSE, remain = TRUE
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_1, DIALOGUE_OPTION_TALK
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_2, DIALOGUE_OPTION_OPINION
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_3, DIALOGUE_OPTION_ROMANCE
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_4, DIALOGUE_OPTION_QUESTS
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_5, DIALOGUE_OPTION_BATTLE
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_6, DIALOGUE_OPTION_GIFT
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_7, DIALOGUE_OPTION_GOODBYE
	dynmultistack 0, 0, FALSE, 2, TRUE, 0, DYN_MULTICHOICE_CB_DIALOGUE_OPTIONS
	switch VAR_RESULT
	case DIALOGUE_OPTION_TALK, IkigaiCharacterBox_InitialDialogue_2
	case DIALOGUE_OPTION_OPINION, IkigaiCharacterBox_InitialDialogue_3
	case DIALOGUE_OPTION_ROMANCE, IkigaiCharacterBox_InitialDialogue_4
	case DIALOGUE_OPTION_QUESTS, IkigaiCharacterBox_InitialDialogue_5
	case DIALOGUE_OPTION_BATTLE, IkigaiCharacterBox_InitialDialogue_5
	case DIALOGUE_OPTION_GIFT, IkigaiCharacterBox_InitialDialogue_5
	case DIALOGUE_OPTION_GOODBYE, IkigaiCharacterBox_InitialDialogue_6
IkigaiCharacterBox_InitialDialogue_6:
	goto IkigaiCharacterBox_Goodbye
	return

IkigaiCharacterBox_InitialDialogue_2:
	goto IkigaiCharacterBox_Talk
	return

IkigaiCharacterBox_InitialDialogue_3:
	goto IkigaiCharacterBox_Debug_GetOpinion
	return

IkigaiCharacterBox_InitialDialogue_4:
	goto IkigaiCharacterBox_RomanceMenu
	return

IkigaiCharacterBox_InitialDialogue_5:
	goto IkigaiCharacterBox_Talk
	return


IkigaiCharacterBox_Goodbye::
	mnmbox IkigaiCharacterBox_Goodbye_Text_0
	charemotedefault
	release
	end


IkigaiCharacterBox_Talk::
	mnmbox IkigaiCharacterBox_Talk_Text_0, waitbuttonpress = FALSE, remain = TRUE
	attitudechoice IkigaiCharacterBox_Talk_Text_1, IkigaiCharacterBox_Talk_Text_2, IkigaiCharacterBox_Talk_Text_3, IkigaiCharacterBox_Talk_Text_4, IkigaiCharacterBox_Talk_Text_5, bPress = FALSE
	callnative IkigaiCharacter_HandleDialogue_Attitudes
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq IkigaiCharacterBox_Talk_2
IkigaiCharacterBox_Talk_1:
	charemoteresult
	release
	return

IkigaiCharacterBox_Talk_2:
	goto IkigaiCharacterBox_Goodbye
	goto IkigaiCharacterBox_Talk_1


IkigaiCharacterBox_Debug_GetOpinion::
	callnative DEBUG_IkigaiCharacter_CharacterOpinions
	msgbox IkigaiCharacterBox_Debug_GetOpinion_Text_0, MSGBOX_DEFAULT
	closemessage
	end


IkigaiCharacterBox_RomanceMenu::
	getromanticflag
	compare VAR_RESULT, TRUE
	goto_if_eq IkigaiCharacterBox_RomanceMenu_2
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_1, SET_CLEAR_ROMANCE
IkigaiCharacterBox_RomanceMenu_1:
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_2, ROMANCE_EXCLUSIVE
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_3, CHECK_ROMANCE
	dynmultistack 0, 0, TRUE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case SET_CLEAR_ROMANCE, IkigaiCharacterBox_RomanceMenu_6
	case ROMANCE_EXCLUSIVE, IkigaiCharacterBox_RomanceMenu_7
	case CHECK_ROMANCE, IkigaiCharacterBox_RomanceMenu_8
IkigaiCharacterBox_RomanceMenu_8:
	checkrelationships
	compare VAR_RESULT, CHARACTER_DEFAULT
	goto_if_eq IkigaiCharacterBox_RomanceMenu_13
	compare VAR_RESULT, CHARACTER_COUNT_TOTAL
	goto_if_eq IkigaiCharacterBox_RomanceMenu_14
	compare VAR_RESULT, VAR_0x8004
	goto_if_eq IkigaiCharacterBox_RomanceMenu_18
	buffercharactername
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_10
	return

IkigaiCharacterBox_RomanceMenu_2:
	dynmultipush IkigaiCharacterBox_RomanceMenu_Text_0, SET_CLEAR_ROMANCE
	goto IkigaiCharacterBox_RomanceMenu_1

IkigaiCharacterBox_RomanceMenu_6:
	toggleromanticflag
	getromanticflag
	compare VAR_RESULT, TRUE
	goto_if_eq IkigaiCharacterBox_RomanceMenu_10
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_5
IkigaiCharacterBox_RomanceMenu_9:
	end

IkigaiCharacterBox_RomanceMenu_7:
	setromanticflagexclusive
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_6
	end

IkigaiCharacterBox_RomanceMenu_10:
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_4
	goto IkigaiCharacterBox_RomanceMenu_9

IkigaiCharacterBox_RomanceMenu_13:
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_7
	return

IkigaiCharacterBox_RomanceMenu_14:
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_8
	return

IkigaiCharacterBox_RomanceMenu_18:
	mnmbox IkigaiCharacterBox_RomanceMenu_Text_9
	return


IkigaiCharacterBox_InitialDialogue_Text_0:
	.string "Hello there, {NICKNAME}.$"

IkigaiCharacterBox_InitialDialogue_Text_1:
	.string "Talk$"

IkigaiCharacterBox_InitialDialogue_Text_2:
	.string "Opinion$"

IkigaiCharacterBox_InitialDialogue_Text_3:
	.string "Romance$"

IkigaiCharacterBox_InitialDialogue_Text_4:
	.string "Quests$"

IkigaiCharacterBox_InitialDialogue_Text_5:
	.string "Battle$"

IkigaiCharacterBox_InitialDialogue_Text_6:
	.string "Gift$"

IkigaiCharacterBox_InitialDialogue_Text_7:
	.string "Goodbye$"

IkigaiCharacterBox_Goodbye_Text_0:
	.string "Goodbye.$"

IkigaiCharacterBox_Talk_Text_0:
	.string "What response will you\ngive to me?$"

IkigaiCharacterBox_Talk_Text_1:
	.string "Inspiring$"

IkigaiCharacterBox_Talk_Text_2:
	.string "Humble$"

IkigaiCharacterBox_Talk_Text_3:
	.string "Dominant$"

IkigaiCharacterBox_Talk_Text_4:
	.string "Cynical$"

IkigaiCharacterBox_Talk_Text_5:
	.string "Neutral$"

IkigaiCharacterBox_Debug_GetOpinion_Text_0:
	.string "{STR_VAR_1}{STR_VAR_2}$"

IkigaiCharacterBox_RomanceMenu_Text_0:
	.string "End Romance$"

IkigaiCharacterBox_RomanceMenu_Text_1:
	.string "Start Romance$"

IkigaiCharacterBox_RomanceMenu_Text_2:
	.string "Set Exclusive Romance$"

IkigaiCharacterBox_RomanceMenu_Text_3:
	.string "Check Romance$"

IkigaiCharacterBox_RomanceMenu_Text_4:
	.string "You're so sweet, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_5:
	.string "That's a shame, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_6:
	.string "You're the sweetest, nobody compares\nto you {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_7:
	.string "It seems you're single, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_8:
	.string "It seems you're in multiple\nrelationships, {NICKNAME}.$"

IkigaiCharacterBox_RomanceMenu_Text_9:
	.string "We are in an exclusive relationship.$"

IkigaiCharacterBox_RomanceMenu_Text_10:
	.string "You are in an exclusive relationship\nwith {STR_VAR_1}.$"
