@ Starting from here, these movements are considered
@ 'safe' (won't put follower into a Pokeball)
Common_Movement_FollowerSafeStart::
Common_Movement_QuestionMark:
	emote_question_mark
	step_end

Common_Movement_ExclamationMark:
	emote_exclamation_mark
	step_end

Common_Movement_Heart:
	emote_heart
	step_end

Common_Movement_Smiling:
	emote_smiling
	step_end

Common_Movement_Talking:
	emote_talking
	step_end

Common_Movement_Thinking:
	emote_thinking
	step_end

Common_Movement_DoubleExclamationMark:
	emote_double_exclamation_mark
	step_end

Common_Movement_EmoteX:
	emote_x
	step_end

Common_Movement_Delay48:
	delay_16
	delay_16
	delay_16
	step_end

Common_Movement_FacePlayer:
	face_player
	step_end

Common_Movement_FaceAwayPlayer:
	face_away_player
	step_end

Common_Movement_FaceOriginalDirection:
	face_original_direction
	step_end

Common_Movement_WalkInPlaceFasterLeft:
	walk_in_place_faster_left
	step_end

Common_Movement_WalkInPlaceFasterUp:
	walk_in_place_faster_up
	step_end

Common_Movement_WalkInPlaceFasterRight:
	walk_in_place_faster_right
	step_end

Common_Movement_WalkInPlaceFasterDown:
	walk_in_place_faster_down
	step_end

Common_Movement_FaceRight::
	face_right
	step_end

Common_Movement_FaceLeft::
	face_left
	step_end

Common_Movement_FaceDown::
	face_down
	step_end

Common_Movement_FaceUp::
	face_up
	step_end

Common_Movement_WalkInPlaceDown:
	walk_in_place_down
	step_end

Common_Movement_WalkInPlaceLeft:
	walk_in_place_left
	step_end

Common_Movement_WalkInPlaceRight:
	walk_in_place_right
	step_end

@ End of follower-safe movements
Common_Movement_FollowerSafeEnd::
	step_end

Common_Movement_WalkUp6:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Common_Movement_WalkUp4:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Common_Movement_Delay32:
	delay_16
	delay_16
	step_end

Common_Movement_WalkUp::
	walk_up
	step_end

Common_Movement_WalkDown:
	walk_down
	step_end

Common_Movement_WalkLeft:
	walk_left
	step_end

Common_Movement_WalkRight:
	walk_right
	step_end

@ Unused
Common_Movement_WalkUp2::
	walk_up
	walk_up
	step_end

Common_Movement_DoorEntrance::
	set_visible
    delay_16
    walk_down
	step_end

Common_Movement_DoorExit::
    walk_up
    delay_16
	set_invisible
	step_end

Common_Movement_Waitmovement::
    waitmovement 0
	return

@ Follower NPC
Common_Movement_WalkUpSlow::
    walk_slow_up
    step_end

Common_Movement_WalkDownSlow::
    walk_slow_down
    step_end

Common_Movement_WalkRightSlow::
    walk_slow_right
    step_end

Common_Movement_WalkLeftSlow::
    walk_slow_left
    step_end

Common_Movement_WalkUpFast::
    walk_fast_up
    step_end

Common_Movement_WalkDownFast::
    walk_fast_down
    step_end

Common_Movement_WalkRightFast::
    walk_fast_right
    step_end

Common_Movement_WalkLeftFast::
    walk_fast_left
    step_end

Common_Movement_WalkUpFaster::
    walk_faster_up
    step_end

Common_Movement_WalkDownFaster::
    walk_faster_down
    step_end

Common_Movement_WalkRightFaster::
    walk_faster_right
    step_end

Common_Movement_WalkLeftFaster::
    walk_faster_left
    step_end
