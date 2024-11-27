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
# 21 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setselectedobjectevent 1
# 23 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 28 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 29 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 34 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	switch VAR_IKIGAI_INTRO_STATE
# 36 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	case 2, VyratonIndoors_MayorsOffice_EventScript_Samuel_2
	return

VyratonIndoors_MayorsOffice_EventScript_Samuel_2:
# 36 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 42 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 47 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1
# 53 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 54 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
# 49 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 65 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 66 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 67 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 68 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 69 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 70 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 71 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 72 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 62 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 63 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 77 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 78 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
	return


# 88 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 90 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 91 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 92 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 92 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 92 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 93 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 94 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 97 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 99 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 100 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end
