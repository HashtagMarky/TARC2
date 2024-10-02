Std_MsgboxNPC:
	lock
	faceplayer
	message NULL
	waitmessage
	waitbuttonpress
	release
	return

Std_MsgboxSign:
	lockall
	message NULL
	waitmessage
	waitbuttonpress
	releaseall
	return

Std_MsgboxDefault:
	message NULL
	waitmessage
	waitbuttonpress
	return

Std_MsgboxYesNo:
	message NULL
	waitmessage
	yesnobox 20, 8
	return

Std_MsgboxGetPoints:
	message NULL
	playfanfare MUS_OBTAIN_B_POINTS
	waitfanfare
	waitmessage
	return

@ Never used, pokenavcall is always used directly instead
Std_MsgboxPokenav:
	pokenavcall NULL
	waitmessage
	return

EventScript_UnusedReturn:
	return

Common_EventScript_SaveGame::
	special SaveGame
	waitstate
	return

Std_MsgboxMugshotNPC::
	lock
	faceplayer
	createfieldmugshot MUGSHOT_NPC, emotion=VAR_0x8002
	call_if_eq VAR_0x8003, TRUE, SetKnownName
	call_if_ne VAR_0x8003, TRUE, SetUnknownName
	message NULL
	waitmessage
	waitbuttonpress
	release
	return

Std_MsgboxMugshotPokemon::
	lock
	faceplayer
	createfieldmugshot MUGSHOT_PMD, emotion=VAR_0x8002
	call_if_eq VAR_0x8003, TRUE, SetKnownName
	call_if_ne VAR_0x8003, TRUE, SetUnknownName
	special GetOverworldMonSpecies
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	message NULL
	waitmessage
	waitbuttonpress
	waitmoncry
	release
	return

SetKnownName:
	speakername known=TRUE
	return

SetUnknownName:
	speakername known=FALSE
	return
