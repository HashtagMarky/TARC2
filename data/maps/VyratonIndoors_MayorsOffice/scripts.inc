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
	end

VyratonIndoors_MayorsOffice_EventScript_Samuel_2:
# 38 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon
	goto VyratonIndoors_MayorsOffice_EventScript_Samuel_1


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 49 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon::
# 54 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 55 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 60 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2
# 66 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 67 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
	end

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_2:
# 62 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 80 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 81 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 82 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 83 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 84 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 85 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 86 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 76 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 77 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 91 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 92 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 93 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
# 95 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 96 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_0x8000, ITEM_POKEDEX
	specialvar VAR_RESULT, IsLeadMonNicknamedOrNotEnglish
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2
# 103 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNonNicknamedMon, localId = 1
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1:
# 105 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_MeetThomas
# 106 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex
# 107 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_EAST
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2:
# 99 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNicknamedMon, localId = 1
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
# 112 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 113 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_ExtremelySet, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon::
# 118 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_SUPPRESS_MUGSHOT
# 119 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon
# 120 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 121 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter
# 122 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings
	end


VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon::
# 128 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 129 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 130 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ClearPokedexFlags
# 131 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ShowPokedexHeaderMessage
# 132 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playfanfare MUS_HG_RECEIVE_POKEMON
# 133 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	faceplayer
# 134 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	createfieldmugshot MUGSHOT_PMD, emotion = EMOTE_DETERMINED
# 135 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 136 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call EventScript_DisplayPokedexSpeciesName
# 137 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 138 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	message VyratonIndoors_MayorsOffice_Text_DeterminedMon
# 139 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setseenmon VAR_0x8004
# 140 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon VAR_0x8004
# 141 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 142 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 143 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitfanfare
# 144 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 145 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 40
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter::
# 150 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
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
# 159 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2:
# 153 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3:
# 157 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1


VyratonIndoors_MayorsOffice_EventScript_StarterMon_OtherImportantThings::
# 165 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 166 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_OtherImportantThings, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_MeetThomas::
# 171 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadenewbgm MUS_HG_POKEWALKER
# 172 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 173 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 1, EMOTE_EXCLAM, wait = FALSE
# 174 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_SOUTH
# 175 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 4
# 176 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance, FALSE
# 177 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance
# 178 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_WEST
# 179 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 180 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_YouForgotThis, MSGBOX_DEFAULT
# 181 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 182 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 4, DIR_SOUTH
# 183 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 4, EMOTE_QUESTION
# 184 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_SOUTH
# 185 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 186 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_IsThisThem, MSGBOX_DEFAULT
# 187 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 188 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit, FALSE
# 189 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Exit
# 190 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	removeobject 4
# 191 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadedefaultbgm
# 192 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 60
# 193 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex::
# 198 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GivePokedex
	return


# 202 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 204 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 205 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 206 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 206 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 206 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 207 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 208 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 211 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 213 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 214 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 217 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One:
# 219 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 220 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 220 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 224 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two:
# 226 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 230 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance:
# 232 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 233 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 234 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 235 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 235 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 235 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 235 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 235 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 236 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_right
	step_end

# 239 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Exit:
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 242 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 243 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 244 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 245 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	set_invisible
	step_end

# 248 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance:
# 250 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote_exclamation_mark
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 253 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 254 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 255 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 256 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 257 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 260 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit:
# 262 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 263 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 264 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 265 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 266 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 267 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 268 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_down
	step_end
