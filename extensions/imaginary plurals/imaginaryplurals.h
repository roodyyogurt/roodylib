#ifclear _IMAGINARYPLURALS_H
#set _IMAGINARYPURALS_H
!\  Imaginary Plurals exists for when a room has lots of items that the player
can't actually interact with.  Say, the PC is in a room full of stuffed teddy
bears, the player could type >GET FIVE TEDDY BEARS and get a response like,
"Nah, you don't really want any."  Any number over the imaginary_plurals
number gets the over_max message, so if the player types
>GET 100 TEDDY BEARS, it can say, "Well, there aren't really THAT many."
\!

!\ Roody's note: Added two properties for plural_class scenery objects.
imaginary_plurals is the number of objects you want the player to think there
are.
over_max is the message they see if they refer to more than that number. \!
property imaginary_plurals alias se_to
property over_max alias sw_to

replace plural_class "(plural_class)"
{
	is openable  ! can't remember why I put this here, probably unnecessary
	type plural_class
	in_scope 0                      ! may be an actor at some point
	size 0                          ! must be weightless
	pluralobj_number 0
	imaginary_plurals 0 ! number at which we want to change responses
	over_max 0 ! message to print when player asks for too much
	plural_verbs
	{
#ifclear NO_VERBS
		if verbroutine = &DoLook, &DoGet, &DoDrop, &DoPutin
			return true
#ifset USE_CHECKHELD
		elseif verbroutine = &DoDrop_CheckHeld, &DoPutin_CheckHeld
			return true
#endif
		else
#endif
			return false
	}

	found_in
	{
		if self = plural_class, identical_class
			return parent(player)
		else
			return false
	}

	! Set the parse_rank of this object to the (highest) parse_rank of
	! its member objects
	plural_parse_rank -100
	parse_rank
	{
		if self.plural_parse_rank = -100
		{
			local i, p, pr = 1
			for (i=1; i<=self.#plural_of; i++)
			{
				p = (self.plural_of #i).parse_rank
				if p > pr
					pr = p
			}
			self.plural_parse_rank = pr
		}
		return self.plural_parse_rank
	}

	before
	{
		object                  ! intercepts all plural_object verbs
		{
			local i, j, n, obj, count, flag

! Count the number of workflagged member objects, to see if there are
! less than specified (or any at all, for that matter).

			for (i=1; i<=self.#plural_of; i++)
			{
				if self.plural_of #i and self.plural_of #i is workflag
				{
					if ++n = 1
						obj = self.plural_of #i
!					AssignPronoun(obj)
!					n++
				}
			}

			if n > 1:  AssignPronoun(self)
			elseif n = 1 : AssignPronoun(obj)
			else ! n = 0
			{
				! "There are none..."
!				OMessage(plural_class, 1)

				if pluralobj_heldmode = 1
					! "You're not holding..."
					ParseError(15, self)
				else
					! "You don't see that..."
					ParseError(11, self)

				return true
			}

			if not self.imaginary_plurals
			{
				if (n < self.pluralobj_number) or
					(n = 1 and self.pluralobj_number = 0):
				{
					! "There are only x things..."
					OMessage(plural_class, 2, n)
					return true
				}
			}
			elseif self.pluralobj_number > self.imaginary_plurals
			{
				run self.over_max
				return true
			}
			else
			{	! not sure about this but I *think* it'll be okay
				self.pluralobj_number =1
			}

! If it's not an allowable plural-objects action, disqualify it:

			if self.pluralobj_number ~=1 and not self.plural_verbs
			{
				! "You'll have to do that one at a time."
				OMessage(plural_class, 3)
				return true
			}

! Handle examining objects as a group specially:
#ifclear NO_VERBS
			if verbroutine=&DoLook
			{
				if self.pluralobj_number ~= 1
				{
					if self.type = identical_class
						return false

					if self.long_desc
						return true
				}

				flag = 0
				for (i=1; i<=self.#plural_of; i++)
				{
					j = self.plural_of #i
					if j and &j.long_desc
						flag = true
				}
				if flag = 0:  return false
			}
#endif	! ifclear NO_VERBS

! Finally, process the action on workflagged individual objects:

			for (i=1; i<=self.#plural_of; i++)
			{
				obj = self.plural_of #i
				if obj and obj is workflag
				{
					if n~=1 and self.pluralobj_number~=1
					{
						if self.type = identical_class and
							obj.article
						{
							print CountWord(count+1);
							print " ";
						}
						print obj.name; ":  ";
					}

					Perform(verbroutine, obj, xobject)
					count++

					AssignPronoun(obj)
				}

				! number of objects specified (if specified)
				if count=self.pluralobj_number and count>0
					break
			}
		}
	}
	is plural, known
}

! Roody's Note: Since this inherits from pural_class, we need to declare it
! again
replace identical_class "(identical_class)"
{
	inherits plural_class
	type identical_class
	plural_parse_rank -100
	parse_rank
	{
		if self.plural_parse_rank = -100
			self.plural_parse_rank = (self.plural_of).parse_rank + 1
		return self.plural_parse_rank
	}
}

#endif ! _IMAGINARYPLURALS_H