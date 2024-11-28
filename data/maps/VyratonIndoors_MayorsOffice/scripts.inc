VyratonIndoors_MayorsOffice_MapScripts::
# 3 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 1, VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro
	.2byte 0


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_DontLeave::
# 14 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerbox VyratonIndoors_MayorsOffice_Text_ShouldReturn, EMOTE_THINK
# 15 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkUp
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro::
# 20 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 22 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 27 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 28 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 33 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEDEX_GET, VyratonIndoors_MayorsOffice_EventScript_Samuel_1
# 39 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
	return

VyratonIndoors_MayorsOffice_EventScript_Samuel_1:
# 35 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 45 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon::
# 50 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 51 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 56 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1
# 62 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 63 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
# 58 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 74 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 75 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 76 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 77 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 78 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 80 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 81 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 71 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 72 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 86 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 87 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 88 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
# 90 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 91 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_0x8000, ITEM_POKEDEX
	specialvar VAR_RESULT, IsLeadMonNicknamedOrNotEnglish
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2
# 98 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNonNicknamedMon, localId = 1
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1:
# 100 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_EAST
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2:
# 94 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNicknamedMon, localId = 1
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
# 105 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 106 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_ExtremelySet, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon::
# 111 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_SUPPRESS_MUGSHOT
# 112 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon
# 113 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 114 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon::
# 119 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 120 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 121 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ClearPokedexFlags
# 122 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ShowPokedexHeaderMessage
# 123 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playfanfare MUS_HG_RECEIVE_POKEMON
# 124 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	faceplayer
# 125 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	createfieldmugshot MUGSHOT_PMD, emotion = EMOTE_DETERMINED
# 126 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 127 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call EventScript_DisplayPokedexSpeciesName
# 128 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 129 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	message VyratonIndoors_MayorsOffice_Text_DeterminedMon
# 130 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setseenmon VAR_0x8004
# 131 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon VAR_0x8004
# 132 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 133 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 134 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitfanfare
# 135 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter::
# 140 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 2, DIR_NORTH
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_SOUTH
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3
VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1:
# 149 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2:
# 143 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3:
# 147 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1


# 153 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 155 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 156 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 157 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 157 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 157 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 158 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 159 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 162 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 164 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 165 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 168 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One:
# 170 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 171 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 171 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 172 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 175 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two:
# 177 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 178 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end
