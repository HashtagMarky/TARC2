VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure::
# 11 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	ssannedeparturecutcene 1
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers::
# 16 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	callnative SSPathfinder_SetPassengers
# 17 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SpawnPassengers
# 18 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	playse SE_RG_SS_ANNE_HORN
# 19 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 6
# 20 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	switch VAR_RESULT
# 22 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 0, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_3
# 23 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 1, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_4
# 24 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 2, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_5
# 25 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 3, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_6
# 26 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 4, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_7
# 27 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	case 5, VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_8
VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1:
# 29 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	waitse
# 30 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_DespawnPassengers
	return

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_3:
# 22 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetOne
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_4:
# 23 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetTwo
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_5:
# 24 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetThree
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_6:
# 25 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFour
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_7:
# 26 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFive
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1

VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_8:
# 27 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetSix
	goto VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_1


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SpawnPassengers::
# 35 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 3
# 36 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 4
# 37 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 5
# 38 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 6
# 39 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 7
# 40 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_DespawnPassengers::
# 45 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 3
# 46 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 4
# 47 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 5
# 48 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 6
# 49 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 7
# 50 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetOne::
# 55 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne
# 56 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo
# 57 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree
# 58 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour
# 59 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive
# 60 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetTwo::
# 65 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne
# 66 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo
# 67 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree
# 68 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour
# 69 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive
# 70 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetThree::
# 75 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne
# 76 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo
# 77 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree
# 78 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour
# 79 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive
# 80 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFour::
# 85 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne
# 86 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo
# 87 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree
# 88 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour
# 89 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive
# 90 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFive::
# 95 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne
# 96 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo
# 97 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree
# 98 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour
# 99 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive
# 100 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetSix::
# 105 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne
# 106 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo
# 107 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree
# 108 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour
# 109 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive
# 110 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix
	return


# 113 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne:
# 115 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 116 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 116 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 117 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 118 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 118 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 118 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 118 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 118 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 121 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo:
# 123 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 127 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree:
# 129 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 129 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 130 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 131 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 132 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 132 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 132 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 133 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 134 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 134 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 134 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 134 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 137 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour:
# 139 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 139 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 140 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 141 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 142 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 142 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 143 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 147 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive:
# 149 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 149 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 150 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 152 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 152 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 153 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 153 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 157 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix:
# 159 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 159 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 160 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 161 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 162 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 162 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 162 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 163 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 164 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 164 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 164 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 164 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 167 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne:
# 169 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 170 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 170 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 170 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 172 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 172 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 172 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 172 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 175 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo:
# 177 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 179 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 179 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 180 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 180 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 183 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree:
# 185 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 185 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 186 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 187 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 188 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 188 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 188 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 188 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 189 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 190 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 190 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 190 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 193 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour:
# 195 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 195 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 196 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 197 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 199 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_in_place_left
# 200 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 200 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 201 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 201 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 204 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive:
# 206 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 206 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 207 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 208 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 209 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 210 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 211 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 211 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 211 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 214 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix:
# 216 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 216 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 217 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 218 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 219 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 219 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 219 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 220 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 220 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 221 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 221 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 221 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 221 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 224 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne:
# 226 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 228 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 228 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 229 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 229 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 229 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 232 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo:
# 234 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 236 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 237 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 240 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree:
# 242 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 242 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 243 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 244 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 246 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 250 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour:
# 252 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 252 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 253 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 254 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 256 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 257 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_in_place_left
# 258 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 259 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 259 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 259 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 259 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 259 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 262 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive:
# 264 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 264 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 265 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 266 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 267 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 267 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 267 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 267 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 267 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 268 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 269 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 269 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 269 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 269 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 272 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix:
# 274 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 274 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 275 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 276 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 277 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 278 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 279 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 279 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 279 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 282 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne:
# 284 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 285 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 285 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 286 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 287 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 287 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 288 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_in_place_up
# 289 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 290 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 294 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo:
# 296 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 297 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 297 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 297 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 297 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 297 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 298 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 299 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 299 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 299 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 302 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree:
# 304 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 304 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 305 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 306 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 307 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 307 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 307 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 307 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 308 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 312 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour:
# 314 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 314 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 315 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 316 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 317 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 317 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 317 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 317 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 317 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 318 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 319 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 319 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 322 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive:
# 324 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 324 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 325 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 326 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 327 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 328 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 332 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix:
# 334 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 334 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 335 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 336 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 338 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 338 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 342 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne:
# 344 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 346 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 350 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo:
# 352 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 353 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 353 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 353 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 353 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 354 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 354 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 355 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 355 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 355 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 358 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree:
# 360 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 360 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 361 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 362 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 363 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 364 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 368 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour:
# 370 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 370 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 371 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 372 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 373 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 373 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 373 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 374 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 374 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 375 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 375 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 375 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 375 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 378 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive:
# 380 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 380 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 381 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 382 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 384 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 385 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 385 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 385 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 385 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 385 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 388 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix:
# 390 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 390 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 391 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 392 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 394 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 395 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 395 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 395 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 395 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 398 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne:
# 400 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 401 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 401 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 401 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 402 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 406 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo:
# 408 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 409 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 410 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 411 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 411 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 414 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree:
# 416 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 416 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 417 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 418 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 419 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 419 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 419 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 419 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 419 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 420 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 420 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 421 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 421 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 424 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour:
# 426 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 426 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 427 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 428 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 429 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 430 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 431 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 431 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 431 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 434 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive:
# 436 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 436 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 437 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 438 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 439 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 439 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 439 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 439 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 439 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 440 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_in_place_right
# 441 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 442 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 442 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 442 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 442 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 445 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix:
# 447 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 447 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 448 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 450 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 450 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 450 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 450 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 451 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 452 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 452 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 452 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 452 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 452 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end
