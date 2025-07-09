Mindscape_MapScripts::
	.byte 0

Mindscape_EventScript_Celebi::
	mnmbox Mindscape_Text_Celebi
	warp MAP_PLAYER_HOUSE, 4, 3
	releaseall
	end

Mindscape_Text_Celebi::
	.string "Urgh... Flavor text...$"
