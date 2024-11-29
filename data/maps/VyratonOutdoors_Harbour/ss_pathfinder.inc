VyratonOutdoors_Harbour_EventScript_SSPathfinder_Departure::
# 11 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	ssannedeparturecutcene 1
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers::
# 16 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	call VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetPassengers
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


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetPassengers::
# 35 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 6
# 36 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_0, VAR_RESULT
# 37 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_0, 15
# 39 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 4
# 40 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_1, VAR_RESULT
# 41 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_1, 23
# 43 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 5
# 44 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_2, VAR_RESULT
# 45 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_2, 31
# 47 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 5
# 48 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_3, VAR_RESULT
# 49 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_3, 44
# 51 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 6
# 52 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_4, VAR_RESULT
# 53 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_4, 15
# 55 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	random 5
# 56 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	copyvar VAR_OBJ_GFX_ID_5, VAR_RESULT
# 57 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addvar VAR_OBJ_GFX_ID_5, 31
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SpawnPassengers::
# 62 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 3
# 63 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 4
# 64 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 5
# 65 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 6
# 66 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 7
# 67 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	addobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_DespawnPassengers::
# 72 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 3
# 73 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 4
# 74 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 5
# 75 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 6
# 76 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 7
# 77 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	removeobject 8
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetOne::
# 82 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne
# 83 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo
# 84 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree
# 85 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour
# 86 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive
# 87 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetTwo::
# 92 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne
# 93 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo
# 94 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree
# 95 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour
# 96 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive
# 97 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetThree::
# 102 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne
# 103 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo
# 104 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree
# 105 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour
# 106 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive
# 107 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFour::
# 112 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne
# 113 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo
# 114 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree
# 115 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour
# 116 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive
# 117 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetFive::
# 122 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne
# 123 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo
# 124 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree
# 125 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour
# 126 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive
# 127 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix
	return


VyratonOutdoors_Harbour_EventScript_SSPathfinder_DisembarkPassengers_SetSix::
# 132 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 3, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne
# 133 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 4, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo
# 134 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 5, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree
# 135 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 6, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour
# 136 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 7, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive
# 137 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	applymovement 8, VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix
	return


# 140 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerOne:
# 142 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 143 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 143 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 144 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 145 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 145 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 145 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 145 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 145 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 148 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerTwo:
# 150 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 151 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 154 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerThree:
# 156 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 156 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 157 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 158 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 159 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 159 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 159 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 160 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 161 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 161 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 161 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 161 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 164 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFour:
# 166 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 166 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 167 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 168 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 169 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 169 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 170 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 171 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 174 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerFive:
# 176 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 176 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 177 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 178 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 179 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 179 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 180 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 180 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 181 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 181 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 181 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 181 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 181 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 184 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetOne_PassangerSix:
# 186 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 186 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 187 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 188 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 189 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 189 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 189 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 190 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 191 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 191 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 191 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 191 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 194 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerOne:
# 196 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 197 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 197 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 197 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 198 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 199 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 199 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 199 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 199 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 202 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerTwo:
# 204 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 205 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 205 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 205 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 205 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 205 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 206 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 206 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 207 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 207 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 210 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerThree:
# 212 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 212 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 213 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 214 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 215 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 215 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 215 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 215 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 216 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 217 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 217 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 217 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 220 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFour:
# 222 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 222 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 223 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 224 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 225 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 225 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 225 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 225 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 225 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 226 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 226 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 227 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 230 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerFive:
# 232 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 232 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 233 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 234 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 235 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
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
	step_end

# 240 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetTwo_PassangerSix:
# 242 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 242 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 243 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 244 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 245 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 246 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 246 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 247 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 250 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerOne:
# 252 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 253 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 253 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 253 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 253 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 254 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 254 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 255 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 258 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerTwo:
# 260 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 261 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 261 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 262 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 263 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 266 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerThree:
# 268 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 268 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 269 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 270 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 271 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 271 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 271 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 271 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 272 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 273 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 273 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 273 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 276 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFour:
# 278 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 278 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 279 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 280 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 281 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 281 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 281 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 282 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 282 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 283 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 283 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 283 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 283 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 283 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 286 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerFive:
# 288 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 288 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 289 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 290 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 291 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 292 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 293 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 293 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 293 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 293 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 296 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetThree_PassangerSix:
# 298 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 298 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 299 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 300 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 301 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 302 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 303 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 303 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 303 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 306 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerOne:
# 308 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 309 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 310 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 311 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 314 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerTwo:
# 316 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
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
	walk_left
# 319 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 319 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 319 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 322 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerThree:
# 324 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 324 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 325 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
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
# 328 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 329 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 332 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFour:
# 334 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 334 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 335 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 336 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 337 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 338 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 339 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 342 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerFive:
# 344 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 344 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 345 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 346 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 347 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 348 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 349 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 349 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 349 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 352 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFour_PassangerSix:
# 354 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 354 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 355 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 356 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 357 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 357 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 357 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 358 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 358 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 359 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 359 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 359 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 359 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 359 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 362 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerOne:
# 364 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 365 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 366 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 367 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 367 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 367 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 370 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerTwo:
# 372 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 373 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
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
	step_end

# 378 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerThree:
# 380 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 380 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 381 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 382 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 383 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
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
	step_end

# 388 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFour:
# 390 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 390 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 391 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 392 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 393 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 394 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
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
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerFive:
# 400 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 400 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 401 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 402 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 403 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 404 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 405 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 405 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 405 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 405 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 405 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 408 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetFive_PassangerSix:
# 410 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 410 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 411 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 412 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 413 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 413 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 413 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 413 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 413 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 414 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 415 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 415 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 415 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 415 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 418 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerOne:
# 420 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 421 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 421 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 421 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 422 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 423 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 423 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 423 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 423 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 423 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 426 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerTwo:
# 428 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
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
	step_end

# 434 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerThree:
# 436 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 436 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 437 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 438 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
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
	walk_right
# 440 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 441 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 441 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 444 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFour:
# 446 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 446 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 447 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 448 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 449 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 450 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 451 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 451 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 451 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 454 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerFive:
# 456 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 456 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 457 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 458 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_4
# 459 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 459 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 459 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 459 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 459 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 460 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_right
# 461 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 461 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 461 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 461 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end

# 464 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
VyratonOutdoors_Harbour_Movement_PassengerSetSix_PassangerSix:
# 466 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 466 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_16
# 467 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_8
# 468 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	delay_2
# 469 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 469 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 469 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 469 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 470 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_left
# 471 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 471 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 471 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 471 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
# 471 "data//maps/VyratonOutdoors_Harbour/ss_pathfinder.pory"
	walk_up
	step_end
