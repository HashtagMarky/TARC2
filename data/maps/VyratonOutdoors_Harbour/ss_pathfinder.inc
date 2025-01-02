VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure::
	ssannedeparturecutcene 1
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers::
	callnative SSPathfinder_SetPassengers
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SpawnPassengers
	playse SE_RG_SS_ANNE_HORN
	random 6
	switch VAR_RESULT
	case 0, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_3
	case 1, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_4
	case 2, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_5
	case 3, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_6
	case 4, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_7
	case 5, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_8
VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1:
	waitse
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_DespawnPassengers
	return

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_3:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetOne
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_4:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetTwo
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_5:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetThree
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_6:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFour
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_7:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFive
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_8:
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetSix
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SpawnPassengers::
	addobject 3
	addobject 4
	addobject 5
	addobject 6
	addobject 7
	addobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_DespawnPassengers::
	removeobject 3
	removeobject 4
	removeobject 5
	removeobject 6
	removeobject 7
	removeobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetOne::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetTwo::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetThree::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFour::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFive::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetSix::
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix
	return


VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne:
	delay_4
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo:
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne:
	delay_2
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo:
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_left
	walk_left
	walk_left
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne:
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo:
	delay_2
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne:
	delay_4
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_in_place_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo:
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne:
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo:
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne:
	delay_4
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo:
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree:
	delay_8
	delay_8
	delay_4
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour:
	delay_8
	delay_8
	delay_4
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive:
	delay_16
	delay_16
	delay_8
	delay_4
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix:
	delay_16
	delay_16
	delay_8
	delay_2
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end
