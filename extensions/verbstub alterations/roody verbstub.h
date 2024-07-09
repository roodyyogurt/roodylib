! changed DoSmell to work similarly to Roodylib's DoListen routine
replace DoSmell
{
	if not object
	{
		print capital player.pronoun #1;
		MatchPlural(player, "doesn't", "don't")
		" smell anything unusual."
	}
	elseif not object.after
			"Why do that?"
	return true
}

!\ Roody's note:  RoomSmells exists just for a quick way to do location
sounds (responses for >SMELL without specific objects).  Add it to a room's
before property like so:

	before
	{
		RoomSounds(location)
		{
			"Someone's making brownies!"
		}
	}
\!

routine RoomSmells(obj)
{
	if verbroutine = &DoSmell and not object
		return location
	else
		return false
}

! Roody's note: DoSwim now supports objects, but we'll assume that the object
! given doesn't make any sense.
replace DoSwim
{
	if object.type ~= direction
		ParseError(6) ! "That doesn't make any sense."
	else
		"Not here, you won't."
}