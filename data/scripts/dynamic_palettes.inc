DynPal_OpenFullMenu::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement OBJ_EVENT_ID_PLAYER
    delay 10
	createfieldmugshot MUGSHOT_PLAYER, emotion=MUGSHOT_EMOTE_HAPPY
	message DynPal_EventScript_Text_DynPal
	special DynPal_ShowFullToneMenu
	waitstate
	closemessage
	release
	end

DynPal_OpenCharacteristicMenu::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement OBJ_EVENT_ID_PLAYER
    delay 10
	createfieldmugshot MUGSHOT_PLAYER, emotion=MUGSHOT_EMOTE_HAPPY
	message DynPal_EventScript_Text_DynPal
	special DynPal_ShowCharacteristicsToneMenu
	waitstate
	closemessage
	release
	end

DynPal_OpenSkinMenu::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement OBJ_EVENT_ID_PLAYER
    delay 10
	createfieldmugshot MUGSHOT_PLAYER, emotion=MUGSHOT_EMOTE_HAPPY
	message DynPal_EventScript_Text_DynPal
	special DynPal_ShowToneMenu_Skin
	waitstate
	closemessage
	release
	end

DynPal_OpenHairMenu::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement OBJ_EVENT_ID_PLAYER
    delay 10
	createfieldmugshot MUGSHOT_PLAYER, emotion=MUGSHOT_EMOTE_HAPPY
	message DynPal_EventScript_Text_DynPal
	special DynPal_ShowToneMenu_Hair
	waitstate
	closemessage
	release
	end

DynPal_OpenClothesMenu::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement OBJ_EVENT_ID_PLAYER
    delay 10
	createfieldmugshot MUGSHOT_PLAYER, emotion=MUGSHOT_EMOTE_HAPPY
	message DynPal_EventScript_Text_DynPal
	special DynPal_ShowToneMenu_Clothes
	waitstate
	closemessage
	release
	end

DynPal_EventScript_Text_DynPal:
	.string "Choose a new style.$"
