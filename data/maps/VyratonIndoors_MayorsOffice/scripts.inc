VyratonIndoors_MayorsOffice_MapScripts::
# 3 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 3, VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro
	.2byte 0


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_DontLeave::
# 15 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerbox VyratonIndoors_MayorsOffice_Text_ShouldReturn, EMOTE_THINK
# 16 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkUp
	end


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro::
# 22 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 4
# 24 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 29 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 30 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart, localId = 1
	end


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 36 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEDEX_GET, VyratonIndoors_MayorsOffice_EventScript_Samuel_2
# 42 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
VyratonIndoors_MayorsOffice_EventScript_Samuel_1:
# 44 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 1, Common_Movement_FaceOriginalDirection
	end

VyratonIndoors_MayorsOffice_EventScript_Samuel_2:
# 38 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon
	goto VyratonIndoors_MayorsOffice_EventScript_Samuel_1


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 50 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon::
# 55 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 56 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 61 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2
# 67 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 68 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
# 70 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 1, Common_Movement_FaceOriginalDirection
	end

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2:
# 63 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 81 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 82 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 83 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 84 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 85 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 86 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 87 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Starter_ReturnFromBriefcase
# 88 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 78 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_GoToBriefcase
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 93 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 94 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 95 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
# 96 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_DONT_TRANSITION_MUSIC
# 98 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 99 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_0x8000, ITEM_POKEDEX
	specialvar VAR_RESULT, IsLeadMonNicknamedOrNotEnglish
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2
# 106 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNonNicknamedMon, localId = 1, lock = FALSE
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1:
# 108 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_MeetThomas
# 109 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex
# 110 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_EAST
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2:
# 102 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNicknamedMon, localId = 1, lock = FALSE
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
# 115 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 116 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_ExtremelySet, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon::
# 121 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_SUPPRESS_MUGSHOT
# 122 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon
# 123 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 124 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter
# 125 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings
	end


VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon::
# 131 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 132 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 133 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ClearPokedexFlags
# 134 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ShowPokedexHeaderMessage
# 135 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playfanfare MUS_HG_RECEIVE_POKEMON
# 136 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	faceplayer
# 137 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	createfieldmugshot MUGSHOT_PMD, emotion = EMOTE_DETERMINED
# 138 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 139 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call EventScript_DisplayPokedexSpeciesName
# 140 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 141 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	message VyratonIndoors_MayorsOffice_Text_DeterminedMon
# 142 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative SetIkigaiStarterPokedexFlags
# 143 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon VAR_0x8004
# 144 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 145 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 146 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitfanfare
# 147 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 148 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 40
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter::
# 153 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, Common_Movement_FaceUp
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_SOUTH
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3
VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1:
# 162 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2:
# 156 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3:
# 160 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1


VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings::
# 168 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 169 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_OtherImportantThings, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_MeetThomas::
# 174 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadenewbgm MUS_HG_POKEWALKER
# 175 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 176 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 1, EMOTE_EXCLAM, wait = FALSE
# 177 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_SOUTH
# 178 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 4
# 179 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance, FALSE
# 180 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance
# 181 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_WEST
# 182 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 183 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_YouForgotThis, MSGBOX_DEFAULT
# 184 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 185 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 4, DIR_SOUTH
# 186 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 4, EMOTE_QUESTION
# 187 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_SOUTH
# 188 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 189 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_IsThisThem, MSGBOX_DEFAULT
# 190 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 191 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit, FALSE
# 192 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Exit
# 193 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	removeobject 4
# 194 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadedefaultbgm
# 195 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 196 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerface DIR_NORTH
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex::
# 201 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GivePokedex, lock = FALSE
	return


# 205 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 207 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 208 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 209 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 209 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 209 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 210 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 211 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 214 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_GoToBriefcase:
# 216 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 217 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 220 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 222 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 223 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 224 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Starter_ReturnFromBriefcase:
# 229 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	exit_pokeball
# 230 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 231 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 234 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One:
# 236 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 238 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two:
# 243 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 244 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 247 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance:
# 249 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 250 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 253 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_right
	step_end

# 256 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Exit:
# 258 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 258 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 258 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 258 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 258 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 259 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 260 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 261 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 262 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	set_invisible
	step_end

# 265 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance:
# 267 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote_exclamation_mark
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 269 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 270 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 271 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 272 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 273 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 274 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 277 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit:
# 279 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 280 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 281 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 282 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 283 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 284 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 285 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_down
	step_end
