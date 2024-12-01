VyratonIndoors_MayorsOffice_MapScripts::
# 3 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE
	.byte 0

VyratonIndoors_MayorsOffice_MapScripts_MAP_SCRIPT_ON_FRAME_TABLE:
# 5 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	map_script_2 VAR_IKIGAI_INTRO_STATE, 1, VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro
	.2byte 0


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_DontLeave::
# 15 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playerbox VyratonIndoors_MayorsOffice_Text_ShouldReturn, EMOTE_THINK
# 16 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer Common_Movement_WalkUp
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro::
# 21 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_IKIGAI_INTRO_STATE, 2
# 23 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice
	return


VyratonIndoors_MayorsOffice_EventScript_IkigaiIntro_EnterOffice::
# 28 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_Entrance
# 29 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_KickStart, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel::
# 34 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEDEX_GET, VyratonIndoors_MayorsOffice_EventScript_Samuel_1
# 40 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase
	return

VyratonIndoors_MayorsOffice_EventScript_Samuel_1:
# 36 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_OpenBriefcase::
# 46 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_CrackOpen
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_GoMeetMon::
# 51 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 52 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GoMeetMon
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase::
# 57 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	goto_if_set FLAG_SYS_POKEMON_GET, VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1
# 63 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase
# 64 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_1:
# 59 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner
	return


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase::
	specialvar VAR_RESULT, GetPlayerFacingDirection
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, DIR_WEST
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1:
# 75 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative StartNewPokeballCaseUI
# 76 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 77 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitstate
# 78 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_STARTER_MON, CRY_MODE_NORMAL
# 79 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 2
# 80 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase, FALSE
# 81 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 2, VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 82 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_2:
# 72 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase
# 73 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_OpenBriefcase_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex::
# 87 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 88 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SYS_NATIONAL_DEX
# 89 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special EnableNationalPokedex
# 91 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 92 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setvar VAR_0x8000, ITEM_POKEDEX
	specialvar VAR_RESULT, IsLeadMonNicknamedOrNotEnglish
# 0 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	compare VAR_RESULT, TRUE
	goto_if_eq VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2
# 99 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNonNicknamedMon, localId = 1
VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1:
# 101 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_MeetThomas
# 102 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex
# 103 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_EAST
	return

VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_2:
# 95 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_MeetNicknamedMon, localId = 1
	goto VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_TakePokedex_1


VyratonIndoors_MayorsOffice_EventScript_SamuelBriefcase_MeetPartner::
# 108 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 109 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_ExtremelySet, localId = 1
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon::
# 114 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	clearflag FLAG_SUPPRESS_MUGSHOT
# 115 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon
# 116 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setflag FLAG_SUPPRESS_MUGSHOT
# 117 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_DeterminedMon::
# 122 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	bufferpartymonnick STR_VAR_1, 0
# 123 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 124 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ClearPokedexFlags
# 125 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	callnative ShowPokedexHeaderMessage
# 126 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playfanfare MUS_HG_RECEIVE_POKEMON
# 127 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	faceplayer
# 128 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	createfieldmugshot MUGSHOT_PMD, emotion = EMOTE_DETERMINED
# 129 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	special GetOverworldMonSpecies
# 130 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	call EventScript_DisplayPokedexSpeciesName
# 131 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
# 132 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	message VyratonIndoors_MayorsOffice_Text_DeterminedMon
# 133 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setseenmon VAR_0x8004
# 134 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	setcaughtmon VAR_0x8004
# 135 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitmessage
# 136 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitbuttonpress
# 137 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	waitfanfare
# 138 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
	return


VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter::
# 143 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
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
# 152 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_2:
# 146 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1

VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_3:
# 150 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two
	goto VyratonIndoors_MayorsOffice_EventScript_StarterMon_MetStarter_1


VyratonIndoors_MayorsOffice_EventScript_MeetThomas::
# 157 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadenewbgm MUS_HG_POKEWALKER
# 158 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 120
# 159 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 1, EMOTE_EXCLAM, wait = FALSE
# 160 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_SOUTH
# 161 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	addobject 4
# 162 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance, FALSE
# 163 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance
# 164 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_WEST
# 165 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 166 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_YouForgotThis, MSGBOX_DEFAULT
# 167 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 168 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 4, DIR_SOUTH
# 169 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote 4, EMOTE_QUESTION
# 170 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnobject 1, DIR_SOUTH
# 171 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	speakername known = FALSE
# 172 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	msgbox VyratonIndoors_MayorsOffice_Text_IsThisThem, MSGBOX_DEFAULT
# 173 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	closemessage
# 174 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymoveplayer VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit, FALSE
# 175 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	applymove 4, VyratonIndoors_MayorsOffice_Movement_Thomas_Exit
# 176 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	removeobject 4
# 177 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	fadedefaultbgm
# 178 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay 120
# 179 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	turnplayer DIR_NORTH
	return


VyratonIndoors_MayorsOffice_EventScript_Samuel_ReceivePokdex::
# 184 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	mnmbox VyratonIndoors_MayorsOffice_Text_GivePokedex
	return


# 188 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_Entrance:
# 190 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 191 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 192 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 192 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 192 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 193 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 194 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 197 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromBriefcase:
# 199 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 200 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
	step_end

# 203 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_One:
# 205 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 206 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 206 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 207 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 210 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_ReturnFromStarter_Two:
# 212 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 213 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
	step_end

# 216 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Entrance:
# 218 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 219 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 220 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_left
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 221 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_up
# 222 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_right
	step_end

# 225 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Thomas_Exit:
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 227 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 228 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_right
# 229 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	walk_down
# 230 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 231 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	set_invisible
	step_end

# 234 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasEntrance:
# 236 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	emote_exclamation_mark
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 237 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 238 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 239 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 240 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 241 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 242 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 243 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_up
	step_end

# 246 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
VyratonIndoors_MayorsOffice_Movement_Player_WatchThomasExit:
# 248 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_16
# 249 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 250 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_left
# 251 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_2
# 252 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_4
# 253 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	delay_8
# 254 "data//maps/VyratonIndoors_MayorsOffice/scripts.pory"
	face_down
	step_end
