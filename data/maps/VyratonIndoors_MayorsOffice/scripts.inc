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
	message VyratonIndoors_MayorsOffice_Text_ShouldReturn
# 15 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote OBJ_EVENT_ID_PLAYER, EMOTE_THINK, FALSE
# 16 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 17 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 18 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 19 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkUp
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro::
# 24 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 25 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setselectedobjectevent 1
# 27 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 32 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 33 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 38 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	switch VAR_IKIGAI_INTRO_STATE
# 40 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	case 2, VyratonIndoors_MayorsOffice_EventScript_Samuel_2
	return

VyratonIndoors_MayorsOffice_EventScript_Samuel_2:
# 40 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 46 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 51 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	switch VAR_IKIGAI_INTRO_STATE
# 53 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	case 2, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2:
# 53 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 64 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 65 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
# 66 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 67 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 68 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 69 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 70 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 71 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 61 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 62 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


# 75 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 77 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 78 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 80 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 81 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 84 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 86 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 87 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end
