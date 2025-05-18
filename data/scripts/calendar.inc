EventScript_SetupCalendarWarpHome::
# 3 "data/scripts/calendar.pory"
	lock
# 4 "data/scripts/calendar.pory"
	playerbox Text_TimeToGoToBed, EMOTE_THINK
# 5 "data/scripts/calendar.pory"
	closemessage
# 6 "data/scripts/calendar.pory"
	delay 20
# 7 "data/scripts/calendar.pory"
	callnative DoCalendarWarpHome
# 8 "data/scripts/calendar.pory"
	waitstate
	end


Text_TimeToGoToBed::
# 12 "data/scripts/calendar.pory"
	.string "Wow… It's getting late now.\n"
	.string "I should probably head home to rest\l"
	.string "for another big day tomorrow.$"
