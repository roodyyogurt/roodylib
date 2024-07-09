!::
! NPC FOLLOWING
!::

!\  An extension to allow >FOLLOW <CHARACTER> in games with NPCs
that use scripts to move from room to room.

(Not to be confused with followers.h, which is for games where NPCs follow
the protagonist from room to room)

Important note:  To work properly, you must add the following code to your
game's player_character object:

	after
	{
		actor MovePlayer
		{
			local x
			while x < movednpcs[]
			{
				movednpcs[x] = 0
				x++
			}
		}
	}
\!

! Define CONCURRET_ROAMING_NPCS with a higher number before this
! extension is included if your game has more than 3 NPCs the player
! might conceivably see in the same room.

#if undefined CONCURRENT_ROAMING_NPCS
constant CONCURRENT_ROAMING_NPCS 3
#endif

array movednpcs[CONCURRENT_ROAMING_NPCS]
array roamingdirs[CONCURRENT_ROAMING_NPCS]

replace CharMove(char, dir)
{
#ifclear NO_OBJLIB

	local newroom, a

	general = 1

	if dir.type ~= direction
		return

	newroom = parent(char).(dir.dir_to)

	if newroom.type = door
	{
		a = newroom
		newroom = a.between #((parent(char) = \
				a.between #1) + 1)
		if a is not open
		{
			if char in location or newroom = location
			{
				self = a
				RlibOMessage(door, 3)
			}
		}
		elseif newroom = location or char in location
			a = 0
	}

	if char in location and not a and general = 1
	{
		Message(&CharMove, 1, char, dir)
		event_flag = true
	}

	move char to newroom

#ifset DEBUG
	if debug_flags & D_SCRIPTS
	{
		print "["; CThe(char); IsorAre(char, true); " now in:  ";
		print capital parent(char).name; ".]"
	}
#endif

	if char in location and not a and general = 1
	{
		Message(&CharMove, 2, char, dir)
		AddToFollowDir(char, dir)
		event_flag = true
	}
	elseif char in location
		event_flag = true

#endif  ! ifclear NO_OBJLIB

	general = 0 ! always reset it

	run parent(char).after

	return true
}

routine AddToFollowDir(char,dir)
{
	local x
	while movednpcs[x] and x<= movednpcs[] and (movednpcs[x] ~= char)
	{
		x++
	}
	movednpcs[x] = char
	roamingdirs[x] = dir
}

#ifclear _VERBSTUB_H
routine DoFollow
{}
#endif

replace DoFollow
{
	if object = player
	{
		if speaking
			SpeakTo(speaking)
		else
		{
			if player_person = 2
				"Who are you talking to?"
			else
			{
				"It's not obvious who you want ";
				print player.pronoun #2;
				" to talk to."
			}
		}
	}
	elseif object in location
		print CThe(object); IsorAre(object, true); " right here."
	else
	{
		local x
		while movednpcs[x]
		{
			if movednpcs[x] = object
			{
				print "You follow after "; The(object);"."
				if (FORMAT & DESCFORM_I)
					""
				return Perform(&DoGo, roamingdirs[x])
			}
			x++
		}
		print "Which way did "; object.pronoun; " go?"
	}
}
