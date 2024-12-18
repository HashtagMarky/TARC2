IkigaiCharacterBox_InitialDialogue::
# 3 "data//scripts/ikigai_characters.pory"
	mnmbox IkigaiCharacterBox_InitialDialogue_Text_0
# 4 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_1, DIALOGUE_OPTION_TALK
# 5 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_2, DIALOGUE_OPTION_QUESTS
# 6 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_3, DIALOGUE_OPTION_BATTLE
# 7 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_4, DIALOGUE_OPTION_GIFT
# 8 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_5, DIALOGUE_OPTION_GOODBYE
# 9 "data//scripts/ikigai_characters.pory"
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_NONE
# 10 "data//scripts/ikigai_characters.pory"
	switch VAR_RESULT
# 12 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_TALK, IkigaiCharacterBox_InitialDialogue_2
# 13 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_QUESTS, IkigaiCharacterBox_InitialDialogue_2
# 14 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_BATTLE, IkigaiCharacterBox_InitialDialogue_2
# 15 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_GIFT, IkigaiCharacterBox_InitialDialogue_2
# 17 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_GOODBYE, IkigaiCharacterBox_InitialDialogue_3
IkigaiCharacterBox_InitialDialogue_3:
# 19 "data//scripts/ikigai_characters.pory"
	goto IkigaiCharacterBox_Goodbye
	return

IkigaiCharacterBox_InitialDialogue_2:
# 16 "data//scripts/ikigai_characters.pory"
	goto IkigaiCharacterBox_Talk
	return


IkigaiCharacterBox_Goodbye::
# 25 "data//scripts/ikigai_characters.pory"
	closemessage
# 26 "data//scripts/ikigai_characters.pory"
	releaseall
	end


IkigaiCharacterBox_Talk::
# 32 "data//scripts/ikigai_characters.pory"
	mnmbox IkigaiCharacterBox_Talk_Text_0
# 33 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_1, ATTITUDE_NEUTRAL
# 34 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_2, ATTITUDE_INSPIRED
# 35 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_3, ATTITUDE_HUMBLE
# 36 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_4, ATTITUDE_DOMINANT
# 37 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_5, ATTITUDE_CYNICAL
# 38 "data//scripts/ikigai_characters.pory"
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_DIALOGUE
# 39 "data//scripts/ikigai_characters.pory"
	callnative IkigaiCharacter_HandleDialogue
	return


IkigaiCharacterBox_InitialDialogue_Text_0:
# 3 "data//scripts/ikigai_characters.pory"
	.string "Hello there, {PLAYER}.$"

IkigaiCharacterBox_InitialDialogue_Text_1:
# 4 "data//scripts/ikigai_characters.pory"
	.string "Talk$"

IkigaiCharacterBox_InitialDialogue_Text_2:
# 5 "data//scripts/ikigai_characters.pory"
	.string "Quests$"

IkigaiCharacterBox_InitialDialogue_Text_3:
# 6 "data//scripts/ikigai_characters.pory"
	.string "Battle$"

IkigaiCharacterBox_InitialDialogue_Text_4:
# 7 "data//scripts/ikigai_characters.pory"
	.string "Gift$"

IkigaiCharacterBox_InitialDialogue_Text_5:
# 8 "data//scripts/ikigai_characters.pory"
	.string "Goodbye$"

IkigaiCharacterBox_Talk_Text_0:
# 32 "data//scripts/ikigai_characters.pory"
	.string "What response will you\ngive to me?$"

IkigaiCharacterBox_Talk_Text_1:
# 33 "data//scripts/ikigai_characters.pory"
	.string "Neutral$"

IkigaiCharacterBox_Talk_Text_2:
# 34 "data//scripts/ikigai_characters.pory"
	.string "Inspiring$"

IkigaiCharacterBox_Talk_Text_3:
# 35 "data//scripts/ikigai_characters.pory"
	.string "Humble$"

IkigaiCharacterBox_Talk_Text_4:
# 36 "data//scripts/ikigai_characters.pory"
	.string "Dominating$"

IkigaiCharacterBox_Talk_Text_5:
# 37 "data//scripts/ikigai_characters.pory"
	.string "Cynical$"
