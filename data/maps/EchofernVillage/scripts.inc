EchofernVillage_MapScripts::
	.byte 0


EchofernVillage_EventScript_Warning::
# 6 "data/maps/EchofernVillage/scripts.pory"
	lock
# 7 "data/maps/EchofernVillage/scripts.pory"
	msgbox EchofernVillage_Text_Warning
# 8 "data/maps/EchofernVillage/scripts.pory"
	release
	end


EchofernVillage_EventScript_Dangerous::
# 12 "data/maps/EchofernVillage/scripts.pory"
	lock
# 28 "data/maps/EchofernVillage/scripts.pory"
	goto EchofernVillage_MonCry_Pangoro
# 38 "data/maps/EchofernVillage/scripts.pory"
	delay 20
# 39 "data/maps/EchofernVillage/scripts.pory"
	applymovement LOCALID_PLAYER, EchofernVillage_EventScript_Dangerous_Movement_0
# 45 "data/maps/EchofernVillage/scripts.pory"
	release
	end


EchofernVillage_MonCry_Annihilape::
# 49 "data/maps/EchofernVillage/scripts.pory"
	playmoncry SPECIES_ANNIHILAPE, CRY_MODE_ENCOUNTER
	return


EchofernVillage_MonCry_Kleavor::
# 53 "data/maps/EchofernVillage/scripts.pory"
	playmoncry SPECIES_KLEAVOR, CRY_MODE_ENCOUNTER
	return


EchofernVillage_MonCry_Trevenant::
# 57 "data/maps/EchofernVillage/scripts.pory"
	playmoncry SPECIES_TREVENANT, CRY_MODE_ENCOUNTER
	return


EchofernVillage_MonCry_Pangoro::
# 61 "data/maps/EchofernVillage/scripts.pory"
	playmoncry SPECIES_PANGORO, CRY_MODE_ENCOUNTER
# 62 "data/maps/EchofernVillage/scripts.pory"
	delay 10
# 63 "data/maps/EchofernVillage/scripts.pory"
	applymovement LOCALID_PLAYER, EchofernVillage_EventScript_Dangerous_Movement_0
# 69 "data/maps/EchofernVillage/scripts.pory"
	waitmoncry
# 70 "data/maps/EchofernVillage/scripts.pory"
	msgbox EchofernVillage_MonCry_Pangoro_Text_0
# 71 "data/maps/EchofernVillage/scripts.pory"
	release
	end


# 39 "data/maps/EchofernVillage/scripts.pory"
EchofernVillage_EventScript_Dangerous_Movement_0:
# 40 "data/maps/EchofernVillage/scripts.pory"
	emote_exclamation_mark
# 41 "data/maps/EchofernVillage/scripts.pory"
	walk_fast_down
# 41 "data/maps/EchofernVillage/scripts.pory"
	walk_fast_down
# 42 "data/maps/EchofernVillage/scripts.pory"
	walk_in_place_fast_up
# 43 "data/maps/EchofernVillage/scripts.pory"
	step_end

EchofernVillage_MonCry_Pangoro_Text_0:
# 70 "data/maps/EchofernVillage/scripts.pory"
	.string "...?\p"
	.string "Was that psf?$"

EchofernVillage_Text_Warning::
# 75 "data/maps/EchofernVillage/scripts.pory"
	.string "This cliff edge looks dangerous...\p"
	.string "I really shouldn't continue further...$"
