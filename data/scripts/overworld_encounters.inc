OverworldEncounters_EventScript_EncounterApproachPlayer::
	lock
	playse SE_PIN
	special DoTrainerApproach
	waitstate
	special GetOverworldMonSpecies
	gotonative LoadOverworldPokemonObjectScript
	end


OverworldEncounters_EventScript_Encounter::
	lock
	faceplayer
	special GetOverworldMonSpecies
	compare VAR_0x8008, EMOTE_ANGRY
	goto_if_ne OverworldEncounters_EventScript_Encounter_2
OverworldEncounters_EventScript_Encounter_1:
	bufferspeciesname STR_VAR_1, VAR_0x8004
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	compare VAR_0x8008, EMOTE_SURPRISED
	goto_if_ne OverworldEncounters_EventScript_Encounter_5
	message OverworldEncounters_EventScript_Encounter_Text_1
OverworldEncounters_EventScript_Encounter_4:
	specialvar VAR_RESULT, GetPlayerFacingDirection
	compare VAR_RESULT, DIR_NORTH
	goto_if_eq OverworldEncounters_EventScript_Encounter_10
OverworldEncounters_EventScript_Encounter_8:
	waitmessage
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_2, 0
	specialvar VAR_RESULT, HasAtLeastOnePokeBall
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Encounter_14
OverworldEncounters_EventScript_Encounter_13:
	checkitem ITEM_POKEBLOCK_CASE
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Encounter_17
OverworldEncounters_EventScript_Encounter_16:
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_5, 3
	dynmultistack 0, 0, TRUE, 4, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_PMD
	switch VAR_RESULT
	case 0, OverworldEncounters_EventScript_Encounter_21
	case 1, OverworldEncounters_EventScript_Encounter_22
	case 2, OverworldEncounters_EventScript_Encounter_23
	goto OverworldEncounters_EventScript_RunAway
OverworldEncounters_EventScript_Encounter_19:
	end

OverworldEncounters_EventScript_Encounter_2:
	setvar VAR_0x8008, EMOTE_SURPRISED
	goto OverworldEncounters_EventScript_Encounter_1

OverworldEncounters_EventScript_Encounter_5:
	message OverworldEncounters_EventScript_Encounter_Text_0
	goto OverworldEncounters_EventScript_Encounter_4

OverworldEncounters_EventScript_Encounter_9:
	special SpawnCameraObject
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement OBJ_EVENT_ID_CAMERA, OverworldEncounters_Movement_CameraAdjust
	waitmovement OBJ_EVENT_ID_CAMERA
	setflag FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	special RemoveCameraObject
	goto OverworldEncounters_EventScript_Encounter_8

OverworldEncounters_EventScript_Encounter_10:
	goto_if_unset FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA, OverworldEncounters_EventScript_Encounter_9
	goto OverworldEncounters_EventScript_Encounter_8

OverworldEncounters_EventScript_Encounter_14:
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_3, 1
	goto OverworldEncounters_EventScript_Encounter_13

OverworldEncounters_EventScript_Encounter_17:
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_4, 2
	goto OverworldEncounters_EventScript_Encounter_16

OverworldEncounters_EventScript_Encounter_21:
	goto OverworldEncounters_EventScript_Fight
	goto OverworldEncounters_EventScript_Encounter_19

OverworldEncounters_EventScript_Encounter_22:
	goto OverworldEncounters_EventScript_ThrowBall
	goto OverworldEncounters_EventScript_Encounter_19

OverworldEncounters_EventScript_Encounter_23:
	goto OverworldEncounters_EventScript_Approach
	goto OverworldEncounters_EventScript_Encounter_19


OverworldEncounters_EventScript_Fight::
	createmon B_SIDE_OPPONENT, B_FLANK_LEFT, VAR_0x8004, VAR_OVERWORLD_MON_LEVEL, isShiny = VAR_0x8005
	closemessage
	waitmoncry
	dowildsafaribattle
	specialvar VAR_RESULT, GetBattleOutcome
	switch VAR_RESULT
	case B_OUTCOME_WON, OverworldEncounters_EventScript_Fight_3
	case B_OUTCOME_DREW, OverworldEncounters_EventScript_Fight_3
	case B_OUTCOME_MON_FLED, OverworldEncounters_EventScript_Fight_4
	case B_OUTCOME_MON_TELEPORTED, OverworldEncounters_EventScript_Fight_4
	case B_OUTCOME_CAUGHT, OverworldEncounters_EventScript_Fight_5
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
	goto OverworldEncounters_EventScript_EncounterEnd
OverworldEncounters_EventScript_Fight_1:
	goto OverworldEncounters_EventScript_EncounterRemove
	end

OverworldEncounters_EventScript_Fight_3:
	playmoncry VAR_0x8004, CRY_MODE_FAINT
	goto OverworldEncounters_EventScript_Fight_1

OverworldEncounters_EventScript_Fight_4:
	playmoncry VAR_0x8004, CRY_MODE_DOUBLES
	goto OverworldEncounters_EventScript_Fight_1

OverworldEncounters_EventScript_Fight_5:
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	goto OverworldEncounters_EventScript_Fight_1


OverworldEncounters_EventScript_ThrowBall::
	setvar VAR_0x8007, ITEM_POKE_BALL
OverworldEncounters_EventScript_ThrowBall_2:
	compare VAR_0x8007, ITEM_CHERISH_BALL
	goto_if_le OverworldEncounters_EventScript_ThrowBall_3
	checkitem ITEM_STRANGE_BALL
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_6
OverworldEncounters_EventScript_ThrowBall_5:
	message OverworldEncounters_EventScript_ThrowBall_Text_1
	waitmessage
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_ITEM
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_12
	copyvar VAR_0x8007, VAR_RESULT
	removeitem VAR_0x8007, 1
	goto OverworldEncounters_EventScript_CatchAttempt
OverworldEncounters_EventScript_ThrowBall_11:
	end

OverworldEncounters_EventScript_ThrowBall_3:
	checkitem VAR_0x8007
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_9
OverworldEncounters_EventScript_ThrowBall_8:
	addvar VAR_0x8007, 1
	goto OverworldEncounters_EventScript_ThrowBall_2

OverworldEncounters_EventScript_ThrowBall_6:
	bufferitemname STR_VAR_1, ITEM_STRANGE_BALL
	dynmultipush OverworldEncounters_EventScript_ThrowBall_Text_0, ITEM_STRANGE_BALL
	goto OverworldEncounters_EventScript_ThrowBall_5

OverworldEncounters_EventScript_ThrowBall_9:
	bufferitemname STR_VAR_1, VAR_0x8007
	dynmultipush OverworldEncounters_EventScript_ThrowBall_Text_0, VAR_0x8007
	goto OverworldEncounters_EventScript_ThrowBall_8

OverworldEncounters_EventScript_ThrowBall_12:
	goto OverworldEncounters_EventScript_Encounter
	goto OverworldEncounters_EventScript_ThrowBall_11


OverworldEncounters_EventScript_Approach::
	special EnterSafariMode
	goto OverworldEncounters_EventScript_Fight
	return


OverworldEncounters_EventScript_CatchAttempt::
	special GetOverworldSpeciesCatchRate
	bufferspeciesname STR_VAR_1, VAR_0x8004
	switch VAR_RESULT
	case 3, OverworldEncounters_EventScript_CatchAttempt_3
	case 1, OverworldEncounters_EventScript_CatchAttempt_4
	message OverworldEncounters_EventScript_CatchAttempt_Text_2
	waitmessage
	waitbuttonpress
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
	message OverworldEncounters_EventScript_CatchAttempt_Text_3
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	waitmessage
	waitbuttonpress
	goto OverworldEncounters_EventScript_Fight
OverworldEncounters_EventScript_CatchAttempt_1:
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	waitmessage
	waitbuttonpress
	closemessage
	givemon VAR_0x8004, VAR_OVERWORLD_MON_LEVEL, isShiny = VAR_0x8005
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	goto OverworldEncounters_EventScript_EncounterRemove
	end

OverworldEncounters_EventScript_CatchAttempt_3:
	message OverworldEncounters_EventScript_CatchAttempt_Text_0
	goto OverworldEncounters_EventScript_CatchAttempt_1

OverworldEncounters_EventScript_CatchAttempt_4:
	message OverworldEncounters_EventScript_CatchAttempt_Text_1
	goto OverworldEncounters_EventScript_CatchAttempt_1


OverworldEncounters_EventScript_RunAway::
	specialvar VAR_RESULT, OverworldTrappedInBattle
	compare VAR_RESULT, BATTLE_RUN_SUCCESS
	goto_if_eq OverworldEncounters_EventScript_RunAway_2
	message OverworldEncounters_EventScript_RunAway_Text_1
	waitmessage
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	waitbuttonpress
	goto OverworldEncounters_EventScript_Fight
OverworldEncounters_EventScript_RunAway_1:
	end

OverworldEncounters_EventScript_RunAway_2:
	message OverworldEncounters_EventScript_RunAway_Text_0
	waitmessage
	waitbuttonpress
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
	goto OverworldEncounters_EventScript_EncounterEnd
	goto OverworldEncounters_EventScript_RunAway_1


OverworldEncounters_EventScript_EncounterEscapeAttempt::
	specialvar VAR_RESULT, WillOverworldEncounterRun
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_EncounterEscapeAttempt_2
OverworldEncounters_EventScript_EncounterEscapeAttempt_1:
	return

OverworldEncounters_EventScript_EncounterEscapeAttempt_2:
	goto OverworldEncounters_EventScript_EncounterEscape
	goto OverworldEncounters_EventScript_EncounterEscapeAttempt_1


OverworldEncounters_EventScript_EncounterEscape::
	closemessage
	special GetOverworldMonSpecies
	playmoncry VAR_0x8004, CRY_MODE_DOUBLES
	goto OverworldEncounters_EventScript_EncounterRemove
	end


OverworldEncounters_EventScript_EncounterRemove::
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	waitfanfare
	waitmoncry
	goto OverworldEncounters_EventScript_EncounterEnd
	end


OverworldEncounters_EventScript_EncounterEnd::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceAwayPlayer
	waitmovement VAR_LAST_TALKED
	specialvar VAR_RESULT, GetPlayerFacingDirection
	compare VAR_RESULT, DIR_NORTH
	goto_if_eq OverworldEncounters_EventScript_EncounterEnd_3
OverworldEncounters_EventScript_EncounterEnd_1:
	call EventScript_DisplayPokedexHeader
	setvar VAR_0x8008, EMOTE_NORMAL
	clearflag FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA
	special ExitSafariMode
	releaseall
	end

OverworldEncounters_EventScript_EncounterEnd_2:
	special SpawnCameraObject
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
	applymovement OBJ_EVENT_ID_CAMERA, OverworldEncounters_Movement_CameraReAdjust
	waitmovement OBJ_EVENT_ID_CAMERA
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
	special RemoveCameraObject
	goto OverworldEncounters_EventScript_EncounterEnd_1

OverworldEncounters_EventScript_EncounterEnd_3:
	goto_if_set FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA, OverworldEncounters_EventScript_EncounterEnd_2
	goto OverworldEncounters_EventScript_EncounterEnd_1


OverworldEncounters_Movement_CameraAdjust:
	walk_up
	step_end

OverworldEncounters_Movement_CameraReAdjust:
	walk_down
	step_end

OverworldEncounters_EventScript_Encounter_Text_0:
	.string "It's a wild {STR_VAR_1}!\n"
	.string "What will {PLAYER} do?$"

OverworldEncounters_EventScript_Encounter_Text_1:
	.string "{PLAYER} caught a wild\n"
	.string "{STR_VAR_1} off-guard!\p"
	.string "The wild {STR_VAR_1} is panicking…\n"
	.string "What will {PLAYER} do?$"

OverworldEncounters_EventScript_Encounter_Text_2:
	.string "{FONT_NARROW}FIGHT$"

OverworldEncounters_EventScript_Encounter_Text_3:
	.string "{FONT_NARROWER}{POKE}BALL$"

OverworldEncounters_EventScript_Encounter_Text_4:
	.string "{FONT_NARROW}APPROACH$"

OverworldEncounters_EventScript_Encounter_Text_5:
	.string "{FONT_NARROW}RUN$"

OverworldEncounters_EventScript_ThrowBall_Text_0:
	.string "{FONT_NARROW}{STR_VAR_1}$"

OverworldEncounters_EventScript_ThrowBall_Text_1:
	.string "What type of {FONT_NARROWER}{POKE}BALL {FONT_NORMAL}will {PLAYER} use?$"

OverworldEncounters_EventScript_CatchAttempt_Text_0:
	.string "The wild {STR_VAR_1} was caught!\n"
	.string "It was a critical capture!$"

OverworldEncounters_EventScript_CatchAttempt_Text_1:
	.string "The wild {STR_VAR_1} was caught!$"

OverworldEncounters_EventScript_CatchAttempt_Text_2:
	.string "The wild {STR_VAR_1} broke free!$"

OverworldEncounters_EventScript_CatchAttempt_Text_3:
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"

OverworldEncounters_EventScript_RunAway_Text_0:
	.string "{PLAYER} got away from the\n"
	.string "wild {STR_VAR_1}!$"

OverworldEncounters_EventScript_RunAway_Text_1:
	.string "Couldn't get away from the\n"
	.string "wild {STR_VAR_1}!\p"
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"
