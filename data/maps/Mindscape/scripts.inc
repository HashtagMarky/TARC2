Mindscape_MapScripts::
	.byte 0

Mindscape_EventScript_Celebi::
	@ setseenmon SPECIES_CELEBI
	mnmbox Mindscape_Text_Celebi, PMD=TRUE, mugshotEmotion=MUGSHOT_EMOTE_PAIN
	clearflag FLAG_DONT_TRANSITION_MUSIC
	warp MAP_PLAYER_HOUSE, 4, 3
	releaseall
	end

Mindscape_Text_Celebi::
	.string "Urgh... Flavor text...\n"
	.string "I am Celebi.\p"
	.string "{SPEAKER_NAME NAME_AUTO}{CHANGE_MUGSHOT_EMOTE MUGSHOT_EMOTE_DETERMINED}{SHOW_POKEDEX_HEADER}"
	.string "More flavour text.$"
