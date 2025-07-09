PlayerHouse_MapScripts::
# 2 "data//maps/PlayerHouse/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PlayerHouse_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

PlayerHouse_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 4 "data//maps/PlayerHouse/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 0, EventScript_PlayerHouse_PlayerWakeUp
	.2byte 0


EventScript_PlayerHouse_Mom::
# 10 "data//maps/PlayerHouse/scripts.pory"
	mnmbox EventScript_PlayerHouse_Mom_Text_0
	return


EventScript_PlayerHouse_PlayerWakeUp::
# 15 "data//maps/PlayerHouse/scripts.pory"
	startcutscene PLAYER_HOUSE_WAKE_UP
# 16 "data//maps/PlayerHouse/scripts.pory"
	playerbox EventScript_PlayerHouse_PlayerWakeUp_Text_0, EMOTE_THINK
# 17 "data//maps/PlayerHouse/scripts.pory"
	setflag FLAG_UNUSED_0x020
# 18 "data//maps/PlayerHouse/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 1
# 19 "data//maps/PlayerHouse/scripts.pory"
	endcutscene
	return


EventScript_PlayerHouse_PlayerWakeUp_Skip::
# 24 "data//maps/PlayerHouse/scripts.pory"
	closemessage
# 25 "data//maps/PlayerHouse/scripts.pory"
	fadescreen FADE_TO_BLACK
# 26 "data//maps/PlayerHouse/scripts.pory"
	delay 30
# 27 "data//maps/PlayerHouse/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 1
# 28 "data//maps/PlayerHouse/scripts.pory"
	fadescreen FADE_FROM_BLACK
	return


EventScript_Trigger_PlayerHouse_MomRoom::
# 33 "data//maps/PlayerHouse/scripts.pory"
	playerbox EventScript_Trigger_PlayerHouse_MomRoom_Text_0, emoteMugshot = MUGSHOT_EMOTE_HAPPY
	return


EventScript_PlayerHouse_Mom_Text_0:
# 10 "data//maps/PlayerHouse/scripts.pory"
	.string "Hi, I'm Mum.$"

EventScript_PlayerHouse_PlayerWakeUp_Text_0:
# 16 "data//maps/PlayerHouse/scripts.pory"
	.string "Wow what a strange dream.\n"
	.string "This is a long bit of exposition\p"
	.string "10\p"
	.string "9\p"
	.string "8\p"
	.string "7\p"
	.string "6\p"
	.string "5\p"
	.string "4\p"
	.string "3\p"
	.string "2\p"
	.string "1\n"
	.string "It's done.$"

EventScript_Trigger_PlayerHouse_MomRoom_Text_0:
# 33 "data//maps/PlayerHouse/scripts.pory"
	.string "I shouldn't go into Mum's room.$"
