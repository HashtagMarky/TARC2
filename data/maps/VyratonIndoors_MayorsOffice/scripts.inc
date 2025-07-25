VyratonIndoors_MayorsOffice_MapScripts::
# 3 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 3, VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro
	.2byte 0


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_DontLeave::
# 15 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerbox VyratonIndoors_MayorsOffice_Text_ShouldReturn, EMOTE_THINK
# 16 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkUp
	end


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_DontGoTooFar::
# 22 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerbox VyratonIndoors_MayorsOffice_Text_ShouldNotGoTooFar, EMOTE_THINK
# 23 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkDown
	end


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro::
# 29 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 4
# 31 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 36 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 37 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart, localId = 1
	end


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 43 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEDEX_GET, VyratonIndoors_MayorsOffice_EventScript_Samuel_2
# 49 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
VyratonIndoors_MayorsOffice_EventScript_Samuel_1:
# 51 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 1, Common_Movement_FaceOriginalDirection
	end

VyratonIndoors_MayorsOffice_EventScript_Samuel_2:
# 45 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon
	goto VyratonIndoors_MayorsOffice_EventScript_Samuel_1


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 57 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon::
# 62 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 63 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 68 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2
# 74 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 75 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
# 77 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 1, Common_Movement_FaceOriginalDirection
	end

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2:
# 70 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 88 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 89 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 90 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 91 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 92 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 4
# 93 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 94 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Starter_ReturnFromBriefcase
# 95 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 85 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_GoToBriefcase
# 86 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 100 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 101 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_0x8004, SPECIES_ROTOM
	specialvar VAR_RESULT, IsLeadMonNicknamedOrNotEnglish
# 0 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2
# 108 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNonNicknamedMon, localId = 1, lock = FALSE
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1:
# 110 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_MeetThomas
# 111 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex
# 112 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_EAST
# 114 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 115 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 116 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
# 117 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 118 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon SPECIES_ROTOM
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2:
# 104 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNicknamedMon, localId = 1, lock = FALSE
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
# 123 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 124 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_ExtremelySet, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon::
# 129 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_SUPPRESS_MUGSHOT
# 130 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon
# 131 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 132 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter
# 133 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings
	end


VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon::
# 139 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 140 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 141 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ShowPokedexHeaderMessage
# 142 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playfanfare MUS_HG_RECEIVE_POKEMON
# 143 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	faceplayer
# 144 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	createfieldmugshot MUGSHOT_PMD, emotion = MUGSHOT_EMOTE_DETERMINED
# 145 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 146 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call EventScript_DisplayPokedexSpeciesName
# 147 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 148 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	message VyratonIndoors_MayorsOffice_Text_DeterminedMon
# 149 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative SetIkigaiStarterPokedexFlags
# 150 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon VAR_0x8004
# 151 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon SPECIES_ROTOM
# 152 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 153 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 154 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitfanfare
# 155 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 156 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 40
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter::
# 161 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, Common_Movement_FaceUp
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_SOUTH
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3
VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1:
# 170 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2:
# 164 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3:
# 168 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1


VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings::
# 176 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 177 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_OtherImportantThings, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_MeetThomas::
# 182 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadenewbgm MUS_HG_POKEWALKER
# 183 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 184 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 1, EMOTE_EXCLAM, wait = FALSE
# 185 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_SOUTH
# 186 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 3
# 187 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance, FALSE
# 188 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 3, VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance
# 189 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_WEST
# 190 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 191 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_YouForgotThis, MSGBOX_DEFAULT
# 192 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 193 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 3, DIR_SOUTH
# 194 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 3, EMOTE_QUESTION
# 195 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_SOUTH
# 196 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 197 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_IsThisThem, MSGBOX_DEFAULT
# 198 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 199 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit, FALSE
# 200 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 3, VyratonIndoors_MayorsOffice_Movement_Thomas_Exit
# 201 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	removeobject 3
# 202 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadedefaultbgm
# 203 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 204 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex::
# 209 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GivePokedex, lock = FALSE
	return


# 213 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 215 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 215 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 215 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 216 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 219 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_GoToBriefcase:
# 221 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 222 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 225 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 227 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 228 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 229 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 232 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Starter_ReturnFromBriefcase:
# 234 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	exit_pokeball
# 235 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 236 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 239 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One:
# 241 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 242 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 242 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 243 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 246 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two:
# 248 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 249 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 252 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance:
# 254 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 255 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 255 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 256 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 257 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 257 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 257 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 258 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_right
	step_end

# 261 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Exit:
# 263 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 263 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 263 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 264 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 265 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 265 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 266 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 267 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	set_invisible
	step_end

# 270 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance:
# 272 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote_exclamation_mark
# 273 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 273 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 273 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 273 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 274 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 275 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 276 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 277 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 278 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 279 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 282 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit:
# 284 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 285 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 286 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 287 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 288 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 289 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 290 "data/maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_down
	step_end
