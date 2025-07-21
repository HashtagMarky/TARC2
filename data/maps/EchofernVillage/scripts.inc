EchofernVillage_MapScripts::
	.byte 0


EchofernVillage_EventScript_Warning::
# 6 "data/maps/EchofernVillage/scripts.pory"
	lock
# 7 "data/maps/EchofernVillage/scripts.pory"
	msgbox EchofernVillage_Text_Warning
# 8 "data/maps/EchofernVillage/scripts.pory"
	setvar VAR_ECHOFERN_DANGER, 1
# 9 "data/maps/EchofernVillage/scripts.pory"
	release
	end


EchofernVillage_EventScript_Dangerous::
# 14 "data/maps/EchofernVillage/scripts.pory"
	lock
# 15 "data/maps/EchofernVillage/scripts.pory"
	randomelement SPECIES_ANNIHILAPE, SPECIES_KLEAVOR, SPECIES_TREVENANT, SPECIES_SCOLIPEDE, SPECIES_URSARING, SPECIES_HAXORUS, SPECIES_BASCULEGION, SPECIES_HATTERENE, SPECIES_PANGORO
# 25 "data/maps/EchofernVillage/scripts.pory"
	playmoncry VAR_RESULT, CRY_MODE_ENCOUNTER
# 26 "data/maps/EchofernVillage/scripts.pory"
	call EchofernVillage_MonCry_RunAndReset
# 27 "data/maps/EchofernVillage/scripts.pory"
	compare VAR_RESULT, SPECIES_PANGORO
	goto_if_eq EchofernVillage_EventScript_Dangerous_2
EchofernVillage_EventScript_Dangerous_1:
# 37 "data/maps/EchofernVillage/scripts.pory"
	release
	end

EchofernVillage_EventScript_Dangerous_2:
# 28 "data/maps/EchofernVillage/scripts.pory"
	applymovement LOCALID_PLAYER, EchofernVillage_EventScript_Dangerous_Movement_0
# 32 "data/maps/EchofernVillage/scripts.pory"
	msgbox EchofernVillage_EventScript_Dangerous_Text_0
	goto EchofernVillage_EventScript_Dangerous_1


EchofernVillage_MonCry_RunAndReset::
# 42 "data/maps/EchofernVillage/scripts.pory"
	setvar VAR_ECHOFERN_DANGER, 0
# 43 "data/maps/EchofernVillage/scripts.pory"
	applymovement LOCALID_PLAYER, EchofernVillage_MonCry_RunAndReset_Movement_0
# 50 "data/maps/EchofernVillage/scripts.pory"
	waitmoncry
	return


# 28 "data/maps/EchofernVillage/scripts.pory"
EchofernVillage_EventScript_Dangerous_Movement_0:
# 29 "data/maps/EchofernVillage/scripts.pory"
	emote_question_mark
# 30 "data/maps/EchofernVillage/scripts.pory"
	step_end

# 43 "data/maps/EchofernVillage/scripts.pory"
EchofernVillage_MonCry_RunAndReset_Movement_0:
# 44 "data/maps/EchofernVillage/scripts.pory"
	delay_16
# 45 "data/maps/EchofernVillage/scripts.pory"
	emote_double_exclamation_mark
# 46 "data/maps/EchofernVillage/scripts.pory"
	walk_fast_down
# 46 "data/maps/EchofernVillage/scripts.pory"
	walk_fast_down
# 47 "data/maps/EchofernVillage/scripts.pory"
	walk_in_place_fast_up
# 48 "data/maps/EchofernVillage/scripts.pory"
	step_end

EchofernVillage_EventScript_Dangerous_Text_0:
# 32 "data/maps/EchofernVillage/scripts.pory"
	.string "........................................\n"
	.string "........................................\p"
	.string "Was that psf?$"

EchofernVillage_Text_Warning::
# 53 "data/maps/EchofernVillage/scripts.pory"
	.string "This cliff edge looks dangerous...\p"
	.string "I really shouldn't continue further...$"
