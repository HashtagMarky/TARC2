EventScript_PictureBookShelf::
	msgbox Text_PictureBookShelf, MSGBOX_SIGN
	end

EventScript_BookShelf::
	msgbox Text_BookShelf, MSGBOX_SIGN
	end

EventScript_PokemonCenterBookShelf::
	msgbox Text_PokemonCenterBookShelf, MSGBOX_SIGN
	end

EventScript_Vase::
	msgbox Text_Vase, MSGBOX_SIGN
	end

EventScript_EmptyTrashCan::
	msgbox Text_EmptyTrashCan, MSGBOX_SIGN
	end

EventScript_ShopShelf::
	msgbox Text_ShopShelf, MSGBOX_SIGN
	end

EventScript_Blueprint::
	msgbox Text_Blueprint, MSGBOX_SIGN
	end

EventScript_OfficeCertificate::
	msgbox Text_OfficeCertificate, MSGBOX_SIGN
	end

EventScript_OfficeWaterCooler::
	playse SE_M_BUBBLE3
	end

EventScript_OfficeBlankWhiteboard::
	msgbox Text_OfficeBlankWhiteboard, MSGBOX_SIGN
	end

EventScript_OfficeWhiteboard::
	msgbox Text_OfficeWhiteboard, MSGBOX_SIGN
	end

EventScript_OfficeCoffeeCup::
	callnative DoCoffeeMachineEffect
	end

EventScript_SSPathfinderToilet::
	callnative DoToiletEffect
	playse SE_M_BUBBLE3
	end
