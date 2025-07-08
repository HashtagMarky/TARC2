PlayerHouse_MapScripts::
	.byte 0


EventScript_PlayerHouse_Mom::
# 5 "data//maps/PlayerHouse/scripts.pory"
	mnmbox EventScript_PlayerHouse_Mom_Text_0
	return


EventScript_Trigger_PlayerHouse_MomRoom::
# 10 "data//maps/PlayerHouse/scripts.pory"
	playerbox EventScript_Trigger_PlayerHouse_MomRoom_Text_0, emoteMugshot = MUGSHOT_EMOTE_HAPPY
	return


EventScript_PlayerHouse_Mom_Text_0:
# 5 "data//maps/PlayerHouse/scripts.pory"
	.string "Hi, I'm Mum.$"

EventScript_Trigger_PlayerHouse_MomRoom_Text_0:
# 10 "data//maps/PlayerHouse/scripts.pory"
	.string "I shouldn't go into Mum's room.$"
