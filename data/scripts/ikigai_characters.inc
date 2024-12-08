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
	dynmultipush IkigaiCharacterBox_InitialDialogue_Text_4, DIALOGUE_OPTION_GOODBYE
# 8 "data//scripts/ikigai_characters.pory"
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_NONE
# 9 "data//scripts/ikigai_characters.pory"
	switch VAR_RESULT
# 11 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_TALK, IkigaiCharacterBox_InitialDialogue_2
# 12 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_QUESTS, IkigaiCharacterBox_InitialDialogue_2
# 13 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_BATTLE, IkigaiCharacterBox_InitialDialogue_2
# 15 "data//scripts/ikigai_characters.pory"
	case DIALOGUE_OPTION_GOODBYE, IkigaiCharacterBox_InitialDialogue_3
IkigaiCharacterBox_InitialDialogue_3:
# 17 "data//scripts/ikigai_characters.pory"
	goto IkigaiCharacterBox_Goodbye
	return

IkigaiCharacterBox_InitialDialogue_2:
# 14 "data//scripts/ikigai_characters.pory"
	goto IkigaiCharacterBox_Talk
	return


IkigaiCharacterBox_Goodbye::
# 23 "data//scripts/ikigai_characters.pory"
	closemessage
# 24 "data//scripts/ikigai_characters.pory"
	releaseall
	end


IkigaiCharacterBox_Talk::
# 30 "data//scripts/ikigai_characters.pory"
	mnmbox IkigaiCharacterBox_Talk_Text_0
# 31 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_1, CHARACTERISTIC_NEUTRAL
# 32 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_2, CHARACTERISTIC_INSPIRED
# 33 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_3, CHARACTERISTIC_HUMBLE
# 34 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_4, CHARACTERISTIC_DOMINAT
# 35 "data//scripts/ikigai_characters.pory"
	dynmultipush IkigaiCharacterBox_Talk_Text_5, CHARACTERISTIC_CYNICAL
# 36 "data//scripts/ikigai_characters.pory"
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_DIALOGUE
# 37 "data//scripts/ikigai_characters.pory"
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
	.string "Goodbye$"

IkigaiCharacterBox_Talk_Text_0:
# 30 "data//scripts/ikigai_characters.pory"
	.string "What response will you\ngive to me?$"

IkigaiCharacterBox_Talk_Text_1:
# 31 "data//scripts/ikigai_characters.pory"
	.string "Neutral$"

IkigaiCharacterBox_Talk_Text_2:
# 32 "data//scripts/ikigai_characters.pory"
	.string "Inspiring$"

IkigaiCharacterBox_Talk_Text_3:
# 33 "data//scripts/ikigai_characters.pory"
	.string "Humble$"

IkigaiCharacterBox_Talk_Text_4:
# 34 "data//scripts/ikigai_characters.pory"
	.string "Dominating$"

IkigaiCharacterBox_Talk_Text_5:
# 35 "data//scripts/ikigai_characters.pory"
	.string "Cynical$"
