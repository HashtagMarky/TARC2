Mindscape_MapScripts::
	.byte 0


Mindscape_EventScript_Celebi::
# 6 "data//maps/Mindscape/scripts.pory"
	mnmbox Mindscape_Text_Celebi, PMD = TRUE, mugshotEmotion = MUGSHOT_EMOTE_PAIN
# 7 "data//maps/Mindscape/scripts.pory"
	attitudemultipush ATTITUDE_HUMBLE, Mindscape_EventScript_Celebi_Text_0
# 8 "data//maps/Mindscape/scripts.pory"
	attitudemultipush ATTITUDE_DOMINANT, Mindscape_EventScript_Celebi_Text_1
# 9 "data//maps/Mindscape/scripts.pory"
	attitudechoice
# 10 "data//maps/Mindscape/scripts.pory"
	compare VAR_RESULT, ATTITUDE_DOMINANT
	goto_if_eq Mindscape_EventScript_Celebi_2
# 15 "data//maps/Mindscape/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 16 "data//maps/Mindscape/scripts.pory"
	warp MAP_PLAYER_HOUSE, 4, 3
# 17 "data//maps/Mindscape/scripts.pory"
	releaseall
	end

Mindscape_EventScript_Celebi_2:
# 12 "data//maps/Mindscape/scripts.pory"
	callnative StartCredits
	end


Mindscape_EventScript_Celebi_Text_0:
# 7 "data//maps/Mindscape/scripts.pory"
	.string "Of course I will help you.$"

Mindscape_EventScript_Celebi_Text_1:
# 8 "data//maps/Mindscape/scripts.pory"
	.string "No, I refuse.$"

Mindscape_Text_Celebi::
# 21 "data//maps/Mindscape/scripts.pory"
	.string "Urgh... Flavor text...\n"
	.string "I am Celebi.\p"
	.string "{SPEAKER_NAME NAME_AUTO}{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_DETERMINED}{SHOW_POKEDEX_HEADER} More flavour text.$"
