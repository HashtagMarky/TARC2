OverworldEncounters_EventScript_EncounterApproachPlayer::
# 9 "data//scripts/overworld_encounters.pory"
	lock
# 10 "data//scripts/overworld_encounters.pory"
	playse SE_PIN
# 11 "data//scripts/overworld_encounters.pory"
	special DoTrainerApproach
# 12 "data//scripts/overworld_encounters.pory"
	waitstate
# 13 "data//scripts/overworld_encounters.pory"
	special GetOverworldMonSpecies
# 14 "data//scripts/overworld_encounters.pory"
	gotonative LoadOverworldPokemonObjectScript
	end


OverworldEncounters_EventScript_Encounter::
# 20 "data//scripts/overworld_encounters.pory"
	lock
# 21 "data//scripts/overworld_encounters.pory"
	faceplayer
# 22 "data//scripts/overworld_encounters.pory"
	special GetOverworldMonSpecies
# 23 "data//scripts/overworld_encounters.pory"
	compare VAR_0x8008, EMOTE_ANGRY
	goto_if_ne OverworldEncounters_EventScript_Encounter_2
OverworldEncounters_EventScript_Encounter_1:
# 27 "data//scripts/overworld_encounters.pory"
	bufferspeciesname STR_VAR_1, VAR_0x8004
# 28 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
# 29 "data//scripts/overworld_encounters.pory"
	compare VAR_0x8008, EMOTE_SURPRISED
	goto_if_ne OverworldEncounters_EventScript_Encounter_5
# 35 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_Encounter_Text_1
OverworldEncounters_EventScript_Encounter_4:
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, DIR_NORTH
	goto_if_eq OverworldEncounters_EventScript_Encounter_10
OverworldEncounters_EventScript_Encounter_8:
# 47 "data//scripts/overworld_encounters.pory"
	waitmessage
# 48 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_2, 0
	specialvar VAR_RESULT, HasAtLeastOnePokeBall
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Encounter_14
OverworldEncounters_EventScript_Encounter_13:
	checkitem ITEM_POKEBLOCK_CASE
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Encounter_17
OverworldEncounters_EventScript_Encounter_16:
# 57 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_5, 3
# 58 "data//scripts/overworld_encounters.pory"
	dynmultistack 0, 0, TRUE, 4, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_PMD
# 59 "data//scripts/overworld_encounters.pory"
	switch VAR_RESULT
# 61 "data//scripts/overworld_encounters.pory"
	case 0, OverworldEncounters_EventScript_Encounter_21
# 63 "data//scripts/overworld_encounters.pory"
	case 1, OverworldEncounters_EventScript_Encounter_22
# 65 "data//scripts/overworld_encounters.pory"
	case 2, OverworldEncounters_EventScript_Encounter_23
# 68 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_RunAway
OverworldEncounters_EventScript_Encounter_19:
	end

OverworldEncounters_EventScript_Encounter_2:
# 25 "data//scripts/overworld_encounters.pory"
	setvar VAR_0x8008, EMOTE_SURPRISED
	goto OverworldEncounters_EventScript_Encounter_1

OverworldEncounters_EventScript_Encounter_5:
# 31 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_Encounter_Text_0
	goto OverworldEncounters_EventScript_Encounter_4

OverworldEncounters_EventScript_Encounter_9:
# 39 "data//scripts/overworld_encounters.pory"
	special SpawnCameraObject
# 40 "data//scripts/overworld_encounters.pory"
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
# 41 "data//scripts/overworld_encounters.pory"
	applymovement OBJ_EVENT_ID_CAMERA, OverworldEncounters_Movement_CameraAdjust
# 42 "data//scripts/overworld_encounters.pory"
	waitmovement OBJ_EVENT_ID_CAMERA
# 43 "data//scripts/overworld_encounters.pory"
	setflag FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA
# 44 "data//scripts/overworld_encounters.pory"
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
# 45 "data//scripts/overworld_encounters.pory"
	special RemoveCameraObject
	goto OverworldEncounters_EventScript_Encounter_8

OverworldEncounters_EventScript_Encounter_10:
# 37 "data//scripts/overworld_encounters.pory"
	goto_if_unset FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA, OverworldEncounters_EventScript_Encounter_9
	goto OverworldEncounters_EventScript_Encounter_8

OverworldEncounters_EventScript_Encounter_14:
# 51 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_3, 1
	goto OverworldEncounters_EventScript_Encounter_13

OverworldEncounters_EventScript_Encounter_17:
# 55 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_4, 2
	goto OverworldEncounters_EventScript_Encounter_16

OverworldEncounters_EventScript_Encounter_21:
# 62 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Fight
	goto OverworldEncounters_EventScript_Encounter_19

OverworldEncounters_EventScript_Encounter_22:
# 64 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_ThrowBall
	goto OverworldEncounters_EventScript_Encounter_19

OverworldEncounters_EventScript_Encounter_23:
# 66 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Approach
	goto OverworldEncounters_EventScript_Encounter_19


OverworldEncounters_EventScript_Fight::
# 75 "data//scripts/overworld_encounters.pory"
	createmon B_SIDE_OPPONENT, B_FLANK_LEFT, VAR_0x8004, VAR_OVERWORLD_MON_LEVEL, isShiny = VAR_0x8005
# 76 "data//scripts/overworld_encounters.pory"
	closemessage
# 77 "data//scripts/overworld_encounters.pory"
	waitmoncry
# 78 "data//scripts/overworld_encounters.pory"
	dowildsafaribattle
# 79 "data//scripts/overworld_encounters.pory"
	specialvar VAR_RESULT, GetBattleOutcome
# 80 "data//scripts/overworld_encounters.pory"
	switch VAR_RESULT
# 82 "data//scripts/overworld_encounters.pory"
	case B_OUTCOME_WON, OverworldEncounters_EventScript_Fight_3
# 83 "data//scripts/overworld_encounters.pory"
	case B_OUTCOME_DREW, OverworldEncounters_EventScript_Fight_3
# 85 "data//scripts/overworld_encounters.pory"
	case B_OUTCOME_MON_FLED, OverworldEncounters_EventScript_Fight_4
# 86 "data//scripts/overworld_encounters.pory"
	case B_OUTCOME_MON_TELEPORTED, OverworldEncounters_EventScript_Fight_4
# 88 "data//scripts/overworld_encounters.pory"
	case B_OUTCOME_CAUGHT, OverworldEncounters_EventScript_Fight_5
# 91 "data//scripts/overworld_encounters.pory"
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
# 92 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterEnd
OverworldEncounters_EventScript_Fight_1:
# 94 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterRemove
	end

OverworldEncounters_EventScript_Fight_3:
# 84 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_FAINT
	goto OverworldEncounters_EventScript_Fight_1

OverworldEncounters_EventScript_Fight_4:
# 87 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_DOUBLES
	goto OverworldEncounters_EventScript_Fight_1

OverworldEncounters_EventScript_Fight_5:
# 89 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	goto OverworldEncounters_EventScript_Fight_1


OverworldEncounters_EventScript_ThrowBall::
# 100 "data//scripts/overworld_encounters.pory"
	setvar VAR_0x8007, ITEM_POKE_BALL
OverworldEncounters_EventScript_ThrowBall_2:
# 101 "data//scripts/overworld_encounters.pory"
	compare VAR_0x8007, ITEM_CHERISH_BALL
	goto_if_le OverworldEncounters_EventScript_ThrowBall_3
	checkitem ITEM_STRANGE_BALL
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_6
OverworldEncounters_EventScript_ThrowBall_5:
# 115 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_ThrowBall_Text_1
# 116 "data//scripts/overworld_encounters.pory"
	waitmessage
# 117 "data//scripts/overworld_encounters.pory"
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_ITEM
# 118 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_12
# 124 "data//scripts/overworld_encounters.pory"
	copyvar VAR_0x8007, VAR_RESULT
# 125 "data//scripts/overworld_encounters.pory"
	removeitem VAR_0x8007, 1
# 126 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_CatchAttempt
OverworldEncounters_EventScript_ThrowBall_11:
	end

OverworldEncounters_EventScript_ThrowBall_3:
	checkitem VAR_0x8007
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_ThrowBall_9
OverworldEncounters_EventScript_ThrowBall_8:
# 108 "data//scripts/overworld_encounters.pory"
	addvar VAR_0x8007, 1
	goto OverworldEncounters_EventScript_ThrowBall_2

OverworldEncounters_EventScript_ThrowBall_6:
# 112 "data//scripts/overworld_encounters.pory"
	bufferitemname STR_VAR_1, ITEM_STRANGE_BALL
# 113 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_ThrowBall_Text_0, ITEM_STRANGE_BALL
	goto OverworldEncounters_EventScript_ThrowBall_5

OverworldEncounters_EventScript_ThrowBall_9:
# 105 "data//scripts/overworld_encounters.pory"
	bufferitemname STR_VAR_1, VAR_0x8007
# 106 "data//scripts/overworld_encounters.pory"
	dynmultipush OverworldEncounters_EventScript_ThrowBall_Text_0, VAR_0x8007
	goto OverworldEncounters_EventScript_ThrowBall_8

OverworldEncounters_EventScript_ThrowBall_12:
# 120 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Encounter
	goto OverworldEncounters_EventScript_ThrowBall_11


OverworldEncounters_EventScript_Approach::
# 133 "data//scripts/overworld_encounters.pory"
	special EnterSafariMode
# 134 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Fight
	return


OverworldEncounters_EventScript_CatchAttempt::
# 139 "data//scripts/overworld_encounters.pory"
	special GetOverworldSpeciesCatchRate
# 140 "data//scripts/overworld_encounters.pory"
	bufferspeciesname STR_VAR_1, VAR_0x8004
# 141 "data//scripts/overworld_encounters.pory"
	switch VAR_RESULT
# 143 "data//scripts/overworld_encounters.pory"
	case 3, OverworldEncounters_EventScript_CatchAttempt_3
# 145 "data//scripts/overworld_encounters.pory"
	case 1, OverworldEncounters_EventScript_CatchAttempt_4
# 148 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_CatchAttempt_Text_2
# 149 "data//scripts/overworld_encounters.pory"
	waitmessage
# 150 "data//scripts/overworld_encounters.pory"
	waitbuttonpress
# 151 "data//scripts/overworld_encounters.pory"
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
# 152 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_CatchAttempt_Text_3
# 153 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
# 154 "data//scripts/overworld_encounters.pory"
	waitmessage
# 155 "data//scripts/overworld_encounters.pory"
	waitbuttonpress
# 156 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Fight
OverworldEncounters_EventScript_CatchAttempt_1:
# 158 "data//scripts/overworld_encounters.pory"
	playfanfare MUS_OBTAIN_ITEM
# 159 "data//scripts/overworld_encounters.pory"
	waitfanfare
# 160 "data//scripts/overworld_encounters.pory"
	waitmessage
# 161 "data//scripts/overworld_encounters.pory"
	waitbuttonpress
# 162 "data//scripts/overworld_encounters.pory"
	closemessage
# 163 "data//scripts/overworld_encounters.pory"
	givemon VAR_0x8004, VAR_OVERWORLD_MON_LEVEL, isShiny = VAR_0x8005
# 164 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 165 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterRemove
	end

OverworldEncounters_EventScript_CatchAttempt_3:
# 144 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_CatchAttempt_Text_0
	goto OverworldEncounters_EventScript_CatchAttempt_1

OverworldEncounters_EventScript_CatchAttempt_4:
# 146 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_CatchAttempt_Text_1
	goto OverworldEncounters_EventScript_CatchAttempt_1


OverworldEncounters_EventScript_RunAway::
	specialvar VAR_RESULT, OverworldTrappedInBattle
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, BATTLE_RUN_SUCCESS
	goto_if_eq OverworldEncounters_EventScript_RunAway_2
# 181 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_RunAway_Text_1
# 182 "data//scripts/overworld_encounters.pory"
	waitmessage
# 183 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
# 184 "data//scripts/overworld_encounters.pory"
	waitbuttonpress
# 185 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_Fight
OverworldEncounters_EventScript_RunAway_1:
	end

OverworldEncounters_EventScript_RunAway_2:
# 173 "data//scripts/overworld_encounters.pory"
	message OverworldEncounters_EventScript_RunAway_Text_0
# 174 "data//scripts/overworld_encounters.pory"
	waitmessage
# 175 "data//scripts/overworld_encounters.pory"
	waitbuttonpress
# 176 "data//scripts/overworld_encounters.pory"
	call OverworldEncounters_EventScript_EncounterEscapeAttempt
# 177 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterEnd
	goto OverworldEncounters_EventScript_RunAway_1


OverworldEncounters_EventScript_EncounterEscapeAttempt::
	specialvar VAR_RESULT, WillOverworldEncounterRun
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_EncounterEscapeAttempt_2
OverworldEncounters_EventScript_EncounterEscapeAttempt_1:
	return

OverworldEncounters_EventScript_EncounterEscapeAttempt_2:
# 194 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterEscape
	goto OverworldEncounters_EventScript_EncounterEscapeAttempt_1


OverworldEncounters_EventScript_EncounterEscape::
# 201 "data//scripts/overworld_encounters.pory"
	closemessage
# 202 "data//scripts/overworld_encounters.pory"
	special GetOverworldMonSpecies
# 203 "data//scripts/overworld_encounters.pory"
	playmoncry VAR_0x8004, CRY_MODE_DOUBLES
# 204 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterRemove
	end


OverworldEncounters_EventScript_EncounterRemove::
# 210 "data//scripts/overworld_encounters.pory"
	closemessage
# 211 "data//scripts/overworld_encounters.pory"
	fadescreenswapbuffers FADE_TO_BLACK
# 212 "data//scripts/overworld_encounters.pory"
	removeobject VAR_LAST_TALKED
# 213 "data//scripts/overworld_encounters.pory"
	fadescreenswapbuffers FADE_FROM_BLACK
# 214 "data//scripts/overworld_encounters.pory"
	waitfanfare
# 215 "data//scripts/overworld_encounters.pory"
	waitmoncry
# 216 "data//scripts/overworld_encounters.pory"
	goto OverworldEncounters_EventScript_EncounterEnd
	end


OverworldEncounters_EventScript_EncounterEnd::
# 222 "data//scripts/overworld_encounters.pory"
	closemessage
# 223 "data//scripts/overworld_encounters.pory"
	applymovement VAR_LAST_TALKED, Common_Movement_FaceAwayPlayer
# 224 "data//scripts/overworld_encounters.pory"
	waitmovement VAR_LAST_TALKED
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//scripts/overworld_encounters.pory"
	compare VAR_RESULT, DIR_NORTH
	goto_if_eq OverworldEncounters_EventScript_EncounterEnd_3
OverworldEncounters_EventScript_EncounterEnd_1:
# 234 "data//scripts/overworld_encounters.pory"
	call EventScript_DisplayPokedexHeader
# 235 "data//scripts/overworld_encounters.pory"
	setvar VAR_0x8008, EMOTE_NORMAL
# 236 "data//scripts/overworld_encounters.pory"
	clearflag FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA
# 237 "data//scripts/overworld_encounters.pory"
	special ExitSafariMode
# 238 "data//scripts/overworld_encounters.pory"
	releaseall
	end

OverworldEncounters_EventScript_EncounterEnd_2:
# 227 "data//scripts/overworld_encounters.pory"
	special SpawnCameraObject
# 228 "data//scripts/overworld_encounters.pory"
	setflag FLAG_SAFE_FOLLOWER_MOVEMENT
# 229 "data//scripts/overworld_encounters.pory"
	applymovement OBJ_EVENT_ID_CAMERA, OverworldEncounters_Movement_CameraReAdjust
# 230 "data//scripts/overworld_encounters.pory"
	waitmovement OBJ_EVENT_ID_CAMERA
# 231 "data//scripts/overworld_encounters.pory"
	clearflag FLAG_SAFE_FOLLOWER_MOVEMENT
# 232 "data//scripts/overworld_encounters.pory"
	special RemoveCameraObject
	goto OverworldEncounters_EventScript_EncounterEnd_1

OverworldEncounters_EventScript_EncounterEnd_3:
# 225 "data//scripts/overworld_encounters.pory"
	goto_if_set FLAG_SYS_OVERWORLD_ENCOUNTER_CAMERA, OverworldEncounters_EventScript_EncounterEnd_2
	goto OverworldEncounters_EventScript_EncounterEnd_1


# 242 "data//scripts/overworld_encounters.pory"
OverworldEncounters_Movement_CameraAdjust:
# 243 "data//scripts/overworld_encounters.pory"
	walk_up
	step_end

# 246 "data//scripts/overworld_encounters.pory"
OverworldEncounters_Movement_CameraReAdjust:
# 247 "data//scripts/overworld_encounters.pory"
	walk_down
	step_end

OverworldEncounters_EventScript_Encounter_Text_0:
# 31 "data//scripts/overworld_encounters.pory"
	.string "It's a wild {STR_VAR_1}!\n"
	.string "What will {PLAYER} do?$"

OverworldEncounters_EventScript_Encounter_Text_1:
# 35 "data//scripts/overworld_encounters.pory"
	.string "{PLAYER} caught a wild\n"
	.string "{STR_VAR_1} off-guard!\p"
	.string "The wild {STR_VAR_1} is panicking…\n"
	.string "What will {PLAYER} do?$"

OverworldEncounters_EventScript_Encounter_Text_2:
# 48 "data//scripts/overworld_encounters.pory"
	.string "{FONT_GET_NARROW}FIGHT$"

OverworldEncounters_EventScript_Encounter_Text_3:
# 51 "data//scripts/overworld_encounters.pory"
	.string "{FONT_GET_NARROWER}{POKE}BALL$"

OverworldEncounters_EventScript_Encounter_Text_4:
# 55 "data//scripts/overworld_encounters.pory"
	.string "{FONT_GET_NARROW}APPROACH$"

OverworldEncounters_EventScript_Encounter_Text_5:
# 57 "data//scripts/overworld_encounters.pory"
	.string "{FONT_GET_NARROW}RUN$"

OverworldEncounters_EventScript_ThrowBall_Text_0:
# 106 "data//scripts/overworld_encounters.pory"
	.string "{FONT_GET_NARROW}{STR_VAR_1}$"

OverworldEncounters_EventScript_ThrowBall_Text_1:
# 115 "data//scripts/overworld_encounters.pory"
	.string "What {FONT_GET_NARROWER}{POKE}BALL {FONT_NORMAL}will {PLAYER} use?$"

OverworldEncounters_EventScript_CatchAttempt_Text_0:
# 144 "data//scripts/overworld_encounters.pory"
	.string "The wild {STR_VAR_1} was caught!\n"
	.string "It was a critical capture!$"

OverworldEncounters_EventScript_CatchAttempt_Text_1:
# 146 "data//scripts/overworld_encounters.pory"
	.string "The wild {STR_VAR_1} was caught!$"

OverworldEncounters_EventScript_CatchAttempt_Text_2:
# 148 "data//scripts/overworld_encounters.pory"
	.string "The wild {STR_VAR_1} broke free!$"

OverworldEncounters_EventScript_CatchAttempt_Text_3:
# 152 "data//scripts/overworld_encounters.pory"
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"

OverworldEncounters_EventScript_RunAway_Text_0:
# 173 "data//scripts/overworld_encounters.pory"
	.string "{PLAYER} got away from the\n"
	.string "wild {STR_VAR_1}!$"

OverworldEncounters_EventScript_RunAway_Text_1:
# 181 "data//scripts/overworld_encounters.pory"
	.string "Couldn't get away from the\n"
	.string "wild {STR_VAR_1}!\p"
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"
