Mindscape_MapScripts::
	.byte 0


Mindscape_EventScript_Celebi::
# 5 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_Text_Celebi, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_PAIN, nameKnown = FALSE, remain = TRUE, waitbuttonpress = FALSE
Mindscape_EventScript_Celebi_2:
# 8 "data//maps/Mindscape/scripts.pory"
	attitudemultipush ATTITUDE_HUMBLE, Mindscape_EventScript_Celebi_Text_0
# 9 "data//maps/Mindscape/scripts.pory"
	attitudemultipush ATTITUDE_DOMINANT, Mindscape_EventScript_Celebi_Text_1
# 10 "data//maps/Mindscape/scripts.pory"
	attitudechoice
# 11 "data//maps/Mindscape/scripts.pory"
	closemessage
# 12 "data//maps/Mindscape/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_TALK
# 13 "data//maps/Mindscape/scripts.pory"
	addvar VAR_TEMP_0, 1
# 15 "data//maps/Mindscape/scripts.pory"
	compare VAR_RESULT, ATTITUDE_HUMBLE
	goto_if_eq Mindscape_EventScript_Celebi_5
# 20 "data//maps/Mindscape/scripts.pory"
	compare VAR_TEMP_0, 1
	goto_if_eq Mindscape_EventScript_Celebi_8
Mindscape_EventScript_Celebi_7:
# 27 "data//maps/Mindscape/scripts.pory"
	compare VAR_TEMP_0, 2
	goto_if_eq Mindscape_EventScript_Celebi_11
Mindscape_EventScript_Celebi_10:
# 34 "data//maps/Mindscape/scripts.pory"
	compare VAR_TEMP_0, 3
	goto_if_eq Mindscape_EventScript_Celebi_13
	goto Mindscape_EventScript_Celebi_2

Mindscape_EventScript_Celebi_1:
# 44 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_EventScript_Celebi_Text_5, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_JOYOUS
# 47 "data//maps/Mindscape/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 48 "data//maps/Mindscape/scripts.pory"
	warp MAP_PLAYER_HOUSE, 4, 3
# 49 "data//maps/Mindscape/scripts.pory"
	releaseall
	end

Mindscape_EventScript_Celebi_5:
	goto Mindscape_EventScript_Celebi_1

Mindscape_EventScript_Celebi_8:
# 22 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_EventScript_Celebi_Text_2, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_ANGRY, remain = TRUE, waitbuttonpress = FALSE
	goto Mindscape_EventScript_Celebi_7

Mindscape_EventScript_Celebi_11:
# 29 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_EventScript_Celebi_Text_3, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_ANGRY, remain = TRUE, waitbuttonpress = FALSE
	goto Mindscape_EventScript_Celebi_10

Mindscape_EventScript_Celebi_13:
# 36 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_EventScript_Celebi_Text_4, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_ANGRY, remain = TRUE
# 39 "data//maps/Mindscape/scripts.pory"
	fadescreenswapbuffers FADE_TO_BLACK
# 40 "data//maps/Mindscape/scripts.pory"
	callnative StartCredits
	end


Mindscape_EventScript_Celebi_Text_0:
# 8 "data//maps/Mindscape/scripts.pory"
	.string "Of course I will help you.$"

Mindscape_EventScript_Celebi_Text_1:
# 9 "data//maps/Mindscape/scripts.pory"
	.string "No, I refuse.$"

Mindscape_EventScript_Celebi_Text_2:
# 23 "data//maps/Mindscape/scripts.pory"
	.string "Please… Reconsider, {PLAYER}.\n"
	.string "Will you help me?$"

Mindscape_EventScript_Celebi_Text_3:
# 30 "data//maps/Mindscape/scripts.pory"
	.string "Are you sure, {PLAYER}?\n"
	.string "The moment is fleeting…$"

Mindscape_EventScript_Celebi_Text_4:
# 37 "data//maps/Mindscape/scripts.pory"
	.string "Very well, {PLAYER}…\n"
	.string "I must have been wrong…$"

Mindscape_EventScript_Celebi_Text_5:
# 45 "data//maps/Mindscape/scripts.pory"
	.string "I knew you would, {PLAYER}.\n"
	.string "But I am overjoyed to hear it.\p"
	.string "You must remain vigilant.\n"
	.string "I'll not be able to help you.$"

Mindscape_Text_Celebi::
# 53 "data//maps/Mindscape/scripts.pory"
	.string "Hghh…\n"
	.string "So far… I've reached so far…\l"
	.string "Yet the thread holds…\p"
	.string "You can hear me?\n"
	.string "Then the veil has parted…\l"
	.string "And you've stepped through.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_SIGH}You see me now, not in thoughts,\n"
	.string "but in presence. In stillness.\l"
	.string "Even, if not in physicality.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_HAPPY}I'm glad you can meet me now…\n"
	.string "See me, presented before you…\l"
	.string "Across silence, across time.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_NORMAL}{SHOW_POKEDEX_HEADER}{SPEAKER_NAME NAME_AUTO}I am Celebi. I am the guardian\n"
	.string "of what was, and what may be.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_WORRIED}{HIDE_POKEDEX_HEADER}This is the Astral Mindscape.\n"
	.string "Where time curls inwards.\l"
	.string "Where truth remembers itself.\p"
	.string "You may have walked this path before.\n"
	.string "You will walk it again…\l"
	.string "But not the same way.\p"
	.string "Each step leaves a mark,\n"
	.string "Even if it's forgotten.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_DETERMINED}That is why I chose you.\n"
	.string "You remember, even when you forget.\p"
	.string "Regression is not a prison.\n"
	.string "Regression is a chance.\l"
	.string "It is a seed. And you are its soil.\p"
	.string "I am with you now.\n"
	.string "And I will be with you again.\p"
	.string "{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_ANGRY}Will you help me?\n"
	.string "Before the moment slips away?$"
