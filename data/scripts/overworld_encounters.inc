OverworldEncounters_EventScript_Encounter::
	lock
	faceplayer
	setvar VAR_0x8006, 5
	special GetOverworldMonSpecies
	bufferspeciesname STR_VAR_1, VAR_0x8004
	specialvar VAR_RESULT, HasAtLeastOnePokeBall
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	message OverworldEncounters_EventScript_Encounter_Text_0
	waitmessage
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_1, 0
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Encounter_2
OverworldEncounters_EventScript_Encounter_1:
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_3, 2
	dynmultistack 0, 0, TRUE, 3, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_PMD
	switch VAR_RESULT
	case 0, OverworldEncounters_EventScript_Encounter_5
	case 1, OverworldEncounters_EventScript_Encounter_6
	case 2, OverworldEncounters_EventScript_Encounter_7
	return

OverworldEncounters_EventScript_Encounter_2:
	dynmultipush OverworldEncounters_EventScript_Encounter_Text_2, 1
	goto OverworldEncounters_EventScript_Encounter_1

OverworldEncounters_EventScript_Encounter_5:
	goto OverworldEncounters_EventScript_Fight
	return

OverworldEncounters_EventScript_Encounter_6:
	goto OverworldEncounters_EventScript_Ball
	return

OverworldEncounters_EventScript_Encounter_7:
	goto OverworldEncounters_EventScript_Run
	return


OverworldEncounters_EventScript_Fight::
	createmon B_SIDE_OPPONENT, B_FLANK_LEFT, VAR_0x8004, VAR_0x8006, isShiny = VAR_0x8005
	closemessage
	waitmoncry
	dowildbattle
	specialvar VAR_RESULT, GetBattleOutcome
	switch VAR_RESULT
	case B_OUTCOME_WON, OverworldEncounters_EventScript_Fight_3
	case B_OUTCOME_MON_FLED, OverworldEncounters_EventScript_Fight_3
	case B_OUTCOME_CAUGHT, OverworldEncounters_EventScript_Fight_3
	playmoncry VAR_0x8004, CRY_MODE_DOUBLES
OverworldEncounters_EventScript_Fight_1:
	waitmoncry
	goto OverworldEncounters_EventScript_EncounterEnd
	return

OverworldEncounters_EventScript_Fight_3:
	playmoncry VAR_0x8004, CRY_MODE_FAINT
	goto OverworldEncounters_EventScript_EncounterRemove
	goto OverworldEncounters_EventScript_Fight_1


OverworldEncounters_EventScript_Ball::
	setvar VAR_0x8007, FIRST_BALL
OverworldEncounters_EventScript_Ball_2:
	compare VAR_0x8007, LAST_BALL
	goto_if_le OverworldEncounters_EventScript_Ball_3
	message OverworldEncounters_EventScript_Ball_Text_1
	waitmessage
	dynmultistack 0, 0, FALSE, 3, TRUE, 0, DYN_MULTICHOICE_CB_SHOW_ITEM
	compare VAR_RESULT, MULTI_B_PRESSED
	goto_if_eq OverworldEncounters_EventScript_Ball_5
	copyvar VAR_0x8007, VAR_RESULT
	removeitem VAR_0x8007, 1
	goto OverworldEncounters_EventScript_CatchAttempt
	return

OverworldEncounters_EventScript_Ball_3:
	checkitem VAR_0x8007
	compare VAR_RESULT, TRUE
	goto_if_eq OverworldEncounters_EventScript_Ball_9
OverworldEncounters_EventScript_Ball_8:
	addvar VAR_0x8007, 1
	goto OverworldEncounters_EventScript_Ball_2

OverworldEncounters_EventScript_Ball_5:
	goto OverworldEncounters_EventScript_Encounter
	return

OverworldEncounters_EventScript_Ball_9:
	bufferitemname STR_VAR_1, VAR_0x8007
	dynmultipush OverworldEncounters_EventScript_Ball_Text_0, VAR_0x8007
	goto OverworldEncounters_EventScript_Ball_8


OverworldEncounters_EventScript_CatchAttempt::
	special GetOverworldSpeciesCatchRate
	bufferspeciesname STR_VAR_1, VAR_0x8004
	switch VAR_RESULT
	case 3, OverworldEncounters_EventScript_CatchAttempt_3
	case 1, OverworldEncounters_EventScript_CatchAttempt_4
	message OverworldEncounters_EventScript_CatchAttempt_Text_2
	waitmessage
	waitbuttonpress
	goto OverworldEncounters_EventScript_Fight
OverworldEncounters_EventScript_CatchAttempt_1:
	playfanfare MUS_OBTAIN_ITEM
	waitfanfare
	waitmessage
	waitbuttonpress
	closemessage
	givemon VAR_0x8004, VAR_0x8006, isShiny = VAR_0x8005
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	goto OverworldEncounters_EventScript_EncounterRemove
	return

OverworldEncounters_EventScript_CatchAttempt_3:
	message OverworldEncounters_EventScript_CatchAttempt_Text_0
	goto OverworldEncounters_EventScript_CatchAttempt_1

OverworldEncounters_EventScript_CatchAttempt_4:
	message OverworldEncounters_EventScript_CatchAttempt_Text_1
	goto OverworldEncounters_EventScript_CatchAttempt_1


OverworldEncounters_EventScript_Run::
	specialvar VAR_RESULT, OverworldTrappedInBattle
	compare VAR_RESULT, BATTLE_RUN_SUCCESS
	goto_if_eq OverworldEncounters_EventScript_Run_1
	message OverworldEncounters_EventScript_Run_Text_1
	waitmessage
	waitbuttonpress
	goto OverworldEncounters_EventScript_Fight
	return

OverworldEncounters_EventScript_Run_1:
	message OverworldEncounters_EventScript_Run_Text_0
	waitmessage
	waitbuttonpress
	closemessage
	goto OverworldEncounters_EventScript_EncounterEnd
	return


OverworldEncounters_EventScript_EncounterEnd::
	releaseall
	end


OverworldEncounters_EventScript_EncounterRemove::
	call Common_EventScript_RemoveStaticPokemon
	waitfanfare
	waitmoncry
	goto OverworldEncounters_EventScript_EncounterEnd
	return


OverworldEncounters_EventScript_Encounter_Text_0:
	.string "It's a wild {STR_VAR_1}!\n"
	.string "What will {PLAYER} do?$"

OverworldEncounters_EventScript_Encounter_Text_1:
	.string "{FONT_NARROW}FIGHT$"

OverworldEncounters_EventScript_Encounter_Text_2:
	.string "{FONT_NARROWER}{POKE}BALL$"

OverworldEncounters_EventScript_Encounter_Text_3:
	.string "{FONT_NARROW}RUN$"

OverworldEncounters_EventScript_Ball_Text_0:
	.string "{FONT_NARROW}{STR_VAR_1}$"

OverworldEncounters_EventScript_Ball_Text_1:
	.string "What type of {FONT_NARROWER}{POKE}BALL {FONT_NORMAL}will {PLAYER} use?$"

OverworldEncounters_EventScript_CatchAttempt_Text_0:
	.string "The wild {STR_VAR_1} was caught!\n"
	.string "It was a critical capture!$"

OverworldEncounters_EventScript_CatchAttempt_Text_1:
	.string "The wild {STR_VAR_1} was caught!$"

OverworldEncounters_EventScript_CatchAttempt_Text_2:
	.string "The wild {STR_VAR_1} broke free!\p"
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"

OverworldEncounters_EventScript_Run_Text_0:
	.string "{PLAYER} got away from the\n"
	.string "wild {STR_VAR_1}!$"

OverworldEncounters_EventScript_Run_Text_1:
	.string "Couldn't get away from the\n"
	.string "wild {STR_VAR_1}!\p"
	.string "The wild {STR_VAR_1} lunged\n"
	.string "at {PLAYER}!$"
