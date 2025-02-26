!::
! ROODYLIB - A collection of hugolib updates and additions
!::

#ifclear _ROODYLIB_H
#set _ROODYLIB_H

constant ROODYBANNER "RoodyLib Version 4.3.0"
constant ROODYVERSION "4.3.0"

#ifset VERSIONS
#message "roodylib.h version 4.3.0"
#endif

#ifset DEBUG
#set HUGOFIX
#endif
!----------------------------------------------------------------------------
!* SOME CONSTANTS AND PROPERTIES AND STUFF
!----------------------------------------------------------------------------

constant LAST_TURN 31 ! used by SaveSettings/LoadSettings to keep track of
                      ! what xverb is being used
#ifclear NO_ACCESSIBILITY
constant CHEAP_ON 1
constant CHEAP_MENUS 2
#endif

! some verbosity constants since I can never remember which value is which
enumerate step + 1
{
	BRIEF, SUPERBRIEF, VERBOSE
}

! some statustype constants
enumerate step * 2
{
	NO_STATUS, SCORE_MOVES = 1, TIME_STATUS, CUSTOM_STATUS, INFOCOM_STYLE,
	MILITARY_TIME
}

enumerate start = 32, step * 2
{
	FINDOBJECT_LIVING, FINDOBJECT_FOUND , FINDOBJECT_CALLED
	FIRSTCOMMAND_F          ! first command of input?
}

! "terp_type" values  2, 4, 8
enumerate step * 2
{
    NORMAL_TERP = 2, GLK_TERP, SIMPLE_TERP
}

enumerate start = 4096, step * 2
{
	DESCFORM_I, DESCFORM_D, CAPLIST_F
}

#ifset USE_RELATIVE_DESCRIPTIONS
global DESCRIBEPLACEFORMAT

enumerate start = 1 , step * 2
{
	OVERRIDEHERE_F, ALSO_F
}
#endif
! if newmenu.h is going to be included, it's easier to just declare the
! usage_desc property now
#if undefined usage_desc
property usage_desc alias short_desc
#endif

!----------------------------------------------------------------------------
!* REPLACED HUGOLIB.H ROUTINES
!----------------------------------------------------------------------------

! Roody's note: AfterRoutines and BeforeRoutines called player.react_before
! twice

replace AfterRoutines
{
	local i, r

	r = player.after
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; player.name;
			if debug_flags & D_OBJNUM
				print " ["; number player; "]";
			print ".after returned "; number r; "]\b"
		}
#endif
	}

	if verbroutine ~= &MovePlayer
		r = player.react_after
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; player.name;
			if debug_flags & D_OBJNUM
				print " ["; number player; "]";
			print ".react_after returned "; number r; "]\b"
		}
#endif
	}

	r = location.after
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; location.name;
			if debug_flags & D_OBJNUM
				print " ["; number location; "]";
			print ".after returned "; number r; "]\b"
		}
#endif
	}

	if verbroutine ~= &MovePlayer
		r = location.react_after
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; location.name;
			if debug_flags & D_OBJNUM
				print " ["; number location; "]";
			print ".react_after returned "; number r; "]\b"
		}
#endif
	}

	for i in location
	{
		if i ~= player
			r = i.react_after
		if r
		{
#ifset DEBUG
			if debug_flags & D_PARSE
			{
				print "\B["; i.name;
				if debug_flags & D_OBJNUM
					print " ["; number i; "]";
				print ".react_after returned "; number r; "]\b"
			}
#endif
		}
	}
}

replace BeforeRoutines(queue)
{
	local r, i

	if verbroutine ~= &MovePlayer
		r = player.react_before
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; player.name;
			if debug_flags & D_OBJNUM
				print " ["; number player; "]";
			print ".react_before returned "; number r; "]\b"
		}
#endif
		return r
	}
	r = player.before
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; player.name;
			if debug_flags & D_OBJNUM
				print " ["; number player; "]";
			print ".before returned "; number r; "]\b"
		}
#endif
		return r
	}
	if verbroutine ~= &MovePlayer
		r = location.react_before
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; location.name;
			if debug_flags & D_OBJNUM
				print " ["; number location; "]";
			print ".react_before returned "; number r; "]\b"
		}
#endif
		return r
	}
	r = location.before
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; location.name;
			if debug_flags & D_OBJNUM
				print " ["; number location; "]";
			print "before returned "; number r; "]\b"
		}
#endif
		return r
	}
	if verbroutine = &MovePlayer
		r = object.before
	if r
	{
#ifset DEBUG
		if debug_flags & D_PARSE
		{
			print "\B["; object.name;
			if debug_flags & D_OBJNUM
				print " ["; number object; "]";
			print "before returned "; number r; "]\b"
		}
#endif
		return r
	}
	for i in location
	{
		if i ~= player
			r = i.react_before
		if r
		{
#ifset DEBUG
			if debug_flags & D_PARSE
			{
				print "\B["; i.name;
				if debug_flags & D_OBJNUM
					print " ["; number i; "]";
				print ".react_before returned "; number r; "]\b"
			}
#endif
			return r
		}
	}

	if verbroutine = &MovePlayer
		return

	! queue is -1 if the object was a number (i.e., a literal digit)
	if queue ~= -1 and xobject > display
	{
		r = xobject.before
		if r
		{
#ifset DEBUG
			if debug_flags & D_PARSE
			{
				print "\B["; xobject.name;
				if debug_flags & D_OBJNUM
					print " ["; number xobject; "]";
				print ".before returned "; number r; "]\b"
			}
#endif
			return r
		}
	}

	if queue ~= -1 and object > display
	{
		r = object.before
		if r
		{
#ifset DEBUG
			if debug_flags & D_PARSE
			{
				print "\B["; object.name;
				if debug_flags & D_OBJNUM
					print " ["; number object; "]";
				print ".before returned "; number r; "]\b"
			}
#endif
			return r
		}
	}
}

! Roody's note: Replaced AnyVerb because the player's PreParse check can
! confuse it. Also added an argument for disallowing SpeakTo since that has
! been coming up a lot for me.
!
! Also made it ignore MovePlayer since AnyVerb exists mainly for the handling
! of commands.

replace AnyVerb(obj,disallow_speakto)
{
#ifclear NO_XVERBS
	if parser_data[VERB_IS_XVERB]
		return false
#endif
	if (verbroutine = &PreParse,&MovePlayer) or
	(verbroutine = &SpeakTo and disallow_speakto)
		return false
	elseif obj
		return obj
	else
		return true
}

#ifclear NO_FUSES
!\
Activate - added a warning for when it is called before the player global has
been set. Also added an option to not run daemons/fuses if the Hugofix
fuse/daemon monitor is on.
\!
replace Activate(a, set)                ! <set> is for fuses only
{
	local err, b

	if a.type = fuse
		b = "fuse"
	else
		b = "daemon"
	if not player
	{
		Font(BOLD_ON)
		print "[WARNING:  The player global must be set before
		daemon (object "; number a;") can be activated.]"
		err = true
	}
#ifset DEBUG
	if debug_flags & D_FUSES
	{
		print "[";
		print b; " "; number a;
		if a.type = fuse
			print " (timer = "; number set; ")";
		" called. Allow activation? (Y/N) > ";
		while true
		{
			pause
			if word[0] = 'y','Y'
			{
				print "Y]"
				break
			}
			elseif word[0] = 'n','N'
			{
				print "N]"
				print "["; b; " "; number a; " canceled.]"
				return false
			}
		}
	}
#endif
	a.in_scope = player
	a is active
	if a.type = fuse and not err
	{
		if set
			a.timer = set

		run a.activate_event
	}
	elseif a.type = daemon and not err
	{
		if set and not a.#timer
		{
			Font(BOLD_ON)
			print "[WARNING:  Attempt to set nonexistent timer
				property on daemon (object "; number a; ")]"
			err = true
		}
		else
			a.timer = set
		run a.activate_event
	}
	elseif not err
	{
		Font(BOLD_ON)
		print "[WARNING:  Attempt to activate non-fuse/\
		daemon (object "; number a; ")]"
		err = true
	}

#ifset DEBUG
	if debug_flags & D_FUSES and not err
	{
		print "[Activating "; b; " "; number a;
		if a.type = fuse
			print " (timer = "; number a.timer; ")";
		print "]"
	}
#endif
	if err
	{
		Font(BOLD_OFF)
		"\npress a key to continue..."
		HiddenPause
	}
	return (not err)
}

replace Deactivate(a)
{
	local err

#ifset HUGOFIX
	move a to fuses_and_daemons
#else
	remove a
#endif
	a.in_scope = 0
	a is not active

	if a.type ~= fuse and a.type ~= daemon
	{
		print "[WARNING:  Attempt to deactivate non-fuse/\
			daemon (object "; number a; ")]"
		err = true
	}
	else
	{
		run a.deactivate_event
	}

#ifset DEBUG
	if debug_flags & D_FUSES and not err
	{
		print "[Deactivating "; a.name; " "; number a; "]"
	}
#endif

	return (not err)
}
#endif ! ifclear NO_FUSES

! Roody's note: Added a new global variable so it's not so important to add
! a holding property to all containers. Character objects still have a holding
! property by default.

global holding_global

replace Acquire(newparent, newchild)
{
	local p,h

	CalculateHolding(newparent)

	if newparent.#holding
		h = newparent.holding
	else
		h = holding_global

	if h + newchild.size > newparent.capacity
		return false
	else
	{
		p = parent(newchild)
		move newchild to newparent
		CalculateHolding(p)
		newchild is moved
		newchild is not hidden
		if newparent.#holding
			newparent.holding = newparent.holding + newchild.size
		return true
	}
}

! AssignPronoun(object)
! sets the appropriate pronoun global to <object>

!\ Roody's note: This version has added support for animals and things being
! called "it" instead of something gendered, written by Mike Snyder.
! Added a "force" argument. Along with different pronoun handling
! in Parse and other places, just adding a true value to "force"
! should guarantee a pronoun change (no longer requiring the author
! to set last_object to -1.
! Also made it so that if there isn't a current value for the applicable
! pronoun global variable, it sets it anyway.
\!

replace AssignPronoun(obj,force)
{
	local a

	if ExcludeFromPronouns(obj)
		return

	! No use if you can't refer to it
	if not obj.#noun and not obj.#adjective
		return
	select true
		case (obj is plural)
		{
			if not them_obj
				a = true
		}
		case (obj is not living or obj.pronoun #2 = "it")
		{
			if not it_obj
				a = true
		}
		case (obj is female)
		{
			if not her_obj
				a = true
		}
		case else
		{
			if not him_obj
				a = true
		}

!	if not a and parser_data[LAST_PARSER_STATUS] & PRONOUNS_SET and not force:  return
	if parser_data[LAST_PARSER_STATUS] & PRONOUNS_SET and not force:  return
	if obj is not living
	{
		if obj is not plural
			it_obj = obj
		else
			them_obj = obj
	}
	else
	{
		if obj is plural
			them_obj = obj
		elseif obj is female
			her_obj = obj
		elseif (obj.pronouns #2) = "it"
			it_obj = obj
		else
			him_obj = obj
	}
	if force
		parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
}

!\ Roody's note: I created a routine for establishing rules for objects
that pronouns should never be set to.  Like the original AssignPronoun,
I've included the player object but I also included direction objects.  Replace
this routine if you'd like special rules for your game. \!

routine ExcludeFromPronouns(obj)
{
	if obj = player:  return true
#ifclear NO_OBJLIB
	elseif obj.type = direction : return true
#endif
	return false
}

!\ Roody's note: This has some extra code added to avoid mobile object
confusion (written by Mike Snyder). Also added support for global_holding
so not all containers necessarily need a holding property. \!

replace CalculateHolding(obj)
{
	local i

	if obj.#holding
	{
		obj.holding = 0
		for i in obj
		{
			if not (i is worn and i is clothing and obj = player)
				obj.holding = obj.holding + i.size
		}
	}
	else
	{
		holding_global = 0
		for i in obj
		{
			if not (i is worn and i is clothing and obj = player)
				holding_global = holding_global + i.size
		}
	}
}

! Roody's note: I forget why, but I thought CenterTitle should be available
! even when menus aren't being used.
#ifset NO_MENUS
! if CenterTitle doesn't exist, I declare it here just to be replaced
routine CenterTitle(a, lines,force)
{}
#endif

replace CenterTitle(a, lines,force, do_not_use_menu_colors)
{
#ifclear NO_ACCESSIBILITY
	if cheap and not force
		return
#endif !ifclear NO_ACCESSIBILITY
	local l, b, c

	if not lines:  lines = 1

	if not (SL_TEXTCOLOR or SL_BGCOLOR)
	{
		b = DEF_SL_BACKGROUND
		c = DEF_SL_FOREGROUND
	}
#ifclear NO_MENUS
	elseif (MENU_SELECTCOLOR or MENU_SELECTBGCOLOR) and
		not do_not_use_menu_colors
	{
		b = MENU_SELECTBGCOLOR
		c = MENU_SELECTCOLOR
	}
#endif
	else
	{
		b = SL_BGCOLOR
		c = SL_TEXTCOLOR
	}

	Font(BOLD_OFF|ITALIC_OFF|UNDERLINE_OFF|PROP_OFF)
	l = string(_temp_string, a)
	if not system(61)
		window 0                        ! remove previous window

	if (TERP & GLK_TERP)
	{
		window 1  ! draw an empty window so glk terps determine screenwidth properly
		{}
	}

	while (l + 1) > (display.linelength * lines)
	{
		lines++
	}

	window lines
	{
		if not (TERP & SIMPLE_TERP)
		{
			color c,b
			cls
			locate 1,1
		}
		print "\_";
		if (l+1) < display.linelength
			print to (display.linelength/2 - l/2);

		print a;
	}
	color TEXTCOLOR, BGCOLOR, INPUTCOLOR
	FONT(DEFAULT_FONT)
	if not (TERP & SIMPLE_TERP) and not force
		cls
	if not system(61) and not force
		locate 1, LinesFromTop
}

#ifclear NO_SCRIPTS

! american vs british spelling
replace CancelScript(obj)
{
	local o

	o = FindScript(obj)
	if o = MAX_SCRIPTS
		return
	scriptdata[o * 3] = 0
	if o = number_scripts - 1
		number_scripts--

#ifset DEBUG
		if debug_flags & D_SCRIPTS
		{
			print "[Script for obj. ";
			print number obj; " ("; obj.name; ") ";
#ifset AMERICAN_ENGLISH
			print "canceled]"
#else
			print "cancelled]"
#endif
		}
#endif ! NO_DEBUG

	return true
}

! Roody's note: This CharMove version tries to work better with doors.
replace CharMove(char, dir)
{
#ifclear NO_OBJLIB

	local newroom, a

	general = 1    ! if general stays true (and the character doesn't go
					   !  through a door or something), we'll get a
						! "So-and-so goes <direction>ward." message

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
		event_flag = true
	}
	elseif char in location
		event_flag = true

#endif  ! ifclear NO_OBJLIB

	general = 0 ! always reset it

	run parent(char).after

	return true
}

! Roody's note: This allows looping scripts without losing a turn.
! When calling it, set a true value for 'loop'
replace LoopScript(obj,loop)
{
	local o

	while scriptdata[o * 3] ~= obj and o < MAX_SCRIPTS:  o++
	if o = MAX_SCRIPTS
		return
	scriptdata[o * 3 + 1] = 0

#ifset DEBUG
	if debug_flags & D_SCRIPTS
	{
		print "[Looping script for obj. ";
		print number obj; " ("; obj.name; ")]"
	}
#endif

	if loop
		RunScripts
}

!\ Roody's note: I also replaced CharGet and CharDrop just to add a
make-sure-there's-an-object check. It's probably unnecessary, but my own
experimentation with CharMove and my findpath extension made me think it might
be good to play it safe. \!

!----------------------------------------------------------------------------
! CharGet
! Script usage:  &CharGet, <object>

replace CharGet(char, obj)
{
	if not obj
		return
	if FindObject(obj, parent(char)) = 1
	{
		if Acquire(char, obj)
		{
			if char in location
			{
				Message(&CharGet, 1, char, obj)
				event_flag = true
			}
		}
		return true
	}
}

!----------------------------------------------------------------------------
! CharDrop
! Script usage:  &CharDrop, <object>

replace CharDrop(char, obj)
{
	if not obj
		return
	move obj to parent(char)
	char.holding = char.holding - obj.size
	if char in location
	{
		Message(&CharDrop, 1, char, obj)
		event_flag = true
	}
	return true
}

#endif ! ifclear NO_SCRIPTS

! Roody's note : This has extra code for components and such, a problem
! first noticed by Jason McWright.
! Also added support for transparent, non-container objects
replace CheckReach(obj)
{
	local i

	if not obj or obj = parent(player)
		return true

#ifclear NO_VERBS
! added a check to make sure the player doesn't have the same parent as the
! object before doing "x is closed." messages
	local parentcheck
	parentcheck = CheckReachParentCheck

	if (verbroutine ~= &DoLook, &DoLookIn) and parent(object) and
		parent(object) ~= player and not parentcheck
	{
	! used to check for transparent here, but FindObject should make that not
	! necessary
		if parent(object) is openable, not open
		{
			VMessage(&DoGet, 5)     ! "X is closed."
			return false
		}
		elseif parent(object) is transparent, not living, not container
		{
#ifset _ROODYLIB_H
			RLibMessage(&CheckReach,1,obj) ! "The <obj> is inside the <parent>."
#else
			print capital The(obj); " is inside "; The(parent(obj)); "."
#endif
			return false
		}
	}
#endif ! NO_VERBS

	if not parent(player).reach or
		Contains(parent(player), obj)
		return true

#ifclear NO_OBJLIB   !  since the component class is part of objlib
	local p
	p = obj.part_of
	while p
	{
	if Contains(parent(player), p)
		return true
	p = p.part_of
	}
#endif

#ifclear NO_VERBS
	if parent(obj) is living
	{
		if verbroutine ~= &DoGet, &DoLook

			! "Except that X has it..."
			Message(&CheckReach, 1, obj)

		elseif verbroutine = &DoGet and parent(obj) is unfriendly

			! "Except that X doesn't want to give it to you..."
			Message(&CheckReach, 2, obj)
	}
#endif

	for (i=1; i<=parent(player).#reach; i++)
	{
		if Contains(parent(player).reach #i, obj) or   ! is the object inside a reach object
          obj = parent(player).reach #i ! is the object a reach object
		{
			return true
		}
	}

#ifclear NO_OBJLIB
	p = obj.part_of
	while p
	{
		for (i=1; i<=parent(player).#reach; i++)
		{
			if Contains(parent(player).reach #i, p) or ! is the object part of an object inside the reach object
				p = parent(player).reach #i ! is the object part of a reach object
			{
				return true
			}
		}
		p = p.part_of
	}
#endif

#ifset USE_ATTACHABLES
	if parent(player).type = attachable
	{
		if InList(parent(player), attached_to, obj)
			return true
	}
	if obj.type = attachable
	{
		if InList(obj, attached_to, parent(player))
			return true
	}
#endif

	! "You can't reach it..."
	Message(&CheckReach, 3, obj)
}

! Roody's note: This exists just to be replaced in the supercontainer
!  extension
routine CheckReachParentCheck
{
	if parent(object) = parent(player)
		return true
	return false
}

#ifset USE_DARK_ROOM
! Roody's note: In games with dark rooms, I didn't like how they look like
! non-rooms. This code makes dark rooms look more "room-y" when the player
! moves into them by printing the name of the darkness object as if it were a
! room name.


! the darkness object is not a room because the room class will be replaced
! further down in Roodylib and it doesn't really need to be a room anyway
object darkness "Darkness"
{}

replace DarkWarning
{
	PrintRoomName(darkness)
	Indent
	print CThe(player); " stumble"; MatchSubject(player); \
		" around in the dark."
}
#endif

!\ Roody's note : Replaced so a new line is still printed, even if indenting
is turned off. Also changed it so that when FORMAT & LIST_F is on, it indents
by the same method for consistency.  \!

replace Indent(newl)
{
	local i

	if newl or (display.cursor_column > 1 and (FORMAT & LIST_F))
		print ""
	if override_indent or display.cursor_column > 1
		print AFTER_PERIOD;
	elseif not (FORMAT & NOINDENT_F)
	{
		if (FORMAT & LIST_F)
		{
			print to 2;
		}
		elseif INDENT_SIZE
		{
		! See display.cursor_column > 1, above
		!print newline
		print "\_";
		for (i=2; i<=INDENT_SIZE; i++)
			print " ";
		}
	}
	override_indent = false
}

!\ Roody's note: added "conjunction" argument so authors can use object
 hierarchy as methods of listing things. For example: you put several objects
 representing choices in a container, then use ListObjects with an "or"
 conjunction to list them all, using "or" if there is more than one choice.
 If no conjunction is specified, it defaults to the AND_WORD constant ("and").
\!
replace ListObjects(thisobj, conjunction)
{
	local i, obj, count, pluralcount,d
	local templist_count            ! temporary total of unlisted objs.
	local id_count                  ! for identical (or plural) objects
#ifset USE_PLURAL_OBJECTS
	local j, this_class
#endif

	if not conjunction
		conjunction = AND_WORD
	list_nest++
	for obj in thisobj
	{
		if obj is hidden
		{
			obj is already_listed
#ifclear NO_OBJLIB
			if obj.type = scenery
				obj is known
#endif  ! NO_OBJLIB
		}
		else
			obj is known

#ifset USE_PLURAL_OBJECTS

		! Need to count identical (and possibly plural) objects
		! for grouping in listing

		if obj.identical_to and obj is not already_listed
		{
			this_class = obj.identical_to
			if this_class.type = identical_class or
				FORMAT & GROUPPLURALS_F
			{
				id_count = 1
				for (i=1; i<=this_class.#plural_of; i++)
				{
					j = this_class.plural_of #i
					if j in thisobj and j~=obj and j is not hidden
					{
						id_count++
						pluralcount++
						list_count--
						j is already_listed
					}
				}
			}
		}
#endif  ! USE_PLURAL_OBJECTS
		if obj is not already_listed
		{
			! May have a leading "is" or "are" that needs to
			! be printed at the head of the list

			if (FORMAT & ISORARE_F)
			{
				if list_count = 1 and id_count <= 1 and
					obj is not plural
					RLibMessage(&ListObjects,1, IS_WORD) ! "is"
				else
					RLibMessage(&ListObjects,1, ARE_WORD) ! "are"
				if (FORMAT & LIST_F)
					RLibMessage(&ListObjects,2) ! ":"
				FORMAT = FORMAT & ~ISORARE_F    ! clear it
			}

			need_newline = false
			if obj is plural
				pluralcount++

			AssignPronoun(obj)
			if not (FORMAT & LIST_F)
			{
				if list_count > 2 and count
					print ",";
				if list_count > 1 and count = list_count - 1
					print " "; conjunction;
				if not (FORMAT & FIRSTCAPITAL_F)
					print " ";
			}
			else
			{
				d = list_nest
				if (verbroutine ~= &DoInventory, &DoLookIn,&DoLook)  and
				FORMAT & NOINDENT_F
					d--
				print to ((d ) * 2);	! INDENT_SIZE);
			}

#ifset USE_PLURAL_OBJECTS

			! If a number of identical (or possibly plural)
			! objects are grouped together, they are printed
			! as a single entry in the list
			!
			if obj.identical_to and
				(this_class.type = identical_class or
				FORMAT & GROUPPLURALS_F)
			{
				if id_count = 1
				{
					if (FORMAT & FIRSTCAPITAL_F) or (FORMAT & LIST_F)
						CArt(obj)
					else
						Art(obj)
				}
				else
				{
					if (FORMAT & FIRSTCAPITAL_F) or (FORMAT & LIST_F)
						print NumberWord(id_count, true);
					else
						print NumberWord(id_count);
					print " "; this_class.name;

					if this_class.type = plural_class
					{
						local k

						if (FORMAT & LIST_F)
							print ":";
						else
							print " (";

						k = 0
						for (i=1; i<=this_class.#plural_of; i++)
						{
							j = this_class.plural_of #i
							if parent(j) = thisobj
							{
								if not (FORMAT & LIST_F)
								{
									if id_count > 2 and k
										print ",";
									if k = id_count - 1
										print " "; AND_WORD;
									if k
										print " ";
								}
								else
								{
									print "\n";
									d = list_nest
									if (verbroutine ~= &DoInventory, &DoLookIn,&DoLook) and  (FORMAT & NOINDENT_F)
										d--
									print to ((d ) * 2);	! INDENT_SIZE);
								}
								if (FORMAT & LIST_F) and
									(FORMAT & CAPLIST_F)
									CArt(j)
								else
									Art(j)
								if not (FORMAT & NOPARENTHESES_F)
									ObjectIs(j)
								k++
							}
						}
						if not (FORMAT & LIST_F):  print ")";
					}
				}
			}
			else
			{
#endif ! USE_PLURAL_OBJECTS
				! Regular old non-plural, non-identical
				! objects get listed here:

				if (FORMAT & FIRSTCAPITAL_F) or
				((FORMAT & LIST_F) and
					(FORMAT & CAPLIST_F))
					CArt(obj)
				else:  Art(obj)
				if not (FORMAT & NOPARENTHESES_F)
					ObjectIs(obj)
#ifset USE_PLURAL_OBJECTS
			}
#endif
			FORMAT = FORMAT & ~FIRSTCAPITAL_F       ! clear it

			count++
		}

		! For itemized listings, list the children of
		! each object immediately after that object (unless
		! it is a SpecialDesc-printed description)

		if obj is not hidden and (FORMAT & LIST_F)
		{
			print newline
			if children(obj)
			{
				if not obj.list_contents
				{
					templist_count = list_count
					WhatsIn(obj)
					list_count = templist_count
				}
			}
		}
	}

	! If not an itemized list, it is necessary to finish off the
	! sentence, adding any necessary words at the end.  Then, the
	! children of all previously objects listed at this level are
	! listed.

	if not (FORMAT & LIST_F)
	{
		if count
		{
			if list_nest = 1 and (FORMAT & ISORAREHERE_F)
			{
				if count + pluralcount > 1
					print " "; ARE_WORD;
				else:  print " "; IS_WORD;
#ifset USE_RELATIVE_DESCRIPTIONS
				if not (DESCRIBEPLACEFORMAT & OVERRIDEHERE_F)  or
				((DESCRIBEPLACEFORMAT & OVERRIDEHERE_F) and
				thisobj = parent(player))
#endif
					print " "; HERE_WORD;
#ifset USE_RELATIVE_DESCRIPTIONS
				if DESCRIBEPLACEFORMAT & OVERRIDEHERE_F
				{
					if DESCRIBEPLACEFORMAT & ALSO_F
					{
						print " also";
						DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT & ~ALSO_F
					}
					print " ";
					RelativeText(thisobj)
					if DESCRIBEPLACEFORMAT & OVERRIDEHERE_F
						DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT & ~OVERRIDEHERE_F
						! clear it
				}
#endif
				FORMAT = FORMAT & ~ISORAREHERE_F  ! clear it
				if not ((FORMAT & LIST_F) or (FORMAT & TEMPLIST_F))
					override_indent = true
			}

			if not (FORMAT & NORECURSE_F)
				print ".";
		}

		i = 0
		local char_count
		for obj in thisobj
		{
			if children(obj) and obj is not hidden and
				(obj is not already_listed or
					thisobj ~= location) and not ClothingCheck(obj)
			char_count++
			if char_count = 2
				break
		}
		for obj in thisobj
		{
			if children(obj) and obj is not hidden and
				(obj is not already_listed or
					thisobj ~= location) and not ClothingCheck(obj)
			{
				if (FORMAT & TEMPLIST_F)
				{
					FORMAT = FORMAT | LIST_F & ~TEMPLIST_F
					i = true
					print newline
				}

#ifclear NO_OBJLIB
				if count > 1 and obj.type = character
				{
					FORMAT = FORMAT | USECHARNAMES_F
					if char_count = 2
					{
						print newline
						override_indent = false
					}
				}
#endif
				templist_count = list_count
				WhatsIn(obj)
				list_count = templist_count
			}
		}
	}

	if --list_nest = 0
	{
		if not (FORMAT & LIST_F) and not (FORMAT & NORECURSE_F)
		{
			print newline
			override_indent = false
			need_newline = false
		}
	}
}

! Roody's note : added conjunction argument like in ListObjects above.
replace PropertyList(obj, prop, artic, conjunction)
{
	local a, b, n, total

	if not conjunction
		conjunction = AND_WORD

	for (a=1; a<=obj.#prop; a++)
	{
		if obj.prop #a:  total++
	}

	for (a=1; a<=obj.#prop; a++)
	{
		b = obj.prop #a
		if b
		{
			if artic
				The(b)
			else
				Art(b)

			if a < total and total > 2
				print ", ";
			elseif a = total - 1:  print " ";

			if a = total - 1
				print conjunction; " ";
			n++
		}
	}
	return n
}

!----------------------------------------------------------------------------
! ObjectIs(object)
! prints a list of parenthetical attributes

replace ObjectIs(obj)
{
	local n

	if not &obj.desc_detail
	{
#ifclear LIST_CLOTHES_FIRST
		if obj is clothing:  n = TestObjectIs(obj, worn, "being worn", n)
#endif
		n = TestObjectIs(obj, light, "providing light", n)

		if n:  print ")";
	}
	else
		run obj.desc_detail
}

! Roody's note: was missing a space
replace TestObjectIs(obj, attr, str, n)
{
	if obj is attr
	{
		if n = 0
			print " (";
		else
			print " "; AND_WORD; " ";
		print str;
		n++
	}
	return n
}

#ifset USE_ELEVATED_PLATFORMS
attribute elevated
#endif

!\ Roody's note: I replaced WhatsIn so printed things like the colon-printing
(":") are sent to RLibMessages for easier message configuring.
  Changed a carriage return to 'print newline' \!

replace WhatsIn(obj,dont_flush)
{
	local i, totallisted
	local initial_list_nest

	if (FORMAT & NORECURSE_F)
		return

	for i in obj
	{
		if not dont_flush
			i is not already_listed
		if i is not hidden and i is not already_listed
			totallisted++
	}

	if totallisted = 0
	{
		print newline
		return
	}
	list_count = totallisted

	if obj is not container or obj is platform or
		(obj is container and (obj is not openable or
			(obj is openable and
				(obj is open or obj is transparent)))) and
		(obj ~= player or (FORMAT & INVENTORY_F)) and obj is not quiet
	{
		SpecialDesc(obj)

		! If list_count is 0 now, but totallisted was not, something must have been
		! printed by SpecialDesc

		if dont_flush and list_count
		{
			list_count = 0
			for i in obj
			{
				if i is not hidden and i is not already_listed
					list_count++
			}
		}

		if list_count = 0
		{
			if (FORMAT & INVENTORY_F) and not (FORMAT & LIST_F) and
				list_nest = 0
			{
				print newline    ! was ' print "" '
			}
			return totallisted
		}

		if obj.list_contents
			return totallisted

		initial_list_nest = list_nest

		if (FORMAT & LIST_F)
		{
			if list_nest
			{
				! Indent the first time so that it lines up with
				! paragraph indentation:
				i = list_nest
				if (FORMAT & NOINDENT_F) and
				verbroutine ~= &DoInventory, &DoLook, &DoLookIn
				{
					i--
				}
				print to (i * 2);	! INDENT_SIZE);
			}
		}
		else
			Indent

		if obj.contains_desc    ! custom heading
		{
			if (FORMAT & LIST_F)
				RLibMessage(&WhatsIn, 1 )  !  ":"
#ifset USE_RELATIVE_DESCRIPTIONS
			FORMAT = FORMAT & ~FIRSTCAPITAL_F
			FORMAT = FORMAT & ~ISORAREHERE_F
			DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT & ~ OVERRIDEHERE_F
#endif
		}
		else
		{
			if obj = player
			{
				if (FORMAT & LIST_F) and list_count < totallisted
					print "\n";

				! "You are carrying..."
				Message(&WhatsIn, 1, totallisted)

				if (FORMAT & LIST_F)
					RLibMessage(&WhatsIn, 1 )  !  ":"
			}
			elseif obj is living and not obj.prep
			{
				! "X has..."
				Message(&WhatsIn, 2, obj, totallisted)
				if (FORMAT & LIST_F)
					RLibMessage(&WhatsIn, 1 )  !  ":"
			}
#ifset USE_RELATIVE_DESCRIPTIONS
			elseif not (DESCRIBEPLACEFORMAT & OVERRIDEHERE_F)
#else
			else
#endif
			{
				if list_count < totallisted
					! "Also sitting on/in..."
					RLibMessage(&WhatsIn, 2, obj )
				else
					! "Sitting on/in..."
					RLibMessage(&WhatsIn, 3, obj )

				The(obj) ! "the <object>"
				FORMAT = FORMAT | ISORARE_F
! FORMAT = FORMAT | ISORARE_F tells ListObjects to print "is" or
!  "are" before beginning the list
			}
		}

		ListObjects(obj)
		for i in obj
		{
			i is already_listed
		}
		list_nest = initial_list_nest
	}
	return totallisted
}


!\ Roody's note: Added some code to SpecialDesc so that if the obj is the only
child of a parent, it still checks (and lists) any children of that obj. \!
replace SpecialDesc(obj)
{
	local a, c, flag, printed_blankline, d

	if (FORMAT & LIST_F)
		return

	list_count = 0
	for a in obj
	{
		if a is not hidden
		{
			flag = true
			c++
		}
		else
			flag = false

		if (FORMAT & INVENTORY_F) and obj = player and flag
		{
			if &a.inv_desc
			{
				print newline
				Indent
			}
			if a.inv_desc
			{
				if (FORMAT & LIST_F):  print newline
				AddSpecialDesc(a)
			}
		}
		elseif a is not moved and flag
		{
			if &a.initial_desc
			{
				d++
				print newline
				override_indent = false
				if (FORMAT & INVENTORY_F) and (FORMAT & NOINDENT_F) and not printed_blankline
					print ""
				printed_blankline = true
				Indent
				need_newline = true
			}
			if a.initial_desc
				AddSpecialDesc(a)
		}
	}
	list_count = c - list_count
	if not list_count and c
	{
		for a in obj
		{
			if children(a) and a is not quiet and
			(a is platform or a is transparent or
			(a is container and
			(a is not openable or (a is openable and a is open)))) and
			not a.list_contents and not ClothingCheck(a)
			{
				WhatsIn(a)
				list_count = 0
			}
		}
	}
}

! Roody's note: The following is a routine mainly used by the
! LIST_CLOTHES_FIRST switch, so that containers and platforms that are
! clothing don't have their contents listed twice. For that reason, it's
! called "ClothingCheck" but this routine might be useful to replace
! anytime you want to split parts of the inventory up different ways.

routine ClothingCheck(a)
{
#ifset LIST_CLOTHES_FIRST
	if (a is worn and a is clothing and verbroutine ~= &ListClothesFirst) or
		(a is not worn and verbroutine = &ListClothesFirst)
		return true
	else
#endif
		return false
}

! Roody's note: Changed it so word[1] is cleared when leaving the routine
! so chained YesorNo's don't get confused by the unchanging word array
replace YesorNo
{
	local w, count, val, ret
	w = word[1]
	while true
	{
		select w
			case "yes", "y":ret = ++val
			case "no", "n": ret = true
			case else
			{
!				if (FORMAT & DESCFORM_I) and not count
!					""
				if ++count < 10
					RlibMessage(&YesorNo, 1)	! ask "yes" or "no"
				else
					RlibMessage(&YesorNo, 2)
				GetInput
				w = word[1]
			}
		if ret
		{
			word[1] = "" ! clearing word[1], duh
			return val
		}
	}
}

!----------------------------------------------------------------------------
!* REPLACED HUGOFIX.H ROUTINES
!----------------------------------------------------------------------------

! Roody's note: Changed the parse$-printing code.
#ifset HUGOFIX
enumerate start = 64, step * 2
{
	D_PLAYBACK_HELPER, D_OPCODE_MONITOR
}

! Roody's note: Added objects to provide test for the "$"/"$?" response
! so extensions can add commands to the relevant grouping as needed.
! (currently hugofix_monitoring is the only one used by the op code
!   hugofix commands)

object hugofix_monitoring "hugofix monitoring"
{
	in hugofixlib
	long_desc
	{
		"Monitoring:\n\
		\_    $on - Toggle object numbers"

#ifclear NO_SCRIPTS
	       "\_    $sc - Script monitor on/off"
#endif
#ifclear NO_FUSES
	       "\_    $fd - Fuse/daemon monitor on/off"
#endif

               "\_    $fi - FindObject monitoring on/off"
               "\_    $pm - Parser monitoring on/off"
	}
}



object hugofix_manipulation "hugofix object manipulation"
{
	in hugofixlib
	long_desc
	{
	       "Object manipulation:\n\
		\_    $at <obj.> is [not] <attr. #>\n\
		\_                         - set/clear object attribute\n\
		\_    $mo <obj.> to <obj.> - Move object to new parent\n
		\_    $mp <obj.>           - Move player object to new
			parent"
	}
}

object hugofix_testing "hugofix object testing"
{
	nearby
	long_desc
	{
		"Object testing:\n\
		\_    $fo [obj.]   - Find object (or player, if no obj.)\n\
		\_    $na <obj. #> - Print name of object number <obj.>\n\
		\_    $nu <obj.>   - Print number of object <obj.>"
	}
}

object hugofix_parser "hugofix parser testing"
{
	nearby
	long_desc
	{
		"Parser testing:\n\
		\_    $ca                - Check articles for all objects\n\
		\_    $pc [$all] [obj.]  - Check parser conflicts (for <obj.>)\n\
		\_                         (parse_ranked objects only or all)\n\
		\_    $pn                - List current pronouns\n\
		\_    $pr                - parse_rank monitoring"
	}
}

object hugofix_utilities "hugofix utilities"
{
	nearby
	long_desc
	{
		"Other utilities:"

#ifclear NO_FUSES
	       "\_    $ac <obj.> [timer] - Activate fuse (with timer) or
			daemon\n\
		\_    $de <obj.>         - Deactivate fuse or daemon"
#endif
#ifclear NO_OBJLIB
	       "\_    $di [obj.]         - Audit directions (for room <obj.>)"
#endif
	       "\_    $ii                - Display interpreter information\n\
		\_    $kn                - Set all objects to 'known'\n\
		\_    $kn <obj. #>       - Make <obj.> 'known'\n\
		\_    $nr                - Normalize random numbers\n\
		\_    $ot [obj. | $loc]  - Print object tree (beginning with
			<obj.>)\n\
		\_    $rp                - Skip in-game pauses in recording playback\n\
		\_    $rr                - Restore \"random\" random numbers\n\
		\_    $uk <obj.>         - Make object unknown\n\
		\_    $wo <val.>         - Print dictionary word entry <val.>\n\
		\_    $wn <word>         - Value/addr. of (lowercase) <word>\n\
		\n\_    $au                - Run HugoFixAudit"
	}
}

! Added some additional Hugofix commands and added an object system so
! extensions can add Hugofix commands as needed.
replace DoHugoFix
{
	local i, n

	Font(PROP_OFF) ! proportional printing off

	if ((VerbWord ~= "$wo", "$wn") and object > objects and object > 0) or
		((VerbWord ~= "$ac", "$at") and xobject > objects)
	{
		"[Object number(s) out of range]"
		Font(DEFAULT_FONT)
		return
	}

	select VerbWord
	case "$", "$?"
	{
		"HUGOFIX DEBUGGING COMMANDS:\n\
		\_    $?  - Display this help menu\n"
		i = child(hugofixlib)
		while i
		{
			run i.long_desc
			n = child(i)
			while n
			{
				run n.long_desc
				n = younger(n)
			}
			i = younger(i)
			if i
				""
		}
	}

#ifclear NO_FUSES
	case "$ac"
	{
		if object.type ~= fuse and object.type ~= daemon
			"Not a fuse or daemon."
		else
		{
			Activate(object)
			object.timer = xobject
			"Activated."
		}
	}
	case "$de"
	{
		if object.type ~= fuse and object.type ~= daemon
			"Not a fuse or daemon."
		else
		{
			Deactivate(object)
			"Deactivated."
		}
	}
#endif

	case "$at"
	{
		local isnot

		for (i=1; i<=words; i++)
		{
			if word[i] = "": break
			if word[i] = "not": isnot = true
		}

		print "[Setting "; object.name; " is ";

		n = xobject

		if isnot
		{
			object is not n
			"not ";
		}
		else
			object is n

		print "attribute "; number n; "]"
	}
	case "$au"
	{
		HugoFixAudit
	}
	case "$ca"
	{
		"[Checking articles]"
		for (i=last_library_object+1; i<objects; i++)
		{
			if (i.noun or i.adjective) and not i.article
			{
				print "Object \""; i.name; "\" ("; number i; ") has no article"
			}
		}
		"[...Done]"
	}

#ifclear NO_OBJLIB
	case "$di"
	{
		local place

		if object:  place = object
		else:  place = location

		print "DIRECTIONS FROM:  "; capital place.name; " "; "("; \
			number place; ")"
		local printed
		for i in direction
		{
			if &place.(i.dir_to)
			{
				print "\_ "; i.name; ":  (Property routine)"
				printed++
			}
			else
			{
				n = place.(i.dir_to)
				if n > 1
				{
					print "\_ "; i.name; ":  "; \
						capital n.name
					printed++
				}
				elseif n = 0
				{
					if &place.cant_go
					{
						print "\_ "; i.name; ":  ";
						if not place.cant_go
							print newline
						printed++
					}
				}
			}
		}
		if not printed:  "\_ (None)"
	}
#endif

#ifclear NO_FUSES
	case "$fd"
	{
		"[Fuse/daemon monitor";
		OnorOff(D_FUSES)
	}
#endif

	case "$fi"
	{
		"[FindObject monitoring";
		OnorOff(D_FINDOBJECT)
	}
	case "$fo"
	{
		if not object:  object = player
		"[Parent of obj. "; : print number object; " ("; \
		object.name; "):  Obj. ";
		print number (parent(object)); " ("; parent(object).name; ")]"
	}
	case "$ii"
	{
		local g
		g = IsGlk
		print "This is a ";
		if display.hasgraphics
			print "graphics-capable ";
		else
		   print "non-graphics-capable ";
		if g
			print "Glk-based ";
		elseif (not g and system(61))
			print "minimal interface ";
		print "interpreter."
		Indent(true)
		print "The screen width is "; number display.screenwidth; " characters."
		Indent
		print "The screen height is ";  number display.screenheight; " characters."
		Indent
		print "The line length is "; number display.linelength; " characters."
		Indent
		print "The status line height is "; number display.statusline_height; " characters."
		Indent
		print "The window lines height is ";
		if not g
			print number display.windowlines; " characters."
		else
			print "unavailable."
		if opcodeterp is switchedon
		{
			"\nFurther interpreter capabilities can be listed with the
			opcode audit command, \"$oa\"."
		}
	}
	case "$kn"
	{
		if object
		{
			object is known
			print "["; object.name; " now 'known']"
		}
		else
		{
#ifset NO_OBJLIB
			i = 0
#else
			i = out_obj + 1
#endif
			for (; i<objects; i++)
				i is known
			"[All objects now 'known']"
		}
	}
	case "$mo"
	{
		if word[words] = "$loc":  xobject = location
		print "[Obj. "; number object; " ("; object.name; ") ";
		print "moved to obj. "; number xobject; " (";
		print xobject.name; ")]"
		move object to xobject
		object is not hidden
		object is known
	}
	case "$mp"
	{
		if (parent(object) and object is not enterable)
		{
			print "[Obj. "; number object; " (";
			print object.name; ") is not a room or enterable object]"
		}
#ifclear NO_OBJLIB
		elseif (not parent(object) and object.type ~= room)
		{
			print "[Obj. "; number object; " (";
			print object.name; ") is not a room or enterable object]"
		}
#endif
		else
		{
			Font(DEFAULT_FONT)
			MovePlayer(object, false, true)
			Font(PROP_OFF)
			"[Player moved to obj. ";
			print number object; " ("; object.name; ")]"
		}
	}
	case "$na", "$nu"
	{
		print "[Obj. "; number object; ": \""; object.name; "\"]"
	}
	case "$nr"
	{
		system(21)
		"[Random numbers now normalized]"
	}

	case "$on"
	{
		"[Object numbers";
		OnorOff(D_OBJNUM)
	}
	case "$ot"
	{
		if object = 0 and words = 1
		{
			"[Specify a parent object to draw the tree from, or
			'$ot 0' to draw the entire object tree, including the children
			of the \"object classes\" and \"settings\" objects.]\n"
	!		Roodylib prints the object tree, skipping classes and settings.
		}
		elseif word[2] = "0"
			"[Skipped object numbers are replaced objects.]\n"

		if word[2] = "$loc":  object = location
		print "["; number object; "] ";
		if object = 1
			print "(display)"
		elseif object.name
			print object.name
#ifset _WINDOW_H
		elseif object.type = window_class
		{
			if object = window_class
				print "(window_class)"
			else
				print "(window object)"
		}
#endif
		else
			print "<replaced object>"
		list_nest = (object ~= 0)
		for (i=0; i<objects; i=i+1)
		{
			local a,b,c
#ifset _WINDOW_H
			if i.type = window_class
				a++
#endif
#ifclear NO_FUSES
			if i.type = fuse and i ~= fuse
				b++
			elseif i.type = daemon and i ~= daemon
				c++
#endif
			if i ~= object and parent(i) = object
				DrawBranch(i,a,b,c)
		}
	}
	case "$pc"
	{
		local parse_rank_only = true

		if word[2] = "$all"
			parse_rank_only = false

		if object
		{
			if not ListParserConflicts(object, parse_rank_only)
			{
				"No parser conflicts for:  ";
				PrintParserInfoForObject(object)
			}
		}
		else
		{
			"NOTE:  You have chosen to check parser conflicts for the
			entire tree";
			if not parse_rank_only
				" for ALL objects";
			".  This may take a long time depending on the number of
			objects/adjectives/nouns.  Do you wish to continue (YES OR NO)? ";
			input
			if not YesOrNo
			{
				Font(DEFAULT_FONT)
				return
			}
			"[Checking parser conflicts...]"

			local count
			for (i=0; i<objects; i++)
			{
				if i.noun and (not parse_rank_only or i.parse_rank)
				{
					if ListParserConflicts(i, parse_rank_only)
						count++
				}
			}
			if not count
				"No parser conflicts found."
		}
	}
	case "$pn"
	{
		n = 1
		while n < 5
		{
			select n
				case 1
				{
					print "it object: ";
					i = it_obj
				}
				case 2
				{
					print "him object: ";
					i = him_obj
				}
				case 3
				{
					print "her object: ";
					i = her_obj
				}
				case 4
				{
					print "them object: ";
					i = them_obj
				}
			if not i
				print "unassigned"
			else
				print i.name; " ("; number i; ")"
			n++
		}
	}
	case "$rp"
	{
		"[In-game pause-skipping in recording playback";
		OnorOff(D_PLAYBACK_HELPER)
	}
	case "$pm"
	{
		"[Parser monitoring";
		OnorOff(D_PARSE)
	}
	case "$pr"
	{
		"[parse_rank monitoring";
		OnorOff(D_PARSE_RANK)
	}
	case "$rr"
	{
		system(22)
		"[\"Random\" random numbers restored]"
	}

#ifclear NO_SCRIPTS
	case "$sc"
	{
		"[Script monitor";
		OnorOff(D_SCRIPTS)
	}
#endif

	case "$uk"
	{
		object is not known
		print "["; object.name; " now not 'known']"
	}
	case "$wn"
	{
		print "[\""; word[2]; "\": "; number word[2]; "]"
	}
	case "$wo"
	{
		print "[\""; object; "\": "; number object; "]"
	}
	case else
	{
		i = child(hugofixlib)
		while i
		{
			local ret
			n = child(i)
			while n
			{
				if Inlist(n, extra_scenery, word[1])
				{
					run n.long_desc
					ret = true
				}
				if not ret
					n = younger(n)
				else
					break
			}
			if not ret
				i = younger(i)
			else
				break
		}
	}
	Font(DEFAULT_FONT)	! restore font (i.e., proportional printing
				! if it was turned off)
}

! Roody's note - added some code so windows and replaced objects
! get listed when printing the object tree
replace DrawBranch(obj,win_count,fuse_count,daemon_count)
{
	local i, nextobj

	if (not word[2] or word[2] = "0") and not obj.name and obj ~= 1
	{
#ifset _WINDOW_H
		if obj.type ~= window_class
#endif
			return  ! skip replaced objects
	}
	for (i=1; i<=list_nest; i++)
		print ". . ";
	print "["; number obj; "] ";
		if obj = 1
			print "(display)"
#ifclear NO_FUSES
		elseif obj.name = "(fuse)" and obj ~= fuse and not list_nest
			print "(fuse #"; number fuse_count;")"
		elseif obj.name = "(daemon)" and obj ~= daemon and not list_nest
			print "(daemon #"; number daemon_count;")"
#endif
		elseif obj.name
			print obj.name
#ifset _WINDOW_H
		elseif obj.type =  window_class and obj ~= window_class
			print "(untitled window #"; number win_count ; ")"
#endif
		else
			print "<replaced object>"

	if (not word[2] and (obj = settings, object_classes))
		return

	for nextobj in obj
	{
		list_nest++
		DrawBranch(nextobj)
		list_nest--
	}
}

replace CheckParserConflicts(obj1, obj2, what)
{
	local n

	for (n=1; n<obj1.#what; n++)
	{
		if obj1.what #n and InList(obj2, what, obj1.what #n)
		{
			if obj1 is not workflag
			{
				"\n\B*** Conflicts with: ";
				PrintParserInfoForObject(obj1)
				Font(BOLD_OFF)
			}
			"\_ ";
			PrintParserInfoForObject(obj2, obj1)
			obj1 is workflag
			return true
		}
	}
}

replace ListParserConflicts(obj, parse_rank_only)
{
	local i, matched, check

	obj is not workflag

	for (i=0; i<objects; i++)
	{

		if parse_rank_only
		{
			if obj.parse_rank or i.parse_rank
				check = true
		}
		else
			check = true

		if i ~= obj and check and i.noun
		{
			if i.adjective
			{
				if CheckParserConflicts(obj, i, adjectives)
					matched = true
			}

			if not matched
			{
				if CheckParserConflicts(obj, i, nouns)
					matched = true
			}
		}
		check = false
	}

	return matched
}

! Roody's note: Added an option to call it silently
replace OnorOff(a,silent)
{
	if not (debug_flags & a)
	{
		if not silent
			print " on.]"
		debug_flags = debug_flags | a
	}
	else
	{
		if not silent
			print " off.]"
		debug_flags = debug_flags & ~a
	}
}

! Roody's note - just got rid of a jump
replace The(obj, a)
{
	AssignPronoun(obj)

	if obj = player and player_person = 1 and a
		print player.pronoun #2;
	else
	{
		if obj.article
			print "the ";
		print obj.name;
	}

	if (debug_flags & D_OBJNUM)
		print " ["; number obj; "]";
}

! Roody's note - just got rid of a jump
replace Art(obj, a)
{
	AssignPronoun(obj)

	if obj = player and player_person = 1 and a
		print player.pronoun #2;
	else
	{
		if obj.article
			print obj.article; " ";
		print obj.name;
	}

	if (debug_flags & D_OBJNUM)
		print " ["; number obj; "]";
}

! Roody's note: just replaced to add a note that a holding property isn't so
! important with Roodylib
replace HugoFixAudit
{
	local obj, err, count

	Font(PROP_OFF)

	"Validating library object setup..."

	! Make sure "nothing" is object 0
	if nothing ~= 0
	{
		"\"nothing\" object is not object 0"
		err++
	}

#ifset USE_PLURAL_OBJECTS
	! Check for proper plural/identical objects initialization
	local i

	"Validating plural/identical objects..."
	count = 0

	for (obj=1; obj<objects; obj++)
	{
		if (obj.type = plural_class, identical_class) and
			(obj ~= plural_class, identical_class)
		{
			count++

			if not obj.plural_of
			{
				err++
				"Plural/identical class ";
				print "\""; obj.name; "\" ("; number obj; ")";
				" has no 'plural_of' property"
			}
			elseif obj.type = identical_class
			{
				for (i=1; i<=obj.#plural_of; i++)
				{
					if obj.plural_of #i.identical_to ~= obj
					{
						err++
						"Identical object ";
						print "\""; obj.plural_of #i.name; "\" ";
						print "("; number obj.plural_of #i; ") ";
						"should have 'identical_class' ";
						print "\""; obj.name; "\" ";
						print "("; number obj; ") ";
						"in 'identical_to' property"
					}
				}
			}
		}
	}

	print number count;
	" plural/identical object(s) checked"

#endif	! ifset USE_PLURAL_OBJECTS

	"Validating containers and platforms..."
	count = 0

	for (obj=1; obj<objects; obj++)
	{
		if obj is container or obj is platform
		{
			count++

			local a
			if obj.capacity and not obj.#holding and not &obj.holding
			{
				if obj is container
					"Container ";
				else
					"Platform ";
				print "\""; obj.name; "\" ("; number obj; ")";
				" has 'capacity' but no 'holding' property"
				err++
				a++
			}
			if a
			{
				"\B(Roodylib containers and platforms should still work even
				without the holding property.)\b"
			}
			if obj.holding and not obj.#capacity and not &obj.capacity
			{
				if obj is container
					"Container ";
				else
					"Platform ";
				print "\""; obj.name; "\" ("; number obj; ")";
				" has 'holding' but no 'capacity' property"
				err++
			}
		}
	}

	print number count;
	" container/platform object(s) checked"

	print "[";
	if err
		print number err;
	else
		print "No";
	" error(s)/warning(s) found by HugoFixAudit]"
	Font(DEFAULT_FONT)
}

! Roody's note:  The following routines are called at the beginning of the game,
! allowing to start monitoring various things before the game has actually
! started.

routine HugoFixInit
{
	local key, n

   while true
	{
		local ret, cheap_mode
		while true
		{
			if not (TERP & SIMPLE_TERP)
				cls
			else
				""
			if not (TERP & SIMPLE_TERP)
				CenterTitle("HugoFix Debugging Options")
			if display.needs_repaint
				display.needs_repaint = false
			print newline
			Indent
			print "\_  ";
			Font(BOLD_ON)
			print "HugoFix Debugging Options"
			Font(BOLD_OFF)
			""
			n = PrintHugoFixOptions
			print ""
			"Select the number of your choice (or ESCAPE to begin the game)"
			key = HiddenPause
			local numb
			if key = 'q','Q', '0', ESCAPE_KEY, ' ', ENTER_KEY
			{
				ret = 0
				break
			}
#ifclear BETA
			elseif key = 't','T'
			{
				ret = key
				break
			}
#endif
			else
				numb = key - 48

			if numb and (numb > 0) and (numb <= n)
			{
				ret = numb
				break
			}
			""
		}
		if not (TERP & SIMPLE_TERP)
			cls
		if not ret
			break
#ifclear BETA
		elseif ret = 't','T'
		{
				if betalib is not special
				{
					""
					if (not scripton)
						VMessage(&DoScriptOnOff, 1) ! "Unable to begin
															 ! transcription."
					else
					{
						VMessage(&DoScriptOnOff, 2) ! "Transcription on."
						betalib is special
					}
					""
					CoolPause
					if betalib is special
						break
				}
		}
		else
#endif ! ifclear BETA
			OnOrOff((hugofixinitmenu.hugofixmenu #ret),1)
		window 0 ! only to draw a line in simple interpreters
#ifclear NO_ACCESSIBILITY
		cheap_mode = cheap
#endif
		if not (cheap_mode or (TERP & SIMPLE_TERP))
			cls
	}
}

routine PrintHugoFixOptions
{
	local x
	run hugofixinitmenu.long_desc
	x = child(hugofixinitmenu)
	while x
	{
		run x.long_desc
		x = younger(x)
	}
#ifclear BETA
	if betalib is not special
	{
		Indent
		print "T. Start a transcript and begin game"
	}
#endif
	return (hugofixinitmenu.hugofixmenu #10)
}

property hugofixmenu alias s_to

object hugofixinitmenu "hugofixinitmenu"
{
	in settings
	type settings
	hugofixmenu 0 0 0 0 0 0 0 0 0 0
	long_desc
	{
		local n = 1, sel = 1 , matched = true
		while true
		{
			if matched
			{
				Indent
				print number sel; ". ";
			}
			select n
			case 1
			{
				print "Toggle object numbers (";
				if (debug_flags & D_OBJNUM)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_OBJNUM
				matched = true
			}
#ifclear NO_SCRIPTS
			case 2
			{
				print "Script Monitor (";
				if (debug_flags & D_SCRIPTS)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_SCRIPTS
				matched = true
			}
#endif
#ifclear NO_FUSES
			case 3
			{
				print "Fuse/Daemon Monitor (";
				if (debug_flags & D_FUSES)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_FUSES
				matched = true
			}
#endif
			case 4
			{
				print "FindObject Monitoring (";
				if (debug_flags & D_FINDOBJECT)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_FINDOBJECT
				matched = true
			}
			case 5
			{
				print "Parser Monitoring (";
				if (debug_flags & D_PARSE)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_PARSE
				matched = true
			}
			case 6
			{
				print "Parse Rank Monitoring (";
				if (debug_flags & D_PARSE_RANK)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel++) = D_PARSE_RANK
				matched = true
			}
			case 7
			{
				print "Recording Playback Helper (";
				if (debug_flags & D_PLAYBACK_HELPER)
					print "ON";
				else
					print "OFF";
				")"
				self.hugofixmenu #(sel) = D_PLAYBACK_HELPER
				break
			}
			case else : matched = false
			n++
		}
		self.hugofixmenu #10 = sel
	}
}


! Roody's note: This next section is to organize the object tree when HugoFix
! is on to better distinguish between Hugo "guts" and game objects like rooms
! and their contents.

object replaced_objects
{}

object object_classes
{}

object fuses_and_daemons
{}

routine OrganizeTree
{
	local i
	for (i=2;i<=objects ;i++ )
	{
		if parent(i) = nothing
		{
			local a
#ifclear NO_OBJLIB
				a = female_character ! since female_character is of type character
#endif
			if i.type = i or i = a
				move i to object_classes
			elseif i.type = settings
				move i to settings
#ifclear NO_FUSES
			elseif i.type = fuse,daemon
				move i to fuses_and_daemons
#endif
			elseif not i.name
			{
#ifset _WINDOW_H
				if i.type ~= window_class
#endif
					move i to replaced_objects
			}
		}

	}
}

!\ Roody's note: When I'm testing stuff, I often will code something
in a room's long_desc to print out whether things are true and false.
Here is a helper routine to save people some time.
\!
routine TrueorFalse(arg)
{
	if arg
		print "true";
	else
		print "false";
}

#endif  ! #ifset HUGOFIX


!\ Roody's note: Changed EndGame to make the menu more configurable
  This is the version I had already written that splits EndGame up into
  two routines for both readability and configurability (for adding options
  like (A)musing ).

  As such, I added SpecialKey and SpecialRoutine routines, so people can do
  this:

replace SpecialKey(end_type)
{
   if (end_type = 3) and (word[1] = "s","special")
	{
		return word[1]
	}
   else
		return false
}
 \!

replace EndGame(end_type)
{
	verbroutine = &EndGame     ! something for newmenu and such to check for
	PrintStatusLine                 ! update one last time
	PrintEndGame(end_type)          ! print appropriate ending message

#ifclear NO_VERBS
	local r,n

	RlibMessage(&EndGame, 1,end_type)    ! ask to RESTART, RESTORE, (UNDO), or QUIT

	r = -1
	while r = -1
	{
		GetInput
		n++
		r = ProcessKey(word[1], end_type)
		if r = -1
		{
			if display.needs_repaint and is_fluid_layout is not special
			{
				if RepaintScreen
				{
					InitScreen
					PrintStatusLine
				}
				n = 0
			}
			if word[1] and word[1] = SpecialKey(end_type) or not n
				RlibMessage(&EndGame, 1,end_type)    ! ask to RESTART, RESTORE, (UNDO), or QUIT
			else
				RlibMessage(&EndGame, 2,end_type)    ! ask again (more succinctly)
		}
	}
	return r
#else   ! i.e., #elseif set NO_VERBS
	QuitGameText
	return 0;
#endif
}

! to be replaced by "prettyscreens.h"
routine RepaintScreen
{}

!#ifclear NO_VERBS
! Roody's note: Companion routine to EndGame
routine ProcessKey(entry,end_type)
{
   local r,i
   r = -1
	i = 0
	if not entry
		return -1
   select entry
	! we do SpecialKey first since options like RESTART can clear word[1]
	! and trigger SpecialKey (which sometimes equals false)
		case SpecialKey(end_type)
			SpecialRoutine(end_type)
		case "restart", "r"
		{
#ifclear NO_XVERBS
			word[1] = "yes"
			if (call &DoRestart)
				r = 1
#else
			if restart
				r = 1
#endif
		}
! "restore" is not possible in a game without xverbs as the player
! shouldn't have been able to save
#ifclear NO_XVERBS
		case "restore", "e"
		{
			word[1] = "restore"
			if (call &DoRestore)
				r = 1
		}
#endif
#ifclear NO_UNDO
		case "undo", "u"
		{
			word[1] = "undo"
#ifclear NO_XVERBS
			if (call &DoUndo)
				r = 1
#else
				""
				if not UNDO_OFF
				{
					if undo
					{
						r = true
						UndoResponse
					}
					else
						RLibMessage(&UndoResponse,2)  ! "Unable to UNDO."
				}
				else
					RLibMessage(&UndoResponse,2) ! "UNDO is not currently allowed."
#endif
		}
#endif
		case "quit", "q"
		{
			QuitGameText
			r = 0
		}
   return r
}
!#endif  ! ifclear NO_VERBS

routine SpecialKey(end_type)   ! this routine exists to be replaced
{
	! example SpecialKey code
!	if (word[1] = "amusing","a") and end_type = 1
!		return word[1] ! so it matches the select word[1] text
!	return 0
}

routine SpecialRoutine(end_type)  ! also exists to be replace
{
!	You most likely won't need the end_type argument but it's there in case
!	 you want additional responses besides the winning condition

!	ShowPage(amusing_list) ! example of using newmenu's ShowPage routine
}

! Roody's note: A routine for text to be used both by Endgame and DoQuit.
routine QuitGameText
{
#ifset NO_ACCESSIBILITY
	local cheap
#endif
	""
	RLibMessage(&QuitGameText) ! "Thanks for playing!"
	""
	if not cheap
		display.title_caption = PRESS_ANY_KEY
	if not system(61) ! if not simple port
	{
		print PRESS_ANY_KEY  ! was PRESS_ANY_KEY; but that doesn't show up in DOS
		HiddenPause
	}
}


! Roody's note: Future Boy! replacement by Kent Tessman that omits objects
! held by NPCs unless specically mentioned ("GET ALL FROM FRED")
! Also updated by me to ignore non-clothing items for clothing verbs
replace ExcludeFromAll(obj)
{
	if obj is hidden
		return true

! in most cases, disallow >WEAR ALL / REMOVE ALL
	local takeoff_check, wear_check,verb_check
	if verbroutine =&DoGet and (word[1] = "remove" or
	(word[1] = "take" and word[3] = "off")) and
		xobject is living
#ifset NO_WEARALL
		return true
#else
	{
		verb_check = true
		if obj is worn
			takeoff_check = true
	}
#endif
	elseif verbroutine =&DoGet and (obj is clothing,worn)
		return true
	elseif verbroutine = &DoGet and parent(obj) = player
		return true
#ifclear USE_CHECKHELD
	if verbroutine = &DoTakeOff
	{
		verb_check = true
		if obj is worn
			takeoff_check = true
	}
	elseif verbroutine = &DoWear
	{
		verb_check = true
		if obj is not worn
			wear_check = true
	}
	if verb_check
#else
	if verbroutine = &DoWear_Checkheld
	{
		verb_check = true
		if obj is not worn
			wear_check = true
	}
	elseif verbroutine = &DoTakeOff_Checkheld
	{
		verb_check = true
		if obj is worn
			takeoff_check = true
	}
	if verb_check
#endif  ! USE_CHECKHELD
	{
#ifclear NO_WEARALL
! Make >WEAR ALL and >REMOVE ALL only acknowledge clothes
		if obj is not clothing or (obj is clothing and
		not (takeoff_check or wear_check))
#endif
			return true
	}

	! Exclude things NPCs are carrying unless the NPC is explicitly
	! given as the parent
	if parent(obj) is living
	{
		if IsPossibleXobject(parent(obj)) or parent(obj) = player
			return false
		return true
	}
	return false
}

#ifclear NO_DISAMB_HELP
#if undefined disamb_max
constant DISAMB_MAX 3
#endif

property disamb_suspects alias misc
property disamb_total alias each_turn

object disamb_holder
{
	in settings
	disamb_total 0
	disamb_suspects #DISAMB_MAX
}
#endif  ! NO_DISAMB_HELP

! Roody's note: FindObject tweaked to get rid of jumps, just because
! Added some new checkheld code
replace FindObject(obj, objloc, recurse)
{
	local a, p
	local this_parse_rank
	local found_result = true
	local FindObjectIsFound

	parser_data[PARSER_STATUS] |= FINDOBJECT_CALLED

	if obj = nothing or obj = player
	{
		if obj = nothing and not recurse
		{
#ifset DEBUG
			if debug_flags & D_FINDOBJECT
				print "[Resetting FindObject]"
			if debug_flags & D_PARSE_RANK
				print "[parser_data[BEST_PARSE_RANK] = 0]"
#endif
			parser_data[BEST_PARSE_RANK] = 0
			parser_data[PARSE_RANK_TESTS] = 0
			parser_data[PARSER_STATUS] &= ~FINDOBJECT_KNOWN
			parser_data[PARSER_STATUS] &= ~FINDOBJECT_NONLIVING
		}
		elseif obj = player
		{
			this_parse_rank = obj.parse_rank
			if this_parse_rank > parser_data[BEST_PARSE_RANK]
				parser_data[BEST_PARSE_RANK] = this_parse_rank
		}

		! nothing and player are always available
		obj is known
		return true
	}
#ifclear NO_FUSES
	elseif obj.type = fuse or obj.type = daemon
	{
		! Optimize checking of simple fuses and daemons:
		if obj.#in_scope = 1 and not &obj.in_scope and not obj.#found_in and not &obj.found_in
		{
			if obj is active and obj.in_scope = player
				return true
			else
				return false
		}
	}
#endif

	if not recurse and parser_data[PARSER_STATUS] & PARSER_ACTIVE
	{
		if FailsAllChecks(obj,objloc)
			return false

	! Do some parse_rank tweaking to rank lower objects which (probably) don't
	! apply to a particular command, like trying to open something that's
	! already open:
		this_parse_rank = obj.parse_rank

	! And be ready to prefer the last specifically referred to object in
	! the event of disambiguation
		if obj = parser_data[LAST_SINGLE_OBJECT]
			this_parse_rank++

#ifclear NO_VERBS
		this_parse_rank = CheckParseRank(obj, this_parse_rank)

#ifset USE_CHECKHELD
		if checkheld is active
		{
	! Roody's note: I rewrote this section so it only calls one
	! routine
!\
			if DismissUnheldItems(obj,objloc)
				return false
			elseif PrioritizeHeldItems(obj)
				this_parse_rank += 100 \!

		! if the command doesn't use "all" (>DROP ALL), no parsing
		! help is needed
			if not (checkheld is not plural or obj is checkheld_flag)
			{
				if CheckHeld_Verb_Check
				{
#ifset DEBUG
	
					if debug_flags & D_FINDOBJECT
					{
						print "[FindObject("; obj.name; " ["; number obj; "], "; \
							objloc.name; " ["; number objloc; "]):  "; \
							"false (not checkheld_flag)]"
					}
#endif ! DEBUG
					return false
				}
			}
		! but if all IS used and the verb implies held items,
		! we'll give held items a parser boost
			if not (checkheld is plural or obj is not checkheld_flag)
			{
				if CheckHeld_Verb_Check
					this_parse_rank += 100
			}
		}
#endif	! USE_CHECKHELD
#endif	! #ifclear NO_VERBS
	}

	p = parent(obj)  ! find the parent of the object
#ifset _OBJLIB_H
#ifset USE_PLURAL_OBJECTS
	if p = plural_class, identical_class
		p = 0
#endif
#endif
	! The objloc argument is equal to 0 if a) the grammar token is
	! "anything", or b) the engine is re-testing object availability
	! given the setting of parser_data[BEST_PARSE_RANK] during the first
	! pass of object disambiguation.

	if objloc = 0 and parser_data[PARSER_STATUS] & PARSER_ACTIVE
	{
		if location and obj is not known
		{
			if FindObject(obj, location, true)
				obj is known
		}

		if not ObjectIsKnown(obj)
		{
#ifset DEBUG
			if debug_flags & D_FINDOBJECT and not recurse
			{
				print "[FindObject("; obj.name; " ["; number obj; "], "; \
					objloc.name; " ["; number objloc; "]):  "; \
					"false (object not 'known')]"
			}
#endif
			return false
		}

		if not recurse
		{
			if not AnythingTokenCheck(obj,objloc)
				return false
			elseif this_parse_rank < parser_data[BEST_PARSE_RANK]
			{
#ifset DEBUG
				if debug_flags & D_FINDOBJECT
				{
					print "[FindObject("; obj.name; " ["; number obj; "], "; \
						objloc.name; " ["; number objloc; "]):  "; \
						"false (this_parse_rank = "; number this_parse_rank; \
						" < parser_data[BEST_PARSE_RANK] = "; number parser_data[BEST_PARSE_RANK]; ")]"
				}
#endif
				return false
			}
		}
		FindObjectisFound = true
	}
	elseif obj = objloc or
		(player in obj and obj ~= objloc and (obj~=location or not recurse)) or
		p = obj or p = objloc or p = player or p = parent(player)
	{
		obj is known
		FindObjectIsFound = true
	}
	elseif p  ! does obj have a parent?
	{
		select FindObjectContainerCheck(obj,objloc,p)
			case 1
			{
				obj is known
				FindObjectIsFound = true
			}
			case -1
			{
				obj is known
				found_result = 2
				FindObjectIsFound = true
			}
	}

	if obj.#found_in and not FindObjectIsFound
	{
		for (a=1; a<=obj.#found_in; a++)
		{
			if obj.found_in #a and (obj.found_in #a = objloc or
				FindObject(obj.found_in #a, objloc, true))
			{
				obj is known
				FindObjectIsFound = true
				break
			}
		}
	}

	if obj.#in_scope and not FindObjectIsFound
	{
		for (a=1; a<=obj.#in_scope; a++)
		{
			if obj.in_scope #a
			{
				if obj.in_scope #a=objloc or obj.in_scope #a=actor
					FindObjectIsFound = true
				elseif FindObject(obj.in_scope #a, objloc, true)
					FindObjectIsFound = true
				if FindObjectIsFound
				{
					obj is known
					break
				}
			}
		}
	}
	if parser_data[PARSER_STATUS] & PARSER_ACTIVE and not recurse
		MakeParserNotes(obj, FindObjectIsFound)
	if not FindObjectIsFound
	{
#ifset DEBUG
		if debug_flags & D_FINDOBJECT and not recurse
		{
			print "[FindObject("; obj.name; " ["; number obj; "], "; \
				objloc.name; " ["; number objloc; "]):  "; \
				"false]"
		}
#endif
		return false
	}

! FindObjectIsFound area
	if not recurse and parser_data[PARSER_STATUS] & PARSER_ACTIVE
	{
		local new_disamb
		if parser_data[PARSE_RANK_TESTS]++ = 0
		{
#ifset DEBUG
			if debug_flags & D_PARSE_RANK and this_parse_rank > parser_data[BEST_PARSE_RANK]
			{
				print "[parser_data[BEST_PARSE_RANK] = "; number this_parse_rank; "]"
			}
#endif
			parser_data[BEST_PARSE_RANK] = this_parse_rank
			new_disamb = true
		}
		elseif this_parse_rank > parser_data[BEST_PARSE_RANK]
		{
#ifset DEBUG
			if debug_flags & D_PARSE_RANK and this_parse_rank > parser_data[BEST_PARSE_RANK]
			{
				print "[parser_data[BEST_PARSE_RANK] = "; number this_parse_rank; "]"
			}
#endif
			parser_data[BEST_PARSE_RANK] = this_parse_rank
!			new_disamb = true
		}
#ifclear NO_DISAMB_HELP
	if not objloc or (new_disamb and disamb_holder.disamb_suspects)
		FillDisambHolder(obj,new_disamb,this_parse_rank)
#endif
	}
#ifset DEBUG
	if debug_flags & D_FINDOBJECT and not recurse
	{
		print "[FindObject("; obj.name; " ["; number obj; "], "; \
			objloc.name; " ["; number objloc; "]):  "; \
			"true]"
	}
#endif
	return found_result
}

routine FindObjectContainerCheck(obj,objloc,p)
{
	if (p is not openable or p is platform) and p is not quiet
	{
		if FindObject(p, objloc, true) and ObjectisKnown(p)
		{
			return 1
		}
	}
	elseif p is openable and p is open and p is not quiet
	{
		if FindObject(p, objloc, true) and ObjectisKnown(p)
		{
			return 1
		}
	}
	elseif p is transparent, not quiet
	! (p is openable, not open, transparent, not quiet)
	{
		if FindObject(p, objloc, true) and ObjectisKnown(p)
		{
			return -1
		}
	}
}

! Roody's note: MakeParserNotes adds some values to PARSER_STATUS.
! Some are from the original FindObject, some are new. Mainly, they
! affect how some things are handled in ParseError.

routine MakeParserNotes(obj,is_found)
{
	! If any object FindObject is attempting to disambiguate
	! is known, make a note of it (if during the parsing phase)
	if obj is not living
		parser_data[PARSER_STATUS] |= FINDOBJECT_NONLIVING
	else
	{
		parser_data[PARSER_STATUS] |= FINDOBJECT_LIVING
		last_actor = obj
	}
	if ObjectisKnown(obj)
		parser_data[PARSER_STATUS] |= FINDOBJECT_KNOWN
	if is_found
		parser_data[PARSER_STATUS] |= FINDOBJECT_FOUND
}

! Roody's note: FillDisambHolder makes "which <blank> did you mean,
! the 1) <first blank> or 2) <second blank>?" messages possible.
! This routine fills the disam_holder object with the possible
! suspects.

#ifclear NO_DISAMB_HELP
routine FillDisambHolder(obj,reset,this_parse_rank)
{
	local x
	if not reset
	{
		x = AddPropValue(disamb_holder, disamb_suspects , obj,true)
		if x > disamb_holder.disamb_total
		{
#ifset DEBUG
			if debug_flags & D_PARSE_RANK
				print "["; obj.name;" added to disamb_holder suspects with
				parse rank "; number this_parse_rank;".]"
#endif ! DEBUG
			disamb_holder is special
			disamb_holder.disamb_total = disamb_holder.disamb_total + 1
			return true
		}
		elseif not x
		{
#ifset DEBUG
			if debug_flags & D_PARSE_RANK
				print "[unable to add "; obj.name;" to disamb_holder
				suspects. disamb_holder deactivated.]"
#endif ! DEBUG
			disamb_holder is not special
			return
		}
	}
	x = 1 ! 2
#ifset DEBUG
	if debug_flags & D_PARSE_RANK
		print "[disamb_holder suspects cleared.]"
#endif ! DEBUG
	disamb_holder.disamb_total = 0 ! 1
!	disamb_holder.disamb_suspects = obj
	while (x <= disamb_holder.#disamb_suspects and
	disamb_holder.disamb_suspects #x)
	{
		disamb_holder.disamb_suspects #x = 0
		x++
	}
	disamb_holder is not special
}

object parse_disamb
{
	in preparse_instructions
	type settings
	execute
	{
		local a, n, w
		if disamb_holder is not special
			return false
		if words = 1
		{
			if disamb_holder.disamb_total = 2
			{
				select word[1]
					case "former":a = 1
					case "latter":a = 2
			}
			select word[1]
				case "1","first": a = 1
				case "2","second": a = 2
				case "3", "third": a = 3
				case "4", "fourth": a = 4
				case "5", "fifth": a = 5
			if a and a <= disamb_holder.disamb_total
			{
				parser_data[LAST_SINGLE_OBJECT] = disamb_holder.disamb_suspects #a
				RestoreFromOldWord
				if actor ~= player
				{
					InsertWord(1,2)
					word[2] = "~and"
!					if last_actor
!						word[1] = last_actor.noun
!					else
					word[1] = actor.noun
				}
				return true
			}
		}
		return false
	}
}
#endif  ! NO_DISAMB_HELP

! Roody's note: FailsAllChecks is called by commands with "all" in
! it.

routine FailsAllChecks(obj,objloc)
{
	local a
	if not (word[2] = "~all" or word[3] = "~all")
		return
	if not IsPossibleXobject(obj) or (xobject and xobject ~= obj)
	{
		a = obj.exclude_from_all
		if a < 2
		{
			if a or ExcludeFromAll(obj) = true
			{
#ifset DEBUG
				if debug_flags & D_FINDOBJECT
				{
					print "[FindObject("; obj.name; " ["; number obj; "], "; \
						objloc.name; " ["; number objloc; "]):  "; \
						"false (excluded from \"all\")]"
				}
#endif ! DEBUG
				return true
			}
		}
	}
}

! Roody's note: CheckParseRank puts all of FindObject's parse rank
! modifying code into one routine. Replace this if you have other
! verb routines to add or tweak.

routine CheckParseRank(obj, this_parse_rank)
{
	select verbroutine
	case &DoOpen
	{
		if obj is openable and obj is open
			this_parse_rank--
	}
	case &DoClose
	{
		if obj is openable and obj is not open
			this_parse_rank--
	}
	case &DoSwitchOn
	{
		if obj is switchable and obj is switchedon
			this_parse_rank--
	}
	case &DoSwitchOff
	{
		if obj is switchable and obj is not switchedon
			this_parse_rank--
	}
#ifset USE_CHECKHELD
	case &DoWear_Checkheld
	{
		if obj is clothing and obj is worn
			this_parse_rank--
	}
	case &DoTakeOff_CheckHeld
	{
		if obj is clothing and obj is not worn
			this_parse_rank--
	}
#else  ! if clear USE_CHECKHELD
	case &DoWear
	{
		if obj is clothing and obj is worn
			this_parse_rank--
	}
	case &DoTakeOff
	{
		if obj is clothing and obj is not worn
			this_parse_rank--
	}
#endif !  USE_CHECKHELD

	return this_parse_rank
}

! Roody's note: When using things like the Routine Grammar Helper, if
! your game has multiple items with similar names, disambiguation messages
! can pop up for items not even in the room (since we're using the anything
! grammar token). AnythingTokenCheck exists to make sure those objects aren't
! even considered. You may need to replace and adapt this if you write any
! code that uses the grammar helper.

routine AnythingTokenCheck(obj,objloc)
{
	local failed_check
	select verbroutine
#ifclear NO_VERBS
		case &DoEmpty ! should only be called if NEW_EMPTY is being used
		{
			if not parent(obj) or not FindObject(parent(obj),location,true)
				failed_check = true
#ifset DEBUG
				if debug_flags & D_FINDOBJECT
				{
					print "[FindObject("; obj.name; " ["; number obj; "], "; \
					objloc.name; " ["; number objloc; "]):  "; \
					"false (object not in scope)]"
				}
#endif  ! DEBUG
		}
		case &DoGet
		{
			if obj in player
			{
#ifset DEBUG
				if debug_flags & D_FINDOBJECT
				{
					print "[FindObject("; obj.name; " ["; number obj; "], "; \
						objloc.name; " ["; number objloc; "]):  "; \
						"false (object in player)]"
				}
#endif  ! DEBUG
				failed_check = true
			}
		}
#endif ! ifclear NO_VERBS
		case else: failed_check = false
	return (not failed_check) ! return false if it failed
}

#ifset USE_CHECKHELD
!\
!  I decided that I did not need these two routines after all.

! Roody's note: DismissUnheldItems is called when a command uses
! "all" and the USE_CHECKHELD system is turned on.
routine DismissUnheldItems(obj,objloc)
{
	! checkheld is plural if the command uses "all", like in
	!  >DROP ALL .   checkheld has checkheld_flag if the command
	!  refers to a single item
	if checkheld is not plural or obj is checkheld_flag
		return
	elseif CheckHeld_Verb_Check
	{
#ifset DEBUG
		if debug_flags & D_FINDOBJECT
		{
			print "[FindObject("; obj.name; " ["; number obj; "], "; \
				objloc.name; " ["; number objloc; "]):  "; \
				"false (not checkheld_flag)]"
		}
#endif ! DEBUG
		return true
	}
}

! Roody's note: PrioritizeHeldItems is called when a command uses
! "all" and the USE_CHECKHELD system is turned on.
routine PrioritizeHeldItems(obj)
{
	! checkheld is plural if the command uses "all", like in
	!  >DROP ALL .   checkheld has checkheld_flag if the command
	!  refers to a single item
	if checkheld is plural or obj is not checkheld_flag
		return
	elseif CheckHeld_Verb_Check
		return true
}
\!

! The new code in this routine makes it so items must be held for >WEAR ALL or
! >REMOVE ALL to work
! If your game has some new verbs that require an item to be held,
! you can replace this and add the "checkheld" version of your verb
! routine as well.
routine CheckHeld_Verb_Check
{
	if (verbroutine = &DoDrop_CheckHeld, &DoPutonGround_CheckHeld) or
		(verbroutine = &DoPutIn_CheckHeld and xobject) or
		(verbroutine = &DoTakeOff_CheckHeld, &DoWear_Checkheld)
	return true
}
#endif ! USE_CHECKHOLD

!\ Roody's note: This version of HoursMinutes should work for several
"days" (up to 22) \!
replace HoursMinutes(val)
{
	local hours, minutes, military

	hours = val / 60
	minutes = val - hours * 60
	military = (STATUSTYPE & MILITARY_TIME)

	if not military
	{
		while hours > 12
		{ hours -= 12 }
		if hours = 0:  hours = 12
	}
	else
	{
		while hours >= 24
		{
			hours -= 24
		}
		if hours < 10:  print "0";
	}

	print number hours;
#ifset NO_MILITARY_COLON
	if not military
#endif
	print ":";
	if minutes < 10
		print "0";
	print number minutes; " ";
	if not military
	{
		while val > 1440
		{
			val -= 1440
		}
		if val >= 720
			print "p.m.";
		else
			print "a.m.";
	}
#ifset NO_MILITARY_COLON
	else
		print "hours";
#endif
}

! Roody's note: this version doesn't reset event_flag when it's set to 2 and
! also gets rid of jump, also moves PrintStatusLine to end of routine
replace MovePlayer(loc, silent, ignore)
{
	local v, obj, xobj, act, ret, LeavingMovePlayer,real_loc

#ifclear NO_OBJLIB
	if loc.type = direction
	{
		local l
		l = location.(loc.dir_to)
		if l > 1                ! since a message always returns 1
			loc = l
		else
			return
	}
#endif
	if parent(loc) = 0
		real_loc = loc
	else
		real_loc = parent(loc) ! no need to check deeper as the default Hugo
		                       ! library doesn't really support being inside
		                       ! an object within an object

#ifset USE_ATTACHABLES
	if ObjectisAttached(player, location, real_loc)
		return
#endif

! Check if there's a before routine for MovePlayer in the new
! location.  Note that the (potential) new location and the old
! location are stored in object and xobject, respectively.
!
	v = verbroutine
	obj = object
	xobj = xobject
	act = actor
	verbroutine = &MovePlayer
	object = real_loc
	xobject = location
	actor = player
	if not ignore
	{
		ret = BeforeRoutines
		if ret : LeavingMovePlayer = true
	}

	if (not LeavingMovePlayer)
	{
		move player to loc
		old_location = location
		location = real_loc

#ifset USE_ATTACHABLES
		MoveAllAttachables(player, old_location, location)
#endif
	}

! LeavingMovePlayer area

	if not ret
	{
		local lig
		lig = FindLight(location)
		if not lig
			DarkWarning
		elseif not silent
		{
			DescribePlace(location)
			if not event_flag
				event_flag = true
		}

	! Check if there's an after routine for MovePlayer in the new
	! location:
	!
		if not ignore
			ret = AfterRoutines
		if lig and not silent
		{
#ifset NEW_ROOMS
			local firsttime
			firsttime = location is not visited
			if not location.first_visit or firsttime
				location.first_visit = counter + 1
			location.first_visit #2 = counter + 1
#endif
			location is visited
		}
	}
	verbroutine = v
	object = obj
	xobject = xobj
	actor = act

	return ret
}

! some new globals mainly used by SpeakTo, although last_actor has
! some additional uses
global last_actor
global last_verbroutine
global previous_object
global previous_xobject

! These constants originally were used for a NEW_PARSE global but now is
! used by parser_data[LAST_PARSER_STATUS], an existing but not-utilized
! parser thing in hugolib
enumerate start = 1, step *2
{
	PARSE$_F                ! was there a parse$ string in last command?
	                        ! 2 is reserved by PRONOUNS_SET
	MULTICOMMAND_F = 4      ! is a multiple-command line being interpreted?
	LAST_TURN_TRUE_F        ! did the last turn return true?
	HIDE_COMMAND_F          ! set this if you want to temporarily hide
	                        ! commands when SHOW_COMMANDS is set
}

! Roody's note: Took out Parse's HugoFix code, as it tries to print parse$
! before parse$ is set also got rid of jump
replace Parse
{
	local a
	local reparse
	local JumpToEnd
	local not_character_order
	local command_words

	ResetParse

	list_nest = 0
	event_flag = 0

	if nothing ~= 0                   ! best place to check this
		print "[WARNING:  Objects/classes defined before library]"

#ifset DEBUG
	if debug_flags & D_PARSE_RANK
		print "[parser_data[BEST_PARSE_RANK] = 0]"
#endif

	if word[1] = "~oops"
		return
	! save the last turn's stuff which will be used if the current command is
	! >CHARACTER, G (AGAIN)
	previous_object = object
	previous_xobject = xobject
	last_verbroutine = verbroutine

	! Allow the player object to override the parsing cycle completely
	! (moved to its own routine to lighten the local variable burden)
	if PlayerPreParseOverride
		return false

	if PreParseInstructions: reparse = true
	if PreParse:  reparse = true            ! easily replaceable routine

! The following, devised by Jim Newland, checks to see if the player
! input refers to an unnecessary item of scenery (for example) which is
! nonetheless mentioned in the location description.

	! figure out the words in the current command
	command_words = CurrentCommandWords

	local error
	if VerbCheck(word[1]) and word[1] ~= "~and"
		not_character_order = true
	elseif (word[1] = "him","her","it","them") and word[1] = "~and"
	{
		!  > HIM, DO THING
		ParseError(6)  ! "That doesn't make any sense."
		error = true
	}
	for (a=2; a<=command_words; a++)
	{
		if VerbCheck(word[a]) and word[a] ~= "~and"
			not_character_order = true
		if word[a] = "then"  ! Allow for "then..." and "and then..."
		{
			! end of this command
			word[a] = ""
			command_words = a - 1
			if word[(a-1)] = "~and"
			{
				DeleteWord(a-1)
				command_words--
			}
			reparse = true
			break
		}
		elseif Inlist(player, extra_scenery, word[a])
		{
			Message(&Parse, 1)
			error = true
		}
		elseif Inlist(location, extra_scenery, word[a])
		{
			Message(&Parse, 1)
			error = true
		}
		elseif word[a] = "~oops"
		{
			ParseError(6) ! "That doesn't make any sense."
			error = true
		}
		elseif word[a] = "~and" and VerbCheck(word[a+1])
		{
			if word[(a+1)] = "~and"
			{
				DeleteWord((a+1),1)
				command_words--
			}
			elseif not_character_order
			{
				word[a] = "then"
				a--
			}
			else
			{
				! pass the phrase through the command parsing routine
				if OrdersPreParse((a+1),command_words)
					reparse = true
				! and then check for commanding the NPC to do an xverb
				if XVerbCheck(word[a+1])
				{
					local safe
					if last_actor and last_actor ~= player and
						(word[(a+1)] = "g" or word[a+1] = "again")
					{
						if ObjWord(word[(a-1)], last_actor) !  = noun
							safe = true
					}
					if not safe
					{
						ParseError(6) ! "That doesn't make any sense."
						error = true
					}
				}
			}
		}
		if error
		{
			word[1] = ""            ! force ParseError(0)
			words = 0
			customerror_flag = true
			return true
		}
	}

	! Last player-specified object
	if object > 0 and object < objects and
		not (parser_data[LAST_PARSER_STATUS] & PRONOUNS_SET)
		AssignPronoun(object)

	! Must do this after AssignPronoun, and reset it to 0
	! after both of Perform and ParseError:
	parser_data[PARSER_STATUS] = PARSER_ACTIVE

!  MultiCommandInstructions returns false if a character is given an xverb
!  command
	local x
	x = MultiCommandInstructions(command_words)
	if not x
	{
		ParseError(6) ! "That doesn't make any sense."
		word[1] = ""            ! force ParseError(0)
		words = 0
		customerror_flag = true
		return true
	}
	! since MultiCommandInstructions calls OrdersPreparse, it returns 2 if
	! that returned true
	elseif x = 2
		reparse = true

	! To repeat last command
	if (word[1] = "again" or word[1] = "g") and word[2] = ""
	{
		if (parser_data[LAST_PARSER_STATUS] & PARSE$_F)
		{
			parser_data[LAST_PARSER_STATUS] &= ~PARSE$_F
			RLibMessage(&Parse,1) ! "You will need to type that in again."
			word[1] = ""            ! force ParseError(0)
			words = 0
			customerror_flag = true
			return true
		}
		RestoreFromOldWord(0,1) ! restore one command
		if last_actor and last_actor ~= player and
			parser_data[PARSER_STATUS] & FIRSTCOMMAND_F
		{
			if word[1]
			{
				InsertWord(1,2)
				word[2] = "~and"
			}
			word[1] = last_actor.noun
		}
		if not oldword[0]
			words = 0
		else
			command_words = CurrentCommandWords
		reparse = true
		JumpToEnd = true
	}

#ifclear NO_UNDO
	if not (word[1] = "undo" and word[2] = "")
#endif
		parser_data[LAST_PARSER_STATUS] &= ~PARSE$_F
	if not JumpToEnd
	{
		local count, n, number_pronouns
		for (count=2; count<=words and word[count]~=""; count++)
		{
			select word[count]
				! Rename pronouns to appropriate objects
				case "it", "them", "him", "her"
				{
					select word[count]
						case "it"
						{
						! Check if there were multiple objects in the last command
						! and apply "it" to most applicable
							if xobject
								ParsePronounFix(count)
							a = it_obj
						}
						case "them":    a = them_obj
						case "him":     a = him_obj
						case "her":     a = her_obj

					if a = nothing
					{
						! "...be a little more specific"
						ParseError(13)
						words = 0
						customerror_flag = true
						return
					}
					! Avoid, e.g., "put it it something"
					elseif word[count] ~= word[count+1]
					{
						! For her_obj, we can run into trouble, since
						! it's both possessive and a pronoun, so only
						! replace "her" where it's the last word:
						if a ~= her_obj or
							(a = her_obj and (count = 2 or count = words))
						{
!   Maybe this is a bad idea, but I was hoping this next bit might
!   help with repeated ASK/TELL about other characters, like:
!   >TELL BOB ABOUT FRED.  >ASK BOB ABOUT HIM
							if count ~= 2 and count = words and
							(object is living and
							(word[count] = "her" and xobject is female) or
							(word[count] = "him" and xobject.pronoun #2 = "him"))
							{
								if ObjWord(word[2],object)
									a = xobject
							}
							n = SetObjWord(count, a)
							replace_pronoun[number_pronouns] = a
							number_pronouns++
							if n > 1
								count = count + (n - 1)
							parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
							reparse = true
						}
					}
				}
		}

		if number_pronouns = 2 and replace_pronoun[0] = replace_pronoun[1]
			number_pronouns--

		if number_pronouns > 0
			PrintReplacedPronouns(number_pronouns)

#ifset USE_PLURAL_OBJECTS
		if ParsePluralObjects:  reparse = true
#endif

#ifset USE_CHECKHELD
		ParseCheckHeld
#endif
	}
	! old "LeaveParse area

#ifset DEBUG
	if debug_flags & D_PARSE
	{
		local w
		Font(BOLD_ON)
		for (w=1; w<=words and word[w]~=""; w++)
		{
			print "[";
			if (word[w] ~= -1)
				print word[w];
			else
				print "\Iparse$\i";
			"] ";
		}
		print newline
		Font(BOLD_OFF)
	}
#endif
	return reparse
}

routine ParsePronounFix(count)
{
	local i
	select word[(count-1)]
		case "open","close": i = openable
		case "lock","unlock" : i = lockable
		case "wear" : i = clothing
		case "read","peruse" : i = readable
		case "activate","start","stop","deactivate" : i = switchable
		case "turn", "switch"
		{
			if word[(count+1)] = "off","on"
				i = switchable
		}
	if i and object is not i and it_obj = object
	{
		if xobject is i
			it_obj = xobject
	}
	elseif i and xobject is not i and it_obj = xobject
	{
		if object is i
			it_obj = object
	}
}

replace ResetParse
{
	customerror_flag = false
	parser_data[BEST_PARSE_RANK] = 0  ! reset each parser cycle
	parser_data[PARSE_RANK_TESTS] = 0
	parser_data[VERB_IS_XVERB] = 0
}

routine VerbCheck(a)
{
	if a < "nothing" ! nothing.name
		return true
	else
		return false
}

! Roody's note: Since commands to characters get sent straight to SpeakTo
! when parsed, XverbCheck exists to try to stop xverb commands from
! successfully being passed on. A game with additional xverbs may need to
! replace this and add more.

routine XverbCheck(a)
{
	local r
	select a
		case "load","restore","resume","save","suspend","script", \
		"transcript","transcription","quit","q","restart","brief", \
		"normal","superbrief","verbose","short","record","playback", \
		"long","display","wide","tall","score","undo", "g", "again", \
		"version", "about", "hints", "hint"
			r=1

	return r
}

routine MultiCommandInstructions(n) ! n is CurrentCommandWords
{
	if not (parser_data[LAST_PARSER_STATUS] & LAST_TURN_TRUE_F) ! last_turn_true
		parser_data[LAST_PARSER_STATUS] &= ~MULTICOMMAND_F !multicommand = false
	parser_data[LAST_PARSER_STATUS] &= ~LAST_TURN_TRUE_F
	if not (parser_data[LAST_PARSER_STATUS] & MULTICOMMAND_F) ! multicommand
	{
		parser_data[PARSER_STATUS] |= FIRSTCOMMAND_F
		if n < words
			parser_data[LAST_PARSER_STATUS] |= MULTICOMMAND_F ! multicommand = true
	}
	else
	{
#ifset SHOW_COMMANDS
		if PrintCurrentCommand(true)
			""
#endif
		if n = words
			parser_data[LAST_PARSER_STATUS] &= ~MULTICOMMAND_F ! multicommand off
		if last_actor and last_actor ~= player
		{
			if not (word[1] = "g","again")
			{
				if OrdersPreparse(1,n)
					return 2
				elseif XverbCheck(word[1])
				{
					parser_data[LAST_PARSER_STATUS] &= ~MULTICOMMAND_F ! multi off
					return false
				}
			}
		}
	}
	return true
}

! Roody's note: OrdersPreParse is a PreParse routine for orders given to
! characters.  Since the beginning of the relevant command often won't be
! word[1], b and e are the numbers of the word array where the command
! begins and ends, respectively.

routine OrdersPreParse(b,e)
{}

routine PrintCurrentCommand(print_prompt)
{
	if (parser_data[LAST_PARSER_STATUS] & HIDE_COMMAND_F)
		return false
	FONT(ITALIC_ON)
	if print_prompt
		print prompt;
	if last_actor and last_actor ~= player and (word[1] ~= "g","again")
		print last_actor.name;", ";
	local a = 1
	while not (word[a] = "" or word[a] = "then")
	{
		select word[a]
			case -1 : print "\"";parse$;"\"";
			case "~all" : print "all";
			case "~any" : print "any";
			case "~and"
			{
				if word[a-1] = "~and" or not VerbCheck(word[a+1])
					print " and";
				else
					print ",";
			}
			case "~except" : print "except";
			case "~oops" : print "oops";  ! hopefully never called
			case "l"
			{
				if a = 1 or word[a-1] = "~and"
					print "look";
			}
			case "x"
			{
				if a = 1 or word[a-1] = "~and"
					print "examine";
			}
			case "g" : print "again";
			case else : print word[a];
		if word[a+1] ~= "","~and"
			print " ";
		a++
	}
	FONT(ITALIC_OFF)
	return true
}

routine SaveOldWord
{
	local ow = 1
	if word[1] = "g", "again"
		return
	while ow <= words
	{
		if word[ow] = -1
			parser_data[LAST_PARSER_STATUS] |= PARSE$_F
		oldword[ow] = word[ow]
		ow++
	}
	oldword[0] = words
}

routine RestoreFromOldWord(partial, single_command)
{
	local a, n = 1
	if single_command
	{
		while oldword[(n+1)] ~= "" and n < oldword[0]
		{
			n++
		}
		if n > 1
			InsertWord(1, (n-1))
	}
	else
	{
		words = oldword[0] - partial
		n = words
	}
	for (a=1; a<=n; a++)
	{
		word[a] = oldword[(partial + a)]
	}
}

! Roody's note: Now prints parse$ if parser monitoring is on
! and various other little fixes suggested by Mike Snyder
replace ParseError(errornumber, obj)
{
	if is_fluid_layout is not special
		PrintStatusLine ! redraw PrintStatusLine in case screen size changed
	if word[1] = "*"
	{
		if betalib is special
			RLibMessage(&DoScriptOnOff, 4) ! Comment recorded!
		else
			RLibMessage(&DoScriptOnOff, 5) ! Comment not recorded!
		return
	}
#ifset DEBUG
	if debug_flags & D_PARSE
	{
		local w, p
		Font(BOLD_ON)
		if (errornumber = 0,1,16,17) ! cases where Parse hasn't been called
		{
			for (w=1; w<=words and word[w]~=""; w++)
			{
				print "[";
				if (word[w] ~= -1)
					print word[w];
				else
				{
					print "\Iparse$";
					if p
						print number (p+1);"*";
					p++
					FONT(ITALIC_OFF)
				}
				"] ";
			}
			print newline
			if (p>1)
				"(* Not an \Iactual\i referable token.)"
		}
		print "[parse$ = \""; parse$; "\"]"
		print "[ParseError errornumber = "; number errornumber; "]"
		print "[ParseError object = "; obj.name; "]"
		Font(BOLD_OFF)
	}
#endif

	if errornumber >= 100
	{
		CustomError(errornumber, obj)
		word[1] = ""            ! force ParseError(0)
		words = 0
		last_actor = 0
		customerror_flag = true
		parser_data[PARSER_STATUS] = PARSER_RESET
		return true
	}
	if customerror_flag
	{
		last_actor = 0
		customerror_flag = 0    ! CustomError already printed error
		parser_data[PARSER_STATUS] = PARSER_RESET
		return true
	}
	local r
	r = NewParseError(errornumber, obj)
	if r
	{
		parser_data[PARSER_STATUS] = PARSER_RESET
		return r
	}

	select errornumber
		case 1
		{
			print "You";
			MatchPlural(player, "doesn't", "don't")
			print	" need to use the word \"";
			if obj
				print obj;
			else
				print parse$;
#ifset AMERICAN_ENGLISH
				".\""
#else
				"\"."
#endif
		}
		case 2
		{
			if not (parser_data[PARSER_STATUS] & FINDOBJECT_CALLED)
			{
				if XverbCheck(word[1])
					return ParseError(1,word[1])
				else
					errornumber = 6
			}
			elseif actor = player and
				(parser_data[PARSER_STATUS] & FINDOBJECT_LIVING)
			{
				obj = last_actor
				errornumber = 11
			}
			elseif not (parser_data[PARSER_STATUS] & FINDOBJECT_FOUND)
					errornumber = 11
			else
			{
				r = 1
				while word[r] ~= ""
				{
					if word[r] = "~and","former","latter"
					{
						errornumber = 6
						break
					}
					r++
				}
				if errornumber ~= 6
					"Better start with a verb."
			}
		}
		case 3
		{
			local a, b
			 select word[1]
				case "x"
				{
					a = "examine"
					if word[2] ~= "~all"
						b = word[2]
				}
				case "l","look"
				{
					if word[2] = "~all"
						a = "look at"
					else
						a = "look"
					if word[2] ~= "~all"
						b = word[2]
				}
				case "sit","lie","go","walk","watch"
				{
					a = word[1]
					if word[2] ~= "~all"
						b = word[2]
				}
				case "take"
				{
					if word[2] = "off" or word[3] = "off"
					{
						"Please specify which item you'd like to take off."
						return true
					}
					else
						a = word[1]
				}
				case "remove","wear"
				{
					print "Please specify which item you'd like to "; word[1];"."
					return true
				}
				case "get", "step"
				{
					a = word[1]
					if word[2] ~= "~all"
					{
						if (word[2] = "outof")
							b = "out of"
						elseif word[2] = "offof"
							b = "off of"
						else
							b = word[2]
					}
				}
				case else : a = parse$
			print CThe(actor); " can't "; a;
			if b
				print " "; b ;
			" multiple objects like that."
		}

		case 5
		{
			if VerbWord = "give", "offer", "hand", "show"
			{
				print "Not sure what you're referring to--try \""; \
					VerbWord; " (something) \Ito\i (someone)";
	#ifset AMERICAN_ENGLISH
					".\""
	#else
					"\"."
	#endif
			}
			else
			{
!\
				print CThe(player); \
					MatchPlural(player, "hasn't", "haven't");
					print " seen any";
						print " \""; parse$; "\"";
					print ", nor"; IsorAre(player, true); " "; The(player); \
					" likely to, even if such a thing exists."
\!
				print CThe(player); \
					MatchPlural(player, "hasn't", "haven't");
					print " encountered any";
				if parser_data[LAST_PARSER_STATUS] & PRONOUNS_SET
					print "thing like that";
				else
					print " \""; parse$; "\"";
				print ".";
				print "  (If you're sure you need to refer to that, try
					putting it another way.)"
			}
		}
		case 6
			print "That doesn't make any sense."
#ifclear NO_DISAMB_HELP
		case 8
		{
			if disamb_holder is not special
				return false
			SaveOldWord
			print "Which "; parse$ ; " do you mean, ";
			local i
			for (i=1;i<= disamb_holder.disamb_total ;i++ )
			{
				if disamb_holder.disamb_suspects #i.article
				 print "the ";
				print number i; ") "; disamb_holder.disamb_suspects #i.name;
				if i = disamb_holder.disamb_total
					break
				if disamb_holder.disamb_total > 2
					print ",";
				if disamb_holder.disamb_total > 1 and
					i = disamb_holder.disamb_total - 1
					print " or";
				print " ";
			}
			"?"
		}
#else
		case 8
	{
	!  if Roodylib's disambiguation system is turned off, we make a guess
	!  that the player will only be providing one extra word
		InsertWord(1,1)
		return false
	}
#endif
		case 9
		{
			local v
			if word[1] = "take" and (word[2] = "off" or word[3] = "off")
			{
				v = " off"
			}
#ifset NO_WEARALL
			local c
			if verbroutine =&DoGet and (word[1] = "remove" or
				(word[1] = "take" and word[3] = "off")) and
				xobject is living
			{
				verbroutine = &DoTakeOff
				c = true
				if xobject = player
				{
					print "Try \"";parse$;
					if v
						print v;
					" all\" instead."
				}
				else
				{
					print "Please specify which item you'd like to ";parse$;
					if v
						print v;
					"."
				}
				return true
			}
#ifclear USE_CHECKHELD
			if (verbroutine = &DoWear, &DoTakeOff) or c
#else
			if (verbroutine = &DoWear_Checkheld, &DoTakeOff_Checkheld) or c
#endif
			{
				print "Please specify which item you'd like to ";parse$;
!				if player_person ~= 2:  print The(player, true); " ";
!				print "to ";
				if v
					print v;
				"."
				return true
			}
#endif ! NO_WEARALL
			if verbroutine = &DoGet and xobject = player and
			not (word[1] = "remove" or
				(word[1] = "take" and word[3] = "off"))
			{
				ParseError(6) ! "That didn't make any sense"
				return true
			}
			print "Nothing to ";
			if verbroutine
			{
				print parse$;
				if v
					print v;
				"."
			}
			else:  print "do."
		}

		case 10
		{
			print CThe(player); \
			MatchPlural(player, "hasn't", "haven't");
			if &obj.long_desc
				" seen ";
			else
				" encountered ";
			if (obj and obj is living and obj.pronoun #2 ~= "it")
				print "anybody";
			else: print "anything";
			print " like that";
#ifset DEBUG
			if (debug_flags & D_OBJNUM)
				print " ; number obj; ?";
#endif
			print "."
		}

		case 11
		{
			if not ParseError_ObjectIsKnown(obj)
			{
				ParseError(10, obj)
			}
			elseif parent(obj) ~= 0 and FindObject(parent(obj), location) and
				parent(obj) is openable
			{
				if CheckReach(parent(obj))
				{
					print CThe(parent(obj)); \
					IsOrAre(parent(obj), true); " closed."
				}
			}
			elseif obj is living
			{
!\
				print CThe(obj); \
					MatchPlural(obj, "isn't", "aren't"); " here."
\!
				print CThe(actor); \
					MatchPlural(actor, "doesn't", "don't"); \
					" see";
				if (obj.pronouns #2)
					print " "; obj.pronouns #2;
			  	elseif obj is plural
			  		" them";
			  	elseif obj is female
			  		" her";
			  	else
			  		" him";
#ifset DEBUG
				if (debug_flags & D_OBJNUM)
					print " ["; number obj; "]";
#endif
				print "."
			}
			else
			{
#ifset USE_CHECKHELD
				if verbroutine = &DoDrop_CheckHeld, &DoPutIn_CheckHeld,
					&DoPutonGround_CheckHeld
					ParseError(15, obj)
				else
				{
#endif
					print CThe(actor); \
						MatchPlural(actor, "doesn't", "don't"); \
						" see that"; ! MatchPlural(obj, "that", "those");
#ifset DEBUG
					if (debug_flags & D_OBJNUM)
						print " ["; number obj; "]";
#endif
					print "."
#ifset USE_CHECKHELD
				}
#endif
			}
		}

		case 12
		{
			if not ParseError_ObjectIsKnown(obj)
				ParseError(10, obj)
			else
			{
				print CThe(actor); \
					" can't do that with ";
				if obj ~= actor
					TheOrThat(obj)
				else
					print obj.pronouns #4;
				"."
			}
		}
		case 13
			print "You'll have to be a little more specific."

		case 14
		{
			if not ParseError_ObjectIsKnown(obj)
				ParseError(10, obj)
			elseif xobject is living
			{
				print CThe(xobject); \
					MatchPlural(xobject, "doesn't", "don't"); \
					" have "; TheOrThat(obj); "."
			}
			else
			{
				print CThe(actor); \
					MatchPlural(actor, "doesn't", "don't"); \
					" see "; TheOrThat(obj); " there."
			}
		}

		case 15
		{
			if not ParseError_ObjectIsKnown(obj)
				ParseError(10, obj)
			elseif obj = player
				ParseError(12, obj)
			else
			{
				print CThe(actor); IsorAre(actor); " not
					holding"; MatchPlural(obj, "that", "those"); "."
				AssignPronoun(obj,true)
			}
		}

		case else
		{
			last_actor = 0
			parser_data[PARSER_STATUS] = PARSER_RESET
			return false            ! print the default message
		}

	last_actor = 0
	parser_data[PARSER_STATUS] = PARSER_RESET
	return true                             ! message already printed
}

! Roody's note: NewParseError now handles some of the parsing so be sure
! to copy this code into your replacement (it was put in NewParseError just
! in case you need to change the parsing rules yourself for something)
replace NewParseError(errornumber,obj)
{

	! let's clear some parsing related stuff
	if not (errornumber = 8,1)
	{
#ifclear NO_DISAMB_HELP
		disamb_holder is not special
#endif
		parser_data[LAST_PARSER_STATUS] = 0
		oldword[0] = 0
	}

	select errornumber
		case else : return false
	return true
}


! Roody's note: Removes the jump command... just because.
replace Perform(action, obj, xobj, queue, isxverb)
{
	local r
	local objtemp, xobjtemp, verbtemp, actortemp
#ifclear NO_XVERBS
	local restoring
	if verbroutine = &DoRestore:  restoring = true
#endif

#ifset DEBUG
	if debug_flags & D_PARSE
	{
		print "\B[Perform("; number action; ", "; obj.name;
		if (debug_flags & D_OBJNUM) or queue = -1
			print " ["; number obj; "]";
		print ", "; xobj.name;
		if (debug_flags & D_OBJNUM) or queue = -1
			print " ["; number xobj; "]";
		if queue
			print ", "; number queue;
		print ")]\b"
	}
#endif

	if queue
		parser_data[PARSER_STATUS] |= PERFORM_QUEUE

	if not queue and object
		parser_data[LAST_SINGLE_OBJECT] = object
	else
		parser_data[LAST_SINGLE_OBJECT] = 0

	parser_data[VERB_IS_XVERB] = isxverb

!  These temp objects guarantee we go back to whatever the previous
!  settings were before Perform was called
	objtemp = object
	xobjtemp = xobject
	verbtemp = verbroutine
	actortemp = actor

	object = obj
	xobject = xobj
	verbroutine = action
	actor = player

!  some stuff we do when Perform is called by the engine
	if (parser_data[PARSER_STATUS] & PARSER_ACTIVE) and not isxverb
		DeactivateParser

#ifclear NO_OBJLIB
	if verbroutine = &DoGo and not object
		SetupDirectionObjects
#endif

	! Itemize each object in a list of multiple objects
	if queue > 0 and object > display
	{
#ifset USE_CHECKHELD
		! Check if an ImplicitTakeForDrop was just done, meaning we
		! need a newline before printing the next "object:"
		if checkheld is workflag
			print ""
		checkheld is not workflag
#endif
		print object.name; ":  ";
	}

	r = BeforeRoutines(queue)
	if not r ! if action not stopped by before routines
	{
		r = call action                 ! object.after and xobject.after
		                                ! run by verbroutine
#ifclear NO_XVERBS
		if restoring
			verbroutine = &DoRestore
#endif
		if r  ! if action was successful, run after routines
		{
			AfterRoutines
		}
	}

!:DonePerform area
#ifset USE_CHECKHELD
	ResetCheckHeld
#endif

	last_object = object

	verbroutine = verbtemp
	object = objtemp
	xobject = xobjtemp
	actor = actortemp

	if queue = -1
		last_object = -1

	parser_data[PARSER_STATUS] = PARSER_RESET
	return r
}

routine DeactivateParser
{
	last_actor = actor
	parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
	AssignPronoun(object)
	parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
	parser_data[PARSER_STATUS] &= ~PARSER_ACTIVE
	SaveOldWord
}

! Roody's note: Fixes a print statement, the math for determining rank,
! and allows score/rank for more than just STATUSTYPE 1
replace PrintScore(end_of_game)
{
	if MAX_SCORE
	{
		if end_of_game:  print ""
		print "You ";
		if not end_of_game
			print "have ";
		print "scored a total of ";
		print number score; " out of "; number MAX_SCORE;
		if ranking[0] ~= ""
		{
			print ", giving you the rank of ";

			! A complicated formula, since only
			! integer division is allowed:
			!
			local temp_rank
			temp_rank = (score*MAX_RANK)/MAX_SCORE
			if temp_rank > MAX_RANK
				temp_rank = MAX_RANK
			print ranking[temp_rank];
!\ Changed so that if the score goes over the MAX_SCORE, it continues
to give the highest ranking. \!
		}
		print "."
	}
}

#ifclear NO_SCRIPTS

!\ Roody's note:  hugolib's Script routine was returning
an incorrect value:  return o * MAX_SCRIPT * 2

Since the setscript[] array has 1024 elements and there is a maximum
number of concurrent scripts of 16 (MAX_SCRIPTS), we want to setscript[]
divided into groups of 64 elements.  The last line in this routine could
written as   "return o * (setscript[]/MAX_SCRIPTS)" as an alternative.

I'm using "return o * MAX_SCRIPT_STEPS * 2"  (o * 32 * 2)
\!
replace Script(obj, steps)
{
	local o

	o = 0
	while scriptdata[o * 3] ~= obj and scriptdata[o * 3] ~= 0 and
		o < MAX_SCRIPTS
	{
		o++
	}
	if o = MAX_SCRIPTS
		return -1
	if scriptdata[o * 3] = 0
	{
		scriptdata[o * 3] = obj                ! the object
		number_scripts++
	}
	scriptdata[o * 3 + 1] = 0                       ! starting step
	scriptdata[o * 3 + 2] = steps                   ! total no. of steps
	return o * MAX_SCRIPT_STEPS * 2
}
#endif  ! ifclear NO_SCRIPTS

!\ Roody's note: This version of RunScripts allows scripts to be run with a
true/false argument (mainly so LoopScripts can be called with a true
value).

It also has some modifications to adapt to the fixed Script routine above.\!
replace RunScripts
{

#ifclear NO_SCRIPTS

	local a, obj, step, total, routine, b
	local tempactor, tempverb, tempobject

	for (; a<number_scripts; a++)
	{
		obj = scriptdata[a * 3]         ! this object/character
		step = scriptdata[a * 3 + 1]    ! current step
		total = scriptdata[a * 3 + 2]   ! total steps in script

		if obj and total < 0            ! if skipping this script
			scriptdata[a * 3 + 2] = scriptdata[a * 3 + 2] + 32767

		elseif obj and step < total and step >= 0
		{
		!\  Since script "step"s are actually two elements (the
		   character routine and the object), we have to multiply
		   it by two when jumping to the current step being
		   executed.

		   This next line was originally:
		   routine = setscript[(a * MAX_SCRIPT_STEPS) * 2 + step * 2]
		
		   We also could have written it like this:
		   routine = setscript[a * setscript[]/MAX_SCRIPTS + step * 2]
\!
			! action
			routine = setscript[(a * MAX_SCRIPT_STEPS + step) * 2]

			! object being acted upon
			b = setscript[(a * MAX_SCRIPT_STEPS + step) * 2 + 1]

#ifset DEBUG
			if debug_flags & D_SCRIPTS
			{
				print "[Script for obj. ";
				print number obj; " ("; obj.name; "), step ";
				print number (step + 1); ":  $"; \
				hex routine; ", obj. ";
				print number b; " ("; b.name; ")]"
			}
#endif

			scriptdata[a * 3 + 1] = step + 1
			tempactor = actor
			tempverb = verbroutine
			tempobject = object
			actor = obj
			verbroutine = routine
			object = b
			if not parent(actor).before
			{
				if not actor.before
				{
					if not object.before or object = 1
					{
						call routine(obj, b)
						run actor.after
					}
				}
			}
			if actor in location
				actor  is known
			actor = tempactor
			verbroutine = tempverb
			object = tempobject
		}
		elseif step >= 0
		{
			scriptdata[a * 3] = 0   ! clear this object's script
			if a = number_scripts - 1
				number_scripts--
		}
	}

#endif  ! ifclear NO_SCRIPTS

}

! Roody's note: Jump command removed. in superbrief mode, objects with
! short_descs treated like objects without them.
replace ShortDescribe(obj)
{
	local ListContents

	obj is known

	if list_nest = 1
		print newline

	AssignPronoun(obj)

	if obj is not moved and &obj.initial_desc
	{
		Indent
		run obj.initial_desc
		ListContents = true
	}
	elseif obj is openable
	{
		if obj is open
		{
			if &obj.when_open
			{
				Indent
				run obj.when_open
				ListContents = true
			}
		}
		elseif &obj.when_closed
		{
			Indent
			run obj.when_closed
			ListContents = true
		}
	}

	if (not ListContents)
	{
		if verbosity = 1
			return

		if &obj.short_desc
			Indent

		if not obj.short_desc
			return
	}

!  "ListContents" section
	obj is already_listed
	AssignPronoun(obj)

	list_count = 0

	if children(obj) > 0 and (obj is platform or obj is transparent or
	(obj is container and
	(obj is not openable or (obj is openable and obj is open)))) and
	obj is not quiet
	{
		list_nest = (LIST_F = (FORMAT & LIST_F))
		WhatsIn(obj)
	}

	! If INDENT_SIZE is 0, formatting may be thrown off when listing
	! the contents of an object:
	if INDENT_SIZE = 0:  need_newline = true

	if need_newline:  print newline
}

! Roody's note: This only exists for CHARACTER, G. Never actually called.
routine DoAgain
{}

! SpeakTo Replacement
!
! Roody's note: Removes jump command. Only clears/resets speaking global if
! player tries talking to a character.

!\
	There is also now a player.before check so authors can add special
	responses for situations where the player can't speak.  Pretend your game
	has a gas mask that, when worn, it muffles all of the player's words.

	One could add this code:

	before
	{
		player SpeakTo
		{
			if parent(gasmask) = player and gasmask is worn
			{
				"You try to speak, but the gas mask muffles
				all of your words."
				return true
			}
			return false
		}
	}

	Figuring out the best chronology of SpeakTo's behaviors has been
	somewhat of a dilemma for me, so if anyone plays with this feature
	and runs into any problems, let me know!
\!
replace SpeakTo(char)
{
	local TryOrder, IgnoreResponse, retval, stay, same, different, tempverbroutine

	if verbroutine = &DoAgain
	{
		verbroutine = last_verbroutine
		object = previous_object
		xobject = previous_xobject
		RestoreFromOldWord(0,1) ! restore one command
!\ Roody's note: We don't need to, but I restore the command just in case
   authors are checking phrasing, like how >READ THING gets directed to
	>LOOK and authors check word[1] to determine if "read" was used  \!
	}
	else
		DeactivateParser

#ifset USE_CHECKHELD
	if verbroutine = &DoDrop_CheckHeld
		verbroutine = &DoDrop
	elseif verbroutine = &DoPutIn_CheckHeld
		verbroutine = &DoPutIn
	elseif verbroutine = &DoTakeOff_CheckHeld
		verbroutine = &DoTakeOff
	elseif verbroutine = &DoWear_CheckHeld
		verbroutine = &DoWear
#endif
#ifset VERBSTUBS
	if verbroutine = &DoHelpChar and object = player
	{
		verbroutine = &DoHelp
		object = nothing
	}
#endif

#ifset USE_CHECKHELD
	ResetCheckHeld
#endif

#ifset DEBUG
	if debug_flags & D_PARSE
	{
		print "\B[Speakto("; char.name;
		if (debug_flags & D_OBJNUM)
			print " ["; number char; "]";
		print ") verbroutine="; number verbroutine;
		print ", object="; object.name;
		if (debug_flags & D_OBJNUM)
			print " ["; number object; "]";
		print ", xobject="; xobject.name;
		if (debug_flags & D_OBJNUM)
			print " ["; number xobject; "]";
		print "]\b"
	}
#endif

	if char is not living
	{
		if word[1] = ""
			ParseError(2) ! Please supply a verb.
		else
			ParseError(6)  ! "That doesn't make any sense."
		return
	}

	if object
		parser_data[LAST_SINGLE_OBJECT] = object
!	parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
	AssignPronoun(char)
!	parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET

	! Handle player/typist-related ParseError messages:
	if char = player
		Message(&Speakto, 1)    ! "Stop talking to yourself..."
	elseif not ObjectisKnown(object) and not FindObject(object, location)
		ParseError(10, object)
	else
		stay = true

	if not stay
	{
		speaking = 0
		return
	}

	if char is unfriendly
		IgnoreResponse = true
	else
	{
		tempverbroutine = verbroutine
		verbroutine = &SpeakTo
		if player.before
			return true
		verbroutine = tempverbroutine

#ifclear NO_OBJLIB
		if verbroutine = &DoGo and not object
			SetupDirectionObjects
#endif
		! In the event of:  >CHARACTER, GO NORTH.  GET THE THING.  GO WEST., etc.
		if not FindObject(char, location)
		{
			speaking = char
			run char.order_response
			return true
		}

		same = (char = speaking)

		select verbroutine
			case 0                  ! Just the character name is given,
						! so just "X is listening."
			{
				if not char.order_response
					Message(&Speakto, 2, char)
				retval = true
			}

#ifclear NO_VERBS
			case &DoHello           ! Note the ampersands ('&')--or else
			{                       ! the routines themselves would run
				if not char.order_response
				{
					if char is not unfriendly
					{
						! "X nods hello."
						Message(&Speakto, 3, char)
						retval = true
					}
					else
						IgnoreResponse = true
				}
				else
					retval = true
			}

			case &DoAskQuestion
				return Perform(&DoAsk, char, object)

			case &DoTalk
			{
				if xobject and object ~= player
					ParseError(6)
				elseif xobject and object = player
					return Perform(&DoAsk, char, xobject)
				else
					return Perform(&DoAsk, char, object)
			}

			case &DoTell
			{
				if object = player
					return Perform(&DoAsk, char, xobject)
				else
					TryOrder = true
			}
#endif  ! ifclear NO_VERBS

			case else
			{

	! If the character can respond to a request, this should be dealt with by
	! an order_response property routine; order_response--if it exists--should
	! return false if there is no response for the given verbroutine

				TryOrder = true
			}
	}

	if TryOrder
	{
		if (not char.order_response)
			IgnoreResponse = true
		else
			retval = true
	}

	different = (speaking ~= char)

!	This same/different local variable stuff allows for certain
!	orders to end conversations. If your order_response code clears
!	the speaking global, this code prevents it being reset.

	if retval and not (same and different)
		speaking = char

	if IgnoreResponse
	{
		if not char.ignore_response
			Message(&Speakto, 4, char)      ! "X ignores you."
		speaking = 0  ! clear the speaking global
	}
!	parser_data[LAST_PARSER_STATUS] = PARSER_RESET
	return retval
}

#ifset USE_CHECKHELD
! Roody's note: new VerbHeldMode to work with checkheld wear/remove system
replace VerbHeldMode(w)
{
	if w = "get", "take", "grab", "pick" ! , "remove"
	{
		return -1			! explicitly notheld
	}
	elseif w = "drop", "put", "leave", "place", "throw", "insert",
		"give", "offer" , "wear" ! , "remove"
	{
		return 1			! explicitly held
	}
	return 0
}
#endif  ! ifset USE_CHECKHELD

routine CountDigits(num)
{
	local sum
	do
	{
		sum++
		num /= 10
	}
	while num
	return sum
}

routine GetStatusLength(len)
{
	if STATUSTYPE = 1,INFOCOM_STYLE, 9
	{
		local a,b, sum
		a = CountDigits(score)
		b = CountDigits(counter)
		if STATUSTYPE = 1
		{
			sum = a + b + 4
		}
		else
		{
			sum = a + b + 17
			if (STATUSTYPE & INFOCOM_STYLE) and
				((len + sum + 6) > display.screenwidth)
			{
				if (sum - 8 + len) < display.screenwidth
				{
					sum = a + b + 9
				}
			}
		}
	}
	elseif STATUSTYPE = 2,16, 18
	{
		if (STATUSTYPE & MILITARY_TIME)
#ifset NO_MILITARY_COLON
			sum = 11
#else
			sum = 6
#endif
		else
		{
			local num
			num =  counter / 60
			while num > 12
			{
				num -= 12
			}
			if not num or num > 9
				sum = 11
			else
				sum = 10
		}
	}
	elseif STATUSTYPE = 4
	{
		text to _temp_string
		StatusType4
		text to 0
		sum = StringLength(_temp_string) + 1
	}
	return sum
}

routine PrintStatusType
{
	if (STATUSTYPE & INFOCOM_STYLE)
	{
		if (STATUSTYPE & INFOCOM_STYLE) and
			((display.linelength - display.cursor_column) <  17)
		{
				print "S: "; number score; "\_ "; "M: "; number counter;
				return
		}
		print "Score: "; number score; "\_ "; "Moves: "; number counter;
	}
	elseif (STATUSTYPE & SCORE_MOVES)
			print number score; " / "; number counter;
	elseif (STATUSTYPE & TIME_STATUS) or (STATUSTYPE & MILITARY_TIME) or
		STATUSTYPE = 2
	{
		print HoursMinutes(counter);
	}
	elseif (STATUSTYPE = 4)
		StatusType4 ! routine for configurable statusline
}

replace PrintStatusline
{
	local a
#ifset NO_ACCESSIBILITY
	local cheap
#endif

	if cheap = 1
		return

	if not display.statusline_height
		display.statusline_height = 1

	Font(BOLD_OFF | ITALIC_OFF | UNDERLINE_OFF | PROP_OFF)
	color SL_TEXTCOLOR, SL_BGCOLOR
	window display.statusline_height
	{
		if not (TERP & SIMPLE_TERP) ! simple
		{
			cls
			locate 1, 1
		}
		PrintStatusLineLocation
		a = GetStatusLength(display.cursor_column)
		if (a + display.cursor_column + 4) < display.linelength
		{
			print to (display.linelength - \
			( a + \
			((TERP = SIMPLE_TERP)* 2) - (TERP = GLK_TERP)));
			PrintStatusType
		}
	}
	color TEXTCOLOR, BGCOLOR
	Font(DEFAULT_FONT)
	ExtraWindows
}

routine ExtraWindows
{}

routine PrintStatusLineLocation
{
	if not location
		print "\_";
	elseif not light_source
		print "\_In the dark";
	else
	{
		print "\_";
		print capital location.name;
	}
}

! Roody's note: Replace this if you want to use the top right area
! for something else ("HUNGRY", "TIRED", or whatever)
routine STATUSTYPE4
{}

!----------------------------------------------------------------------------
!* "SETTINGS" CLASS AND ROUTINES
!----------------------------------------------------------------------------

! A new "settings" class and some new properties to use for library extension
! settings (so I don't have to keep track of which individual library uses
! what aliased property for what.

class settings
{}

property save_info

!\ A couple routines used by routines like DoRestore and DoRestart
to save certain game state things. \!

routine SaveSettings(w)
{
	word[LAST_TURN] = w
! #ifclear NO_FANCY_STUFF
	local i,n = 30
	for i in init_instructions
	{
		if i.save_info
		{
			n = SaveWordSetting(i.name, n)
			n--
		}
	}
! #endif ! NO_FANCY_STUFF
}

routine SaveWordSetting(w,start_val)
{
	local i
	if not start_val
		start_val = 30
	for (i=start_val ; i>0 ; i-- )
	{
		if word[i] = "" ! found an open slot
			word[i] = w  ! add word to word array
		if word[i] = w ! word is already in word array
			return i
	}
	return false ! didn't find anything

}

routine LoadSettings
{
	local i, n, v, s
	for i in init_instructions
	{
		v = CheckWordSetting(i.name, s)
		if v
		{
			run i.execute
			s = v - 1
		}
	}
	n = LAST_TURN
	while word[n] ~= ""
	{
		word[n--] = ""
	}
}

routine CheckWordSetting(w,start_val)
{
	local i
	if not start_val
		start_val = 30
	for (i=start_val ; i>0 ; i-- )
	{
		if word[i] = ""
			return false
		elseif word[i] = w
			return i
	}
	return false ! didn't find anything
}

!----------------------------------------------------------------------------
!* INIT / MAIN INSTRUCTION OBJECTS
!----------------------------------------------------------------------------

property execute alias misc
property did_print alias extra_scenery

routine Init_Calls
{
	local i
	for i in init_instructions
	{
		run i.execute
		if i.did_print
			InitScreen
	}
	ClearWordArray
}

object init_instructions
{
	in settings
}

object roodylib "roodylib"
{
	in init_instructions
	type settings
!#ifclear NO_FANCY_STUFF
	save_info
	{
		select verbosity
			case 0 : SaveWordsetting("brief")
			case 1 : SaveWordSetting("superbrief")
			case 2 : SaveWordSetting("verbose")
		return true
	}
!#endif ! NO_FANCY_STUFF
	execute
	{
		if word[LAST_TURN] ~= "undo"
		{
			if word[LAST_TURN] ~= "restore" ! new game
			{
				CalculateHolding(player)
#ifset USE_PLURAL_OBJECTS
				InitPluralObjects
#endif
			}
!#ifclear NO_FANCY_STUFF
			local a
			a = CheckWordSetting("roodylib")
			select word[(a+1)]
				case "brief" : verbosity = 0
				case "superbrief" : verbosity = 1
				case "verbose" : verbosity = 2
!#endif ! ! NO_FANCY_STUFF
			local opcodeterp_check,x
			opcodeterp is not switchedon
			opcodeterp.terp_name = 0
			if system(61)
			{
				if IsGLK
					TERP = GLK_TERP
				else
					TERP = SIMPLE_TERP
			}
			else
				TERP = NORMAL_TERP
			for x in opcodeterp
			{
				x is not switchedon
			}
			readfile "HrCheck"
			{
				opcodeterp_check = readval
			}
			if opcodeterp_check
				opcodeterp.terp_name = "Hugor"
			else
			{
				readfile "OpCheck"
				{
					opcodeterp_check = readval
				}
			}
			if opcodeterp_check
			{
				opcodeterp is switchedon
				opcodeterp.control_file = "HrCtlAPI"
				if ExecOpCode(is_opcode_available, is_opcode_available.opcode_value)
				{
					for x in opcodeterp
					{
						if ExecOpCode(is_opcode_available,x.opcode_value)
							x is switchedon
					}
				}
			}
			if is_opcode_available is switchedon
			{
				ExecOpCode(is_fluid_layout)
				ExecOpCode(hidden_cursor_check)
				ExecOpCode(screenreader_capable)
				ExecOpCode(top_justified_check)
			}

			if opcodeterp is switchedon
			{
				ExecOpcode(getversion)
				ExecOpcode(get_os)
				ExecOpcode(full_opacity)
				self.did_print = 1
			}
		}
	}
	did_print 0
}

#ifset HUGOFIX
! Roody's note: Made a HugoFix object so that all monitor commands remain
! active after a restart.
object hugofixlib "hugofixlib"
{
	in init_instructions
	type settings
	did_print true
!#ifclear NO_FANCY_STUFF
	save_info
	{
		if debug_flags = 0 ! if absolutely no debug flags are turn on,
			return          !  just return right away
		local b
#ifclear NO_FUSES
		if debug_flags & D_FUSES
			b = SaveWordSetting("fuses",b)
#endif
		if debug_flags & D_OBJNUM
			b = SaveWordSetting("objnum",b)
		if debug_flags & D_PARSE
			b = SaveWordSetting("parse",b)
#ifclear NO_SCRIPTS
		if debug_flags & D_SCRIPTS
			b = SaveWordSetting("scripts",b)
#endif
		if debug_flags & D_FINDOBJECT
			b = SaveWordSetting("findobject",b)
		if debug_flags & D_PARSE_RANK
			b = SaveWordSetting("parserank",b)
		if debug_flags & D_PLAYBACK_HELPER
			b = SaveWordSetting("playbackhelper",b)
		if b
			return true
		else
			return false
	}
!#endif ! NO_FANCY_STUFF
	execute
	{
!#ifclear NO_FANCY_STUFF
		if word[LAST_TURN]
		{
			local a,b
			a = CheckWordSetting(self.name)
			while true
			{
				select word[(a+1)]
#ifclear NO_FUSES
					case "fuses" : b = D_FUSES
#endif
					case "objnum" : b = D_OBJNUM
					case "parse" : b = D_PARSE
#ifclear NO_SCRIPTS
					case "scripts" : b = D_SCRIPTS
#endif
					case "findobject" : b = D_FINDOBJECT
					case "parserank" : b = D_PARSE_RANK
					case "playbackhelper" : b = D_PLAYBACK_HELPER
					case else : break
				debug_flags = debug_flags | b
				a++
			}
		}
!#endif ! ! NO_FANCY_STUFF
		if (word[LAST_TURN] ~= "undo","restore")
		{
			OrganizeTree
			HugoFixInit
		}
	}
}
#endif ! HUGO_FIX

!\ Roody's note: Main_Calls is called last in Roodylib's shell's main
routines. This is nice for game system messages, like score notifications or
footnotes or what have you.\!

routine Main_Calls
{
	local i
	i = child(main_instructions)
	while i
	{
		run i.execute
		i = younger(i)
	}
}

object main_instructions
{
	in settings
}

!----------------------------------------------------------------------------
!* INTERPRETER THINGY
!----------------------------------------------------------------------------
global TERP

property version alias n_to
property os alias ne_to
property terp_name alias e_to
property control_file alias se_to

object opcodeterp "opcode interpreter monitor"
{
	terp_name 0
	os 0
	version 0 0 0
	type settings
	in settings
	control_file 0 ! "HrCtlAPI"
}

#ifset HUGOFIX
! Roody's note: This object adds an opcode monitor addition to the
! HugoFixInit screen

object opcodemonitorinit "opcode monitor init"
{
	in hugofixinitmenu
	type settings
	long_desc
	{
		if opcodeterp is not switchedon
			return false
		Indent
		local i
		hugofixinitmenu.hugofixmenu #10	+= 1
		i = hugofixinitmenu.hugofixmenu #10
		print number i; ". ";
		print "Opcode Monitor (";
		if (debug_flags & D_OPCODE_MONITOR)
			print "ON";
		else
			print "OFF";
		")"
		hugofixinitmenu.hugofixmenu #i = D_OPCODE_MONITOR
	}
}

! Roody's note: This object adds opcode commands to
! the HugoFix "$?" command and executes the commands

object opcode_monitoring "opcode monitoring"
{
	in hugofix_monitoring
	extra_scenery "$oa" "$om"
	long_desc
	{
		select VerbWord
			case "$","$?"
			{
				if opcodeterp is switchedon
				{
					"\_    $om - Opcode monitor on/off"
					"\_    $oa - Opcode audit"
				}
			}
			case "$oa"
			{
				if opcodeterp is not switchedon
					"[Opcode-enabled interpreter not detected. Opcode audit failed.]"
				else
					OpcodeAudit
			}
			case "$om"
			{
				if opcodeterp is not switchedon
					"[Opcode-enabled interpreter not detected. Opcode monitor not
					turned on.]"
				else
				{
					print "[Opcode monitor";
					OnorOff(D_OPCODE_MONITOR)
				}
			}
	}
}

! Roody's note:  This saves the opcode monitor setting in times
! of UNDO, RESTART, or RESTORE

!#ifclear NO_FANCY_STUFF
object opcodemonitorlib "opcodemonitorlib"
{
	in init_instructions
	type settings
	save_info
	{
		if debug_flags = 0 ! if absolutely no debug flags are turn on,
			return          !  just return right away
		local b
		if debug_flags & D_OPCODE_MONITOR
			b = SaveWordSetting("opcodemonitor",b)
		return b
	}
	execute
	{
		if word[LAST_TURN] !~= "undo" ! ,"restore"
		{
			local a
			a = CheckWordSetting(self.name)
			if word[(a+1)] = "opcodemonitor"
				debug_flags = debug_flags | D_OPCODE_MONITOR
		}
	}
}
! #endif  ! NO_FANCY_STUFF
#endif  ! HUGOFIX

property opcode_results
property opcode_value alias size
property opcode_type alias w_to
!attribute hugor_default

class opcode
{
	type opcode
	execute 0
	opcode_value 0
}

opcode is_opcode_available "opcode availability opcode"
{
	opcode_value 1
	save_info
	{
		local ret
		ret = readval
		return ret
	}
	opcode_type is_opcode_available
	is switchedon
}

opcode getversion "version opcode"
{
	in opcodeterp
	opcode_value 100
	save_info
	{
		opcodeterp.version = readval
		opcodeterp.version #2 = readval
		opcodeterp.version #3 = readval
	}
	opcode_type getversion
!	is hugor_default
}

opcode get_os "Get OS opcode"
{
	nearby
	opcode_value 200
	save_info
		opcodeterp.os = readval
	opcode_type get_os
!	is hugor_default
}

opcode op_abort "Op abort"
{
	nearby
	opcode_value 300
	opcode_type op_abort
	! is hugor_default
}

property block alias n_to ! (true to stop action, false: code continues while
                          !  running)
property duration alias ne_to ! in milliseconds
property start_opacity alias e_to ! -1 -> 255 (-1 to keep at current opacity)
property end_opacity alias se_to ! 0... 255

opcode fade_screen "fade screen opcode"
{
	nearby
	opcode_value 400
	block 1
	duration 0
	start_opacity -9999
	end_opacity -9999
	execute
	{
		! This just checks to make sure that somebody didn't accidentally
		! run ExecOpcode on this class object as it would hide all text on
		! the screen
		if self = fade_screen
		{
#ifset DEBUG
			if debug_flags & D_OPCODE_MONITOR
			{
				print "[Do not execute opcode
				\""; self.name;"\" itself. Create a fade_screen
				object with timing and fade values you want. Fade canceled.]"
			}
#endif
			return
		}
		local start_alpha
		start_alpha = self.start_opacity
		if self.start_opacity < 0
			start_alpha = -9999
		writeval self.duration , start_alpha, self.end_opacity, self.block
	}
	opcode_type fade_screen
	! is hugor_default
}

fade_screen full_opacity "restore full opacity opcode"
{
	in fade_screen
	block 1
	duration 1
	start_opacity 255
	end_opacity 255
}

opcode open_URL "open URL opcode"
{
	in opcodeterp
	opcode_value 500
	opcode_type open_URL
	! is hugor_default
}

opcode set_fullscreen "Set Fullscreen opcode"
{
	nearby
	opcode_value 600
	execute
		writeval 1
	opcode_type set_fullscreen
	! is hugor_default
}

opcode windowed "Windowed opcode"
{
	nearby
	opcode_value 600
	execute
		writeval 0
	opcode_type windowed
	! is hugor_default
}

opcode clipboard "Clipboard opcode"
{
	nearby
	opcode_value 700
	opcode_type clipboard
	! is hugor_default
}

opcode is_music_playing "music detector opcode"
{
	nearby
	opcode_value 800
	save_info
	{
		local ret
		ret = readval
#ifset _RESOURCE_H
		if audio.current_music and not ret
		{
			audio.current_music = 0
			audio.current_music_resfile = 0
			audio.current_music_looping = 0
		}
#endif
		return ret
	}
	opcode_type is_music_playing
	! is hugor_default
}

opcode is_sample_playing "sound sample detector"
{
	nearby
	opcode_value 900
	save_info
	{
		local ret
		ret = readval
#ifset _RESOURCE_H
		if audio.current_sound and not ret
		{
			audio.current_sound = 0
			audio.current_sound_resfile = 0
			audio.current_sound_looping = 0
		}
#endif
		return ret
	}
	opcode_type is_sample_playing
	! is hugor_default
}

opcode is_fluid_layout "Screen Fluidity opcode"
{
	nearby
	opcode_value 1000
	opcode_type is_fluid_layout
	save_info
	{
		local ret
		ret = readval
		if ret
			self is special
		else
			self is not special
		return ret
	}
}

property rgb alias ne_to
property alpha_value alias e_to

opcode set_color "Set Color opcode"
{
	nearby
	opcode_value 1100
	rgb 0 0 0
	alpha_value 0
	opcode_type set_color
	execute
	{
		! This just checks to make sure that somebody didn't accidentally
		! run ExecOpcode on this class object as it would hide all text on
		! the screen
		if self = set_color
		{
#ifset DEBUG
			if debug_flags & D_OPCODE_MONITOR
			{
				print "[Do not execute opcode
				\""; self.name;"\" itself. Create a set_color
				object with timing and fade values you want. Fade canceled.]"
			}
#endif
			return
		}
		writeval self.rgb , self.rgb #2, self.rgb #3
		writeval self.alpha_value
	}
}

opcode is_fullscreen "Fullscreen Check opcode"
{
	nearby
	opcode_value 1200
	opcode_type is_fullscreen
	save_info
	{
		local ret
		ret = readval
		return ret
	}
}

opcode hidden_cursor_check "Does Terp Hide Paused Cursors? opcode"
{
	nearby
	opcode_value 1300
	opcode_type hidden_cursor_check
	! is hugor_default
	save_info
	{
		local ret
		ret = readval
		if ret
			self is special
		else
			self is not special
		return ret
	}
}

opcode top_justified_check "Does Terp handle top justified text ok?"
{
	nearby
	opcode_value 1400
	opcode_type top_justified_check
	! hugor_default
	save_info
	{
		local ret
		ret = readval
		if ret
			self is special
		else
			self is not special
		return ret
	}
}

opcode screenreader_capable "Does terp work with screenreaders?"
{
	nearby
	opcode_value 1500
	opcode_type screenreader_capable
	save_info
	{
		local ret
		ret = readval
		if ret
			self is special
		else
			self is not special
		return ret
	}
}

opcode check_resource_opcode "Resource File Check opcode"
{
	nearby
	opcode_value 1600
	opcode_type check_resource_opcode
	save_info
	{
		local ret
		ret = readval
		return ret
	}
}

routine ExecOpcode(op_file,str,str2)
{
	if op_file.type ~= opcode
	{
#ifset DEBUG
		if debug_flags & D_OPCODE_MONITOR
		{
			print "[ ExecOpcode run with non-opcode type object
			\""; op_file.name;"\" ]"
		}
#endif
		return
	}
	if op_file.opcode_type is not switchedon ! not a typo
	{
#ifset DEBUG
		if debug_flags & D_OPCODE_MONITOR
		{
			print "[ Opcode \""; op_file.name ;"\" not supported by
			interpreter. ]"
		}
#endif
		return
	}
	writefile opcodeterp.control_file
	{
		writeval op_file.opcode_value
		if str
			writeval str
		if str2
			writeval str2
		if &op_file.execute
			run op_file.execute
	}
	local failure, result
	readfile opcodeterp.control_file
	{
		failure = readval
		if failure
		{
#ifset DEBUG
			if debug_flags & D_OPCODE_MONITOR
			{
				select failure
					case 10 : print "[ Incorrect number of parameters for \
					opcode "; op_file.name; " ["; number op_file;"]"
					case 20 : print "[ Interpreter returned unexpected byte count \
					for opcode "; op_file.name; " ["; number op_file;"]"
					case 30
					{
						local num, i
						num = readval
						print "[ unrecognized opcode "; number num ; \
						" attempted";
						num = readval
						if num
						{
							" with ";
							print number num; " argument";
							if num > 1
								print "s";
							print " (";
							for (i=1;i<=num ;i++ )
							{
								failure = readval
								print number failure;
								if failure < num
									print ",";
							}
							print ")";
						}
						print " ]"
					}
					case else
					{
						print "[ unidentified opcode failure with value ";
						print number failure;" ]"
					}
			}
#endif
			return
		}
#ifset DEBUG
		if debug_flags & D_OPCODE_MONITOR
			print "[ \""; op_file.name;"\" success. ]"
#endif
		if &op_file.save_info
			result = op_file.save_info
	}
	return result
}

routine OpCodeAudit
{
	local x
	"Opcodes supported by this interpreter:\n"
	Indent
	print is_opcode_available.name;
	print to 45;
	if is_opcode_available is switchedon
	{
		color 10
		print "opcode supported"
	}
	else
	{
		color 12
		print "opcode not supported"
	}
	color TEXTCOLOR, BGCOLOR
	for x in opcodeterp
	{
		Indent
		print x.name;
		print to 45;
		if x is switchedon
		{
			color 10
			print "opcode supported";
		}
		else
		{
			color 12
			print "opcode not supported";
		}
		color TEXTCOLOR, BGCOLOR
		if (x = is_fluid_layout,hidden_cursor_check,
			top_justified_check, screenreader_capable) and
			x is switchedon
		{
			if x is special
				print " (true)"
			else
				print " (false)"
		}
		else
			print newline
	}
}


!----------------------------------------------------------------------------
!* INIT ROUTINES
!----------------------------------------------------------------------------

routine SimpleIntro
{
	local g
	g = IsGlk
	if (not g and system(61)) and
		not CheckWordSetting("restart") ! non-glk, simple port
	{
"\_          ___\_      ___\_  ___\_      ___\_  _________\_    _________"
"\_         (__ |\_    | __) (__ |\_    | __)\_| _______ |\_  | _______ |"
"\_           | |\_    | |\_    | |\_    | |\_  | |\_    | |\_  | |\_    | |"
"\_           | |\_    | |\_    | |\_    | |\_  | |\_    |_|\_  | |\_    | |"
"\_           | |_____| |\_    | |\_    | |\_  | |\_   ___\_   | |\_    | |"
"\_           |\_ _____\_ |\_    | |\_    | |\_  | |\_  (__ |\_  | |\_    | |"
"\_           | |\_    | |\_    | |\_    | |\_  | |\_    | |\_  | |\_    | |"
"\_           | |\_    | |\_    | |\_    | |\_  | |\_    | |\_  | |\_    | |"
"\_          _| |\_    | |_\_   | |_____| |\_  | |_____| |\_  | |_____| |"
"\_         (___|\_    |___)\_  |_________|\_  |_________|\_  |_________|"
""
		PRINT to (display.screenwidth / 2 - 11) ; "I N T E R A C T I V E"
		PRINT to (display.screenwidth / 2 - 7) ; "F I C T I O N"
		print ""
		print to (display.screenwidth / 2 - 12) ; "press a key to continue"
		pause
		cheap = 1
		DrawCheap
	}
}
!\ old version of the SimpleIntro code- just cleared the screen
			local i  ! hack-y code to clear the screen
			for (i=1;i<display.screenheight ;i++ )
				{
				if i = display.screenheight / 2
					{
					print to (display.screenwidth/2 - 10);
					print "PRESS A KEY TO BEGIN"
					}
				else
					""
				}
\!
routine InitScreen
{
	local simple_port, relocate
#ifset NO_ACCESSIBILITY
	local cheap
#endif
#ifclear NO_ACCESSIBILITY
	if cheaplib is not special
	{
		run cheaplib.execute
		cheaplib is special
		if cheap = 1
			return
	}
#endif
	color TEXTCOLOR, BGCOLOR, INPUTCOLOR
	Font(DEFAULT_FONT)
	simple_port = (not IsGlk and system(61)) ! non-glk simple port
	if not system(61)
		window 0
#ifclear NO_ACCESSIBILITY
	if not simple_port and cheap ~= 1
		cls
#else
	if not simple_port ! not non-glk simple port
		cls
#endif
	if not system(61) and cheap ~= 1 ! CheckWordSetting("cheap")
		CenterTitle("",0,1,1) ! Draw an empty window
	else
		CenterTitle(CheapTitle,0,1,1) ! See CheapTitle routine
	relocate = (LinesFromTop ~= display.windowlines)
#ifclear NO_ACCESSIBILITY
	if not system(61) and not (cheap & CHEAP_ON) and relocate
		locate 1, LinesFromTop
	elseif (cheap & CHEAP_ON) or simple_port ! non-glk simple port
		""
#else
	if not system(61) and relocate
		locate 1,LinesFromTop
	elseif simple_port
		""
#endif ! CHEAP
	if display.needs_repaint
		display.needs_repaint = false
}

routine CheapTitle
{
	if display.title_caption
		return display.title_caption
	return "Hugo Interactive Fiction"
}

routine LinesFromTop
{
!	if not (display.hasvideo or top_justified_check is special)
		return display.windowlines
!	else
!		return 2
}

!\ IsGlk
	Roody's note- I decided roodylib should have its own glk-detecting routine.
	Note: WILL NOT WORK FROM NON-MAIN WINDOW
\!
routine IsGlk
{
	return ((display.screenheight + 100) < display.windowlines)
}

! Added ClearWindow as a quick way to clear and reset the cursor
! without clearing the status (and other) windows

routine ClearWindow
{
	local a, relocate
#ifclear NO_ACCESSIBILITY
	a = cheap
#endif
	if not (a = 1)
	{
		relocate = (LinesFromTop ~= display.windowlines)
		cls
		if relocate
			locate 1, LinesFromTop
	}
}

!----------------------------------------------------------------------------
!* BETA compiles
!----------------------------------------------------------------------------

! Roody's note: This used to be its own extension, but I figured I'd just throw
! it into Roodylib.  This prompts players with a "start transcripts now?"
! question as soon as they open the game and keeps track of whether a
! transcript is in progress despite restarts or restores.

attribute playback_on alias switchedon
attribute record_on alias workflag
attribute transcript_on alias special
!attribute skip_pauses alias light

object betalib "betalib"
{
	type settings
	in init_instructions
	did_print 0
#ifset BETA
	usage_desc
	{
		Indent
		"Betatesters: When a transcript is on (";
		if system(61) ! simple
			print "\"";
		else
		 print "\#147";
		 print "\BSCRIPT ON\b\"), preface your
		notes with an asterisk (";
		if system(61) ! simple
			print "\"";
		else
		 print "\#147";
		 "\B*\b\") to have your comments accepted."
	}
#endif ! ifset BETA
	save_info
	{
		local a, b,c
		if self is workflag
			a = SaveWordSetting("record")
		if self is special
			b = SaveWordSetting("beta")
		if self is switchedon
			c = SaveWordSetting("playback")
		if not (a + b + c)
		{
			if word[1] = "restore"
				SaveWordSetting("none")
			else
				return false
		}
		return true
	}
	execute
	{
		BetaInit
	}
}

routine BetaInit
{
	if CheckWordSetting("record")
		betalib is workflag
	elseif word[LAST_TURN] = "restore"
		betalib is not workflag
	if CheckWordSetting("beta")
		betalib is special
	elseif word[LAST_TURN] = "restore"
		betalib is not special
	if CheckWordSetting("playback")
		betalib is switchedon
	elseif word[LAST_TURN] = "restore"
		betalib is not switchedon
#ifset BETA
	if not (word[LAST_TURN] = "undo","restore") and betalib is not special
	{
		RLibMessage(&BetaInit,1) ! Would you like to start a transcript?
!: fancy pause stuff below
		""
		local a
		a = GetKeyPress
		if a = 'b','B'
		{
			if (not scripton)
				VMessage(&DoScriptOnOff, 1)      ! "Unable to begin..."
			else
			{
				betalib is special
				RlibMessage(&BetaInit, 2)      ! "Starting beta transcript for
				                               !  <GAME TITLE>"
			}
		}
		else
			RlibMessage(&BetaInit,3) ! "No transcript started."
		""
		CoolPause(0,true)
		betalib.did_print = true
	}
#endif ! ifset BETA
}

!----------------------------------------------------------------------------
!* ACCESSIBILITY
!----------------------------------------------------------------------------

! Roody's note: Turning on the cheap system in a game forces a limited display.
! The intention is that this would be useful for games playing with crappy
! displays, like the DOS simple port or the Floyd bot on ifMUD.
! "cheap on" stops the statusline from printing altogether, while "cheap menus"
! forces only newmenu's simple menu printing system.

#ifclear NO_ACCESSIBILITY
global cheap

object cheaplib "cheap"
{
	type settings
	in init_instructions
	save_info
	{
		local a
		a = SaveWordSetting("cheap")
		select cheap
			case 0 : a = SaveWordsetting("off",a)
			case 1 : a = SaveWordSetting("on",a)
			case 2 : a = SaveWordSetting("menus",a)
		if prompt = "Your command... "
			word[(a-1)] = "readerprompt"
!		word[(a-1)] = prompt
		return true
	}
	execute
	{
		local a,b
		b = cheap
!		if word[LAST_TURN] ~= "undo" and word[LAST_TURN] ~= "restore"
!		{
!			local simple_port
!			simple_port = (not IsGlk and system(61)) ! non-glk simple port
!			if simple_port
!				cheap = 1
!		}
		a = CheckWordSetting("cheap")
		if a
		{
		if not promptbackup
			promptbackup = prompt
!			if word[(a+1)]
!				prompt = word[(a - 2)]   ! + 1)]
			select word[(a-1)] ! +2)]
				case "off" : cheap = 0
				case "on" : cheap = 1
				case "menus" : cheap = 2
			if word[(a-2)] = "readerprompt"
				prompt = "Your command... "
		}

		if (cheap & CHEAP_ON) and not b ! and word[LAST_TURN] ~= "restart"
			DrawCheap
	}
	usage_desc
	{
		local simple_port
		simple_port = (not IsGlk and system(61)) ! non-glk simple port
		if simple_port
		{
			"\BPROMPT\b - Toggles between the > prompt and a \"What now? \"
			prompt."
			"\BSTATUS\b - Prints the information in the status bar in the main
			window."
			return
		}
		Indent
		"\BPROMPT\b - Toggles between the > prompt and a \"Your command... \"
		prompt."
		Indent
		"\BSTATUS\b - Prints the information in the status bar in the main
		window."
		Indent
		"\BCLEARSCREEN OFF\b - Any in-game screen-clearing is ignored. This
		might work with some screen readers and also targets console interpreters
		where screen clearing doesn't work well anyway. Menu options are
		numbered."
		Indent
		"\BNORMAL\b - Reverts to allow in-game screen sclearing."

	}
}


routine DoAccessibility
{
	local b, simple_port
	b = cheap
	simple_port = (not IsGlk and system(61)) ! non-glk simple port
	if simple_port
	{
		"Hugo detects that this is a simple interpreter. As such, screen
		clearing is already turned off and menus (if any exist) are written
		as numerical lists."
		return
	}
	select word[1]
		case "normal"
		{
			if cheap
			{
				cheap = false
				print "Normal screen-clearing";
				" restored."
				if (b & CHEAP_ON)
					PrintStatusLine
			}
			else
			{
				print "Normal screen-clearing";
				" already set."			}
		}
		case "clear","clearscreen"
		{
			select word[2]
				case "normal"
				{
					if cheap
					{
						cheap = false
				print "Normal screen-clearing";
				" restored."
				if (b & CHEAP_ON)
							PrintStatusLine
					}
					else
					{
				print "Normal screen-clearing";
				" already set."
					}
				}
				case "never","off"
				{
					if not (cheap & CHEAP_ON)
					{
						cheap = true
				print "Screen-clearing disabled";
				"."
						DrawCheap
					}
					else
					{
						print "Screen-clearing already disabled."
					}
				}
		}
}

routine DoCheapOnOff
{
	local a = 2
	local b
	b = cheap
	while a < 4
	{
		select word[a]
			case "off"
			{
				if cheap
				{
					cheap = false
				print "Normal screen-clearing";
				" restored."
					if (b & CHEAP_ON)
						PrintStatusLine
				}
				else
				{
					print "Normal screen-clearing";
					" already set."
				}
			}
			case "on"
			{
				if not (cheap & CHEAP_ON)
				{
					cheap = true
				print "Screen-clearing disabled";
				"."
					DrawCheap
				}
				else
				{
					print "Screen-clearing already disabled."
				}
			}
			case "menu","menus"
			{
				if cheap ~= CHEAP_MENUS
				{
					cheap = CHEAP_MENUS
					"\"Simple menus\" mode on."
					if b
						PrintStatusLine
				}
				else
					"\"Simple Menus\" mode already on."
			}
		a++
	}
}

routine DoCheapHelp
{
	"To turn on \"cheap mode\", type \BCHEAP ON\b or
	\BCHEAPMODE ON\b. This is the default behavior. To turn cheap
	mode off, type \BCHEAP OFF\b or \BCHEAPMODE OFF\b."
}

routine DoCheapToggle
{
	local b
	b = cheap
	if cheap
	{
		cheap = 0
		RLibMessage(&DoCheapToggle,1) ! "'Cheap' mode toggled off."
		if (b & CHEAP_ON)
			PrintStatusLine
	}
	else
	{
		cheap = true
		RLibMessage(&DoCheapToggle,2) ! "'Cheap' mode toggled on."
		DrawCheap(true)
	}
}

routine DrawCheap ! this is basically InitScreen, slightly modified
{
	if not system(61)
	{
		color TEXTCOLOR, BGCOLOR, INPUTCOLOR
		Font(DEFAULT_FONT)
		window 0
	}
	CenterTitle(CheapTitle,0,1,1) ! See RoodyLib's CheapTitle routine
}

routine DoA11y
{
	DoAccessibilityHelp
}

routine DoAccessibilityHelp
{
	local simple_port
	simple_port = (not IsGlk and system(61)) ! non-glk simple port
	if simple_port
	{
		"Hugo detects that this is a simple interpreter. As such, screen
		clearing is already turned off and menus (if any exist) are written
		as numerical lists.\n"
		"The following commands may still be useful:\n"
	"\BPROMPT\b - Toggles between the > prompt and a \"What now? \" prompt."
	"\BSTATUS\b - Prints the information in the status bar in the main
	window."
		return
	}
!	"The following commands exist to make playing this game with screen
!	readers or console environments more convenient:\n"
	"Players using screen readers or an interpreter within a console
	environment may be interested in the following commands:\n"
	Indent
	"\BPROMPT\b - Toggles between the > prompt and a \"Your command... \" prompt."
	Indent
	"\BSTATUS\b - Prints the information in the status bar in the main
	window."
	Indent
	"\BCLEARSCREEN OFF\b - Any in-game screen-clearing is ignored. This
	might work with some screen readers and also targets console interpreters
	where screen clearing doesn't work well anyway. Menu options are numbered."

	"\nTo revert back to regular screen-clearing and menus, type \BNORMAL\b."
}

!\ Roody's note: DoPrompt exists so that players using screenreaders can
change the prompt to something that sounds better when spoken aloud. \!

global promptbackup

routine DoPrompt
{
	if not promptbackup
		promptbackup = prompt
	"Prompt toggled."
	if prompt = "Your command... "
		prompt = promptbackup
	else
		prompt = "Your command... "
}

!\ Roody's note: DoStatus is a command to relay the information in the
status line to the player; it's functionality is for the vision-impaired and
people using limited-function interpreters where status lines are bothersome
or not printed.

You may very well want to replace this routine and write some better sounding
text that suits your game. \!

routine DoStatus
{
	print "You are in the location, \"";
	print location.name;
#ifset AMERICAN_ENGLISH
	print ".\"";
#else
	print "\".";
#endif
	if STATUSTYPE or max_score
		print AFTER_PERIOD;
	if statustype
	{
		if STATUSTYPE = 2, 16,18
		{
			print "It is ";
			HoursMinutes(counter)
			if MAX_SCORE
			{
				if (STATUSTYPE & MILITARY_TIME)
					print ".";
				print AFTER_PERIOD ; "You ";
			}
			elseif (STATUSTYPE & MILITARY_TIME)
				print "."
			else
				""
		}
		elseif STATUSTYPE = 1,9,8
		{
			print "In "; number counter; " turn";
			if counter ~= 1
				print "s";
			print ", you ";
		}
		if MAX_SCORE or (STATUSTYPE = 1,9,8)
		{
			print "have scored "; number score; " point";
			if score ~= 1
				print "s";
			"."
		}
	}
	elseif MAX_SCORE
	{
		print AFTER_PERIOD;
		print "Your score is "; number score;"."
	}
	else
	{
		print AFTER_PERIOD;
		print "There ";
		if counter ~= 1
			print "have";
		else
			print "has";
		print " been "; number counter; " move";
		if counter ~= 1
			print "s";
		print "."
	}
	print newline
}
#endif ! NO_ACCESSIBILITY
!----------------------------------------------------------------------------
!* PREPARSE CODE
!----------------------------------------------------------------------------
! Roody's note: Created PlayerPreParseOverride only
!  because Parse was running out of local variables and
!  I wanted to keep the code semi-legible
routine PlayerPreParseOverride
{
	local temp_verb, temp_obj, temp_xobj, ret

	temp_verb = verbroutine
	temp_obj = object
	temp_xobj = xobject
	verbroutine = &PreParse
	object = nothing
	xobject = nothing
	if player.before
	{
		parser_data[PARSER_STATUS] = 0
		ret = true
	}
	verbroutine = temp_verb
	object = temp_obj
	xobject = temp_xobj
	return ret
}

routine PreParseInstructions
{
	local i , p, r
	for i in preparse_instructions
	{
		p = i.execute
		r = r | p
	}
	return r
}

object preparse_instructions
{
	in settings
}

!#ifclear NO_FANCY_STUFF

! Roody's note: We do some parser trickery because >REMOVE BLANK usually
! defaults to unheld items, so we change >REMOVE ALL (which usually applies
! to worn items), to >TAKE OFF ALL

#ifclear NO_WEARALL
object parse_remove
{
	in preparse_instructions
	type settings
	execute
	{
		if word[1] = "remove" and word[2] = "~all"
		{
			if CurrentCommandWords ~= 2
				return false
			word[1] = "take"
			InsertWord(2,1)
			word[2] = "off"
			return true
		}
		return false
	}
}
#endif

object parse_orders
{
	in preparse_instructions
	type settings
	execute
	{
		local a, command_words
		if word[1] = "tell", "order", "instruct", "ask", "command"
		{
			for (a=3; a<=words; a++)
			{
				if word[a] = "to" and VerbCheck(word[(a+1)])
				{
					command_words = CurrentCommandWords
					word[a] = "~and"
					DeleteWord(1)   ! "tell", "order", etc.
					OrdersPreParse(a+1,command_words)
					return true
				}\
				if word[a] = "", "then"
					break
			}
		}
		elseif betalib is switchedon and word[1] = "record" and
			word[2] = "off"
		{
			word[1] = "playback"
			return true
		}
		return false
	}
}

!----------------------------------------------------------------------------
!* LAST TURN SUCCESS GLOBAL
!----------------------------------------------------------------------------
! Roody's note: Some code occasionally needs to know if the last turn was
! successful and returned true (for example: the #SHOW_COMMANDS code used
! for printing commands from multi-command lines.  The following makes it
! possible.

!parser_data[LAST_PARSER_STATUS] |= LAST_TURN_TRUE_F

event
{
	parser_data[LAST_PARSER_STATUS] |= LAST_TURN_TRUE_F
#ifclear NO_DISAMB_HELP
	if disamb_holder is special
	{
		local i
		i = 1
		while i < oldword[0]
		{
			if oldword[i] = ""
			{
				RestorefromOldWord(i)
				break
			}
			i++
		}
		disamb_holder is not special
		! because of an obscure case where someone uses the
		! engine disambiguation so Perform doesn't properly set
		! pronouns, we'll clear this so Parse will set pronouns
		! to the last object
		parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
	}
#endif
}

!----------------------------------------------------------------------------
!* REPLACED OBJLIB.H CODE
!----------------------------------------------------------------------------
#ifclear NO_OBJLIB

! Roody's note: if NEW_ROOMS is set, Roodylib sets the "first_visit" property
! to the game counter when a room is first visited.  This allows Roodylib
! to check if it's the first turn in a room after an UNDO or RESTORE and give
! the initial_desc instead of a long_desc.

#ifset NEW_ROOMS
property first_visit
replace room
{
	first_visit 0 0
	type room
	is static, light, open
}

!\ RoomTurnCount keeps track of the number of consecutive turns spent in
the current room. This exists so authors don't have to use the misc property
to keep track of turn count for each_turn stuff. \!

routine RoomTurnCount
{
	return (counter - location.first_visit #2)
}

!\ FirstVisit returns true if the current turn is also the first time
in the room (in case you want different behavior on other visits). \!

routine FirstVisit
{
	return (location.first_visit = location.first_visit #2)
}
#endif

! Roody's note: characters now default to being excluded from >ALL
!  (suggested by Paul Lee)
!\
	Also: I added a default response for when you grab objects from friendly
	characters. Change for your characters if you want a different message.
	If you want to not allow grabbing objects from friendly characters
	altogether, stop the action with a "parent(object) DoGet" before routine.

	This default code also allows >GET ALL FROM <character>

	To disallow that, replace the ExcludeFromAll routine so it always returns
	true when the parent is living. You'll also want to provide NewParseError
	case 9 with a special message that checks that the parent is living
	and returns with something like "You'll have to specify one object at a
	time."
\!

replace character
{
	type character
	pronouns "he", "him", "his", "himself"
	capacity 50
	holding 0
	is living, transparent, static
	exclude_from_all true
	after
	{
		xobject DoGet
		{
			RLibOMessage(character, 1) ! "so-and-so allows you to take the <object>"
		}
	}
}

!\Roody's note: Now, doors should automatically open if you try to enter them
and they are locked and you have a key. Doors are also assumed to be hinged
(see before.DoPush/DoPull section). Also some parse rank stuff added, although
I wonder if I'm playing with fire with that one.

\!
replace door "door"
{
	type door
	door_to                            ! depends on actor location
	{
		local currentroom
		if actor = player
			currentroom = location
		else
			currentroom = parent(actor)

		! sitting on or in something
#ifclear USE_VEHICLES
		if actor = player and player not in location
#else
		if actor = player and player not in location and
			parent(player).type ~= vehicle:
#endif
		{
			! "You'll have to get out first..."
			RlibMessage(&DoGo, 1)      ! "You'll have to get up..."
			general = 2  ! make a note that we printed a message
			return true
		}

		if self is not open and actor = player
		{
#ifclear NO_VERBS
			local verb_check
#ifset USE_VEHICLES
			if verbroutine = &DoMoveInVehicle
				verb_check = true
#endif
			if verbroutine = &DoPushDirTo
				verb_check = true
			if verbroutine = &DoGo : verb_check = true

			if verb_check
			{
				if player not in location
				{
					OMessage(door, 4)       ! "It is closed."
					return true
				}

				if self is not locked and self is openable:
				{
#ifclear SKIP_DOORS
					! check to see if Perform has already been called
					if parser_data[PARSER_STATUS] = 128
						RlibOMessage(door,1) ! "(and then opening it)"
					else
						OMessage(door, 2)       ! "(opening it first)"
					Perform(&DoOpen, self)
					Main    ! counts as a turn
					if self is not open
	!					Perform(&DoGo, self)
						return true
					local d
					if currentroom = self.between #1
						d = self.between #2
					else
						d = self.between #1
					if verbroutine = &DoPushDirTo
					{
						""
						return d
					}
					if not FindLight(d) or (FORMAT & DESCFORM_I)
						""
#endif   ! SKIP_DOORS
				}

				if self is lockable and self is locked
				{
#ifclear NO_AUTOMATIC_DOOR_UNLOCK
					local k,a
					if self.key_object
					{
						for (a=1; a <= self.#key_object ; a++)
						{
							if Contains(player, self.key_object #a)
							{
								k = self.key_object #a
								if k is not quiet
									break
							}
						}
					}
					if (k and k is quiet) or not k
					{
						OMessage(door, 3)      ! "It is locked."
						return true
					}
					RlibOMessage(door, 2,k)      ! "(unlocking <the blank> first))"
					Perform(&DoUnlock, self,k)
					if self is not locked
					{
						Main
						""
						if verbroutine = &DoPushDirTo
							Perform(&DoPushDirTo, object, xobject)
						else
							Perform(&DoGo, self)
						return true
					}
					else
#else
					OMessage(door, 3)      ! "It is locked."
#endif
						return true
				}
				elseif self is not open
				{
#endif
#ifclear SKIP_DOORS
					OMessage(door, 4)       ! "It is closed."
					return true
#endif ! SKIP_DOORS
#ifclear NO_VERBS
				}
			}
#endif
		}

		if self is not open and (actor in location or
			location = self.between #((parent(actor) = \
				self.between #1) + 1))
		{
			! Notify the player that the player or a character
			! has gone through the door:
			!
			RlibOMessage(door, 3)
			general = 2
		}

		if currentroom = self.between #1
			return self.between #2
		else
			return self.between #1
	}
	long_desc
	{
		if self is open
			OMessage(door, 6)       ! when open
		else
			OMessage(door, 7)       ! when closed
	}
	is openable, static

#ifset VERBSTUBS
!\ DoPush, DoPull - this version of the door class has normal hinged doors.
between #1 is the "exterior door" position; pushing opens the door, while #2
is the interior, where pushing closes. If you want your door to emulate
other behavior (swinging doors, whatever), you might want to change this.
\!
	before
	{
! DoOpen and DoClose always return true
		object DoPush
		{
			if location = self.between
				return Perform(&DoOpen, self)
			else
				return Perform(&DoClose, self)
		}
		object DoPull
		{
			if location = self.between
				return Perform(&DoClose, self)
			else
				return Perform(&DoOpen, self)
		}
	}
#endif
! Not sure if this next section is necessary. Added it to deal with a
! weirdly-coded game that messed with scope a lot.
!
! Doors in your location now have a higher priority than other doors
! this might turn out to be really bad design, though. we'll see.
!	parse_rank
!	{
!		if Inlist(self,between,location)
!		{
!			return 1
!		}
!		else
!			return -1
!	}
}

!\ Roody's note: replaced the female_character character class so it is of
type "character" (like a regular character), as you can always check for the
female attribute if you are specifically looking for a female character\!

replace female_character "(female_character)"
{
	inherits character
	pronouns "she", "her", "her", "herself"
	is female
}


!\ Roody's note: replaced the player_character object so it
can be of type "player_character", as I feel authors will often
want to distinguish playable characters from NPC's \!

#ifset AUTOMATIC_EXAMINE
attribute examined
#endif

replace player_character
{
	! 'nouns' and 'pronouns' as defined below are the defaults
	! for second-person, i.e., when player_person is 2--'name'
	! should be "you"
	!
	nouns "me", "myself"
	pronouns "you", "you", "your", "yourself"
	type player_character
	long_desc
		print "Looking good."
#ifset LIST_CLOTHES_FIRST  ! list worn items before listing rest of inventory
	list_contents
		return ListClothesFirst(self)
#endif
	capacity 100
	holding 0
	is hidden, living, transparent, static
	is plural       ! because player_person defaults to 2
}

#ifset LIST_CLOTHES_FIRST

! Roody's note: If this switch is set, inventory listings begin with worn
! items. To use, set the character's list_contents property to like it is
! in the player_character object above.

routine ListClothesFirst(char)
{
	local x,w,c, list_save, v
	v = verbroutine
	verbroutine = &ListClothesFirst
	list_save = list_count
	list_count = 0
	for x in char
	{
		if x is worn and x is clothing
		{
			w++
			list_count++
		}
		elseif x is not hidden
		{
			x is already_listed
			c++
		}
	}
	if w and c
		list_nest = 1
	if list_count
	{
		if v ~= &DoInventory
			Indent
		if v = &DoLook
			FORMAT = FORMAT | USECHARNAMES_F
		RLibMessage(&ListClothesFirst,1,char) ! "You are wearing"
		ListObjects(char)
		if (FORMAT & USECHARNAMES_F)
			FORMAT = FORMAT & ~USECHARNAMES_F
		else
			FORMAT = FORMAT & ~USECHARNAMES_F
		list_nest = 0
	}
	for x in char
	{
		if x is not worn or (x is not clothing and x is worn)
			x is not already_listed
		else
		{
			AddSpecialDesc(x) ! tags worn items as already_listed
		}
	}
	verbroutine = v
	list_count = list_save - w
	return (not c and w)
}
#endif ! #ifset LIST_CLOTHES_FIRST

! Roody's note: Fixes a bug where talking to 2 NPCs (by Mike Snyder).
replace self_class
{
	type self_class
	before
	{
		object
		{
			Perform(verbroutine, self.self_object, xobject)
		}
		xobject
		{
			if object > themselves
			{
				! Rule out numbers used as xobjects:
				local i
				for (i=words; i>2; i--)
				{
					if word[i] <= 0 or StringToNumber(word[i]) = xobject
					{
						return false
					}
				}

				if not self.self_resolve
				{
					OMessage(self_class, 1)
				}
				else
				{
                                !       (the old code)
				!	Perform(verbroutine, object, self.self_object)
					Perform(verbroutine, object, object)
				}
			}
			else
				return false
		}
	}
	is known
}

!\ We also have to replace the himself, herself, itself, and themself classes,
as they all inherit from the self class we have replace about. \!

replace himself "himself"
{
	in self_class
	inherits self_class
	noun "himself"
	self_object:	return him_obj
	self_resolve
	{
		if object is plural or object is female or object is not living
			return false
	}
}

replace herself "herself"
{
	in self_class
	inherits self_class
	noun "herself"
	self_object:	return her_obj
	self_resolve
	{
		if object is plural or object is not female or object is not living
			return false
	}
}

replace itself "itself"
{
	in self_class
	inherits self_class
	noun "itself"
	self_object:	return it_obj
	self_resolve
	{
		if object is plural or object is living
			return false
	}
}

replace themselves "themselves"
{
	in self_class
	inherits self_class
	noun "themselves"
	self_object:	return them_obj
	self_resolve
	{
		if object is not plural
			return false
	}
}

#ifset USE_VEHICLES

! Roody's note: New vehicle class for making it easier to have vehicles where
! DOWN or OUT is not the standard direction for exiting; works with new DoGo
! routine

! Example of a vehicle that allows >DOWN and >OUT to exit
!	before
!	{
!		parent(player) DoGo
!		{
!			if object = d_obj, out_obj
!				return object
!			return false
!		}
!	}

replace vehicle
{
	type vehicle
	vehicle_verb "drive"            ! default verb
	prep "in", "out"                !   "     prepositions
	vehicle_move true               ! by default, always ready to move

#ifclear NO_VERBS
	before
	{
		parent(player) DoGo
		{
#ifset SMART_PARENT_DIRECTIONS
			local a
			a = CanGoDir
			if not a
				VMessage(&DoGo, 2)      ! "You can't go that way."
			elseif a = 1
				RlibMessage(&DoGo, 1)      ! "You'll have to get up..."
			else
#endif
			! "To walk, you'll have to get out..."
			OMessage(vehicle, 1, self)
		}
	}
#endif
	is enterable, static
}

replace DoMoveinVehicle
{
	local v, moveto

	if player in location
	{
		RlibOMessage(vehicle, 2)    ! "You aren't in anything."
		return false
	}

	v = parent(player)              ! the vehicle
	if v.type ~= vehicle
	{
		RlibOMessage(vehicle,3) ! "Good luck with that."
		return false
	}
	if v.before:  return true

	! Match the verb
	if not InList(v, vehicle_verbs, VerbWord)
	{
		OMessage(vehicle, 3, v)        ! wrong verb for this vehicle
		return false
	}

	if not object or (object = v and player in object)
	{
		OMessage(vehicle, 4)   ! "Specify a direction as well..."
		return false
	}

	if not v.vehicle_move:  return true

	if obstacle
	{
		OMessage(vehicle, 5, v) ! "X stops you from moving."
		return true
	}

	if object.type ~= direction
	{
		moveto = object.door_to
		if not moveto
			! "You can't (drive) in there..."
			OMessage(vehicle, 6, v)

		if moveto <= 1
			return
	}
	else
		moveto = location.(object.dir_to)

	if not InList(moveto, vehicle_path, v) and moveto ~= 1
	{
		OMessage(vehicle, 7, v) ! "You can't (drive) that way."
		return false
	}
	elseif moveto = 0
	{
		if not location.cant_go
			OMessage(vehicle, 7, v)
		return false
	}
	elseif moveto = 1               ! already printed message
		return false            !   (moveto is never 1)

#ifset USE_ATTACHABLES
	if ObjectisAttached(v, location, moveto)
		return false
#endif

	! Finally, the vehicle can move
	move v to moveto
	v is moved
	old_location = location
	location = moveto

#ifset USE_ATTACHABLES
	MoveAllAttachables(v, old_location, location)
#endif

	if not FindLight(location)
		DarkWarning
	else
	{
		if not event_flag
			event_flag = true
		DescribePlace(location)
		location is visited
	}

	run parent(player).after

	return true
}
#endif ! #ifset USE_VEHICLES

#ifset USE_PLURAL_OBJECTS

! ParsePluralObjects is called by the Parse routine.
! Roody's note- just got rid of a jump
replace ParsePluralObjects
{
	local i, j, k, n, w, wn
	local any, plist, pobj, plural_type, loc, xobj, pobj_number
	local 2ndpass

! Before doing anything, move all plural/identical object classes back to
! their respective holding classes:

	for (i=0; i<plural_count; i++)
		move plurals[i] to plurals[i].type

	if actor = 0:  actor = player

	pluralobj_heldmode = 0  ! i.e. not explicitly held
	wn = 2                  ! starting word number
	i = 3                   ! make sure we run the following once

	while wn < i
	{
		pobj_number = 0         ! default explicit count (pluralobj_number)

	! The first step is to scan the phrase for a potential plural object word:

		for (w=wn; w<=words; w++)
		{
			if word[w] = 0 or w>words
				break           ! end of phrase

			if word[w] = "my"
			{
				pluralobj_heldmode = 1
			}
			local doublecheck
			for (n=0 ; n < plural_count; n++)
			{
				plist = plurals[n]
				doublecheck = 0
				if InList(plist, single_noun, word[w])
				{
					plural_type = single_noun
					doublecheck = true
				}

				if InList(plist, noun, word[w]) and not doublecheck
				{
					plural_type = noun
					doublecheck = true
				}
				! if this is the last plural object being checked, we already know it's correct
				! and if there are only two words, we know there aren't any adjectives
				if doublecheck and (n+1) < plural_count and words > 2
				{

				! we will doublecheck the possible object by looking
				! at the word before the noun, making sure that it is
				! not an adjective only for another object

				! starting off with w - 1
					j = w - 1
					while true
					{
					! going through every plural class added to the plurals array, skipping the ones already
                                        ! processed (and the noun didn't match)
						for (k=(n+1); k < plural_count; k++)
						{
					!   see if any of the other plural classes are using the same noun
							if InList(plurals[k],plural_type, word[w])
							{
					!   then see if the current word is an adjective for that and not the current
                                        !     considered plural class
								if InList(plurals[k],adjective, word[j]) and
									not (InList(plist, adjective, word[j]))
								{
									doublecheck = false
									break
								}
							}
						}
						! at this point, if the current word is not an adjective for the doublechecked object,
						! it shouldn't be an adjective for any plural object, so we can proceed with this object
						if not (InList(plist, adjective, word[j])) or not doublecheck
							break
						else
						! the word we just tested was an adjective for the doublechecked object (and possibly more)
						! so we will now check the previous word
							j--
					}
				}
				if doublecheck
				{
					pobj = plist
					if pobj in identical_class and plural_type = single_noun
						pobj_number = 1
					break
				}
			}

			if pobj:  break
		}
		if not pobj:  return false      ! didn't find anything

	! At this point, <w> points to the word and <pobj> is the object number of
	! the plural class.  <plural_type> is either noun or single_noun.  What
	! follows is the actual parsing of the plural structure:

	! Determine (roughly) if object is explicitly held or notheld; the
	! SetVerbHeldmode routine checks to see if the verb necessarily implies a
	! specific heldmode by checking the first 6 words--i.e., even if the first
	! five represent an object name in an actor-directed command.

		if not 2ndpass
		{
			for (i=1; i<=6 and i<=w; i++)
			{
				if pluralobj_heldmode:  break

				if word[i] = "":  break         ! end of phrase

				SetVerbHeldmode(word[i])
			}

	! If it is a single_noun (i.e. "any thing" or "either thing"), the object
	! global will have to be appropriately determined.  <pobj> will hold the
	! selected object (if any).  In any case, it will be necessary to know if
	! at least one object (in the case of multiple plural object specification)
	! is available, in order to move the plural_class into scope.

	! To boot, it could also get ugly if a container or platform xobject is
	! specified as the location for the object(s) being acted upon.  There is
	! no polite way of figuring out what the xobject might be at this point.

			for (i=w; i<words; i++)
			{
				k = word[i]             ! for shorthand purposes only
				if k = 0:  break        ! end of phrase

				if (k = "from", "outof", "offof", "off") or
					(pluralobj_heldmode ~= 1 and
					(k = "in" or k = "inside" or k = "on"))
				{
					xobj = 0        ! nothing selected yet
					do
					{
						i=i+1   ! get next word

						for (j=1; j<=pobj.#plural_of; j++)
						{
							n = parent(pobj.plural_of #j)
							if ObjWord(word[i], n)
								xobj = n
						}
					}
					while i<=words and word[i]~=0

					break
				}
			}

			n = 0
			if xobj
				loc = xobj
			elseif actor = player
				loc = location
			else
				loc = parent(actor)
		}

	! Remove the old plural_class adjective(s), if any

		while w > 1 and (ObjWord(word[w-1], pobj) = adjective or word[w-1] = "my")
		{
			if word[w-1] = "my"
			{
				! Try to differentiate between, for instance, "my door"
				! out of a set of door, and "my key", meaning the one
				! the player is holding:
				for (i=1; i<=pobj.#identical_to; i++)
				{
					if Contains(player, pobj.identical_to #i)
						break

				}
				if i > pobj.#identical_to
					break
			}

			DeleteWord(w-1)
			w--
		}

	! Then, find out if there is a preceding number

		if w > 1
		{
!#ifclear NO_DISAMB_HELP
!! This might cause trouble with similarly named plural classes, but
!! this is the best fix in the meantime for a clash between the disamb
!! system and the plural class checking for number words
!			if disamb_holder is not special
!#endif
			k = WordisNumber(word[w-1])
			if k >= 1                       ! e.g., "one (or more) things"
			{
				DeleteWord(w-1)
				w--
				pobj_number = k
			}

			if WordisNumber(word[w-1])>=1   ! "two of the three"
			{
				pobj_number = WordisNumber(word[w-1])
				DeleteWord(w-1)
				w--
			}
		}

	! Remove a preceding "all" or "any"; i.e. "all things" is the same as "things"

		if w>1 and (word[w-1] = "~all" or word[w-1] = "~any")
		{
			if word[w-1] = "~any":  any = true
			DeleteWord(w-1)
			w--
		}

	! Now see if the object (or a member of the plural_class) is available given
	! the restrictions of pluralobj_heldmode:

		n = 0
		j = 0

		if pluralobj_heldmode = -1              ! explicitly notheld object
		{
			for (i=1; i<=pobj.#plural_of; i++)
			{
				k = pobj.plural_of #i
				k is not workflag

				if k in loc
				{
					n = k
					k is workflag
					j++
				}
				if n and plural_type = single_noun:  break
			}
		}

		if not n or pobj_number = 0 or j < pobj_number
		{
			for (i=1; i<=pobj.#plural_of; i++)
			{
				k = pobj.plural_of #i
!  Checking for a k value so FindObject is never run on 0
				if k
				{
					! explicitly notheld object
					if pluralobj_heldmode = -1 and k is not workflag
					{
						if FindObject(k, loc) and k not in actor
						{
							n = k
							k is workflag
							j++
						}
					}
					elseif pluralobj_heldmode ~= -1
						k is not workflag

					! explicitly held object
					if pluralobj_heldmode = 1
					{
						if k in actor
						{
							n = k
							k is workflag
							j++
						}
					}

					! or neither
					elseif pluralobj_heldmode = 0
					{
						if FindObject(k, loc)
						{
							n = k
							k is workflag
							j++
						}
					}
				}

				if n and plural_type = single_noun:  break
				if j and j = pobj_number:  break
			}
		}

	! Even if no object is found, it will be necessary to insert one in the
	! input line to generate a "You don't see that"-esque parser message in
	! the case of a single_noun specification.

		if plural_type = single_noun and not n
			n = pobj.plural_of #1

	! If it's not a plural specification--i.e. it is an "any" or "either"
	! phrasing instead--change the plural object word to represent <n>.

		i = words

		if plural_type = single_noun
		{
			if pobj.type ~= identical_class and any
			{
				SetObjWord(w, n)
				if pobj.type = plural_class
					print "("; The(n); ")"
			}
			elseif pobj.type = identical_class
			{
				SetObjWord(w, pobj)

				if pluralobj_heldmode = 1       ! in inventory
					move pobj to actor
				elseif xobj                     ! in or on something
					move pobj to xobj
				else                            ! anything else
					PutinScope(pobj, actor)
			}
		}

	! If it is a plural specification--plural_class.noun instead of single_noun--
	! move the plural class into an accessible position, but only if one of
	! the component members is available.

		elseif n and plural_type = noun
		{
			! Insert the name of the identical_class, if necessary
			if pobj.type = identical_class
				SetObjWord(w, pobj)

			if pluralobj_heldmode = 1       ! in inventory
				move pobj to actor
			elseif xobj                     ! in or on something
				move pobj to xobj
			else                            ! anything else
				PutinScope(pobj, actor)
		}

		pobj.pluralobj_number = pobj_number     ! # of plural objects

		wn = w + pobj.#adjectives + (pobj.noun > 0)
		if wn < i
		{
			pobj = 0
			2ndpass = true
		}
	}
	parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET  ! since we rebuilt object(s)

	return true
}

! fixes a typo in original
replace AddIdentical(obj, i)
{
	local j

	for (j=1; j<=i.#plural_of; j++)
	{
		if i.plural_of #j = 0           ! a blank slot
		{
			i.plural_of #j = obj    ! add it
			return j
		}
	}
	return false
}

replace WordisNumber(w)
{
	select w
	case "zero", "0":       return 0
	case "one", "1":        return 1
	case "two", "2":        return 2
	case "three", "3":      return 3
	case "four", "4":       return 4
	case "five", "5":       return 5
	case "six", "6":        return 6
	case "seven", "7":      return 7
	case "eight", "8":      return 8
	case "nine", "9":       return 9
	case "ten", "10":       return 10
	case -1
	{
		local a
		a = StringToNumber(parse$)
		if a
			return a
	}
	return -1
}
#endif ! #ifset USE_PLURAL_OBJECTS

#ifset USE_ATTACHABLES
!\ Roody's note: I had to add the "k not in newloc" check so that it doesn't
try to re-move objects that have already been moved.  Maybe this will break
something since it's hard to keep track of everything that's going on, but
cursory testing seems to work ok.  \!
replace Attachable_MoveChildren(obj, oldloc, newloc)
{
	local i, k, first

	for i in obj
	{
		k = Attachable_MoveAttached(i, obj, oldloc, newloc)
		if not first and k not in newloc:  first = k
	}
	return first
}

!\ This routine only has support for one object being pushed which I figure
   works for most games, but I still gave the system its own routine in case
	anyone needs to code a more complex system. \!
routine CheckForAttachments(obj,loc)
{
	local i, j

	for i in loc
	{
		for (j=1; j<=i.#attached_to; j++)
		{
			if i.attached_to #j = obj or
				Contains(obj, i.attached_to #j):
			{
				return i
			}
		}
	}
	for i in player
	{
		for (j=1; j<=i.#attached_to; j++)
		{
			if i.attached_to #j = obj or
				Contains(obj, i.attached_to #j):
			{
				return i
			}
		}
	}
	return 0
}

! Roody's note: Just replaced so that >TIE OBJECT TO ME gets a "you can't tie
! that to yourself." message instead of "you can't tie that to you."
! message
replace DoAttachObject
{
	if object.type ~= attachable
	{
		OMessage(attachable, 6)        ! "You can't (attach) that."
		return false
	}

	! Match the verb
	if not InList(object, attach_verbs, VerbWord)
	{
		OMessage(attachable, 7, object.attach_verb)    ! wrong verb
		return false
	}

	if not xobject
	{
		OMessage(attachable, 8)        ! "Be more specific..."
		return false
	}

	if not CheckReach(xobject)
		return false

#ifclear NO_VERBS
	if object.attach_take and object not in player
	{
		OMessage(attachable, 9)         ! "(taking it first)"
		Perform(&DoGet, object)
		if object not in player
			return false
		Main                    ! counts as a turn
	}
#endif

	! See if the attach-to object is appropriate
	if object.attachable_to and not InList(object, attachable_to, xobject)
	{
		! "You can't (attach) it to that."
		RLibOMessage(attachable, 2)
		return false
	}

	! Make sure it's not already attached
	if InList(object, attached_to, xobject)
	{
		! "Except that it is already (attached) to that."
		OMessage(attachable, 11)
		return false
	}

	if not ObjectAttach(object, xobject)
	{
		! "Except that it is already (attached) to (list)..."
		OMessage(attachable, 12)
		return false
	}

	if not xobject.after
	{
		if not object.after
			! "You (attach) it to..."
			OMessage(attachable, 13)
	}

	return true
}

! ObjectisAttached returns true if something in <oldloc> is keeping
! <obj> from moving to <newloc>.

! Roody's note- got rid of a couple jumps and added a special response for
! pushed-objects-with-attachments
replace ObjectisAttached(obj, oldloc, newloc)
{
	local check

	if oldloc = newloc
		return false

! Check the children of the object in question:

	check = Attachable_CheckChildren(obj, newloc)
	if not check
! Failing finding any attachments there, use the roundabout method of
! checking the location:
		check = Attachable_CheckAttachedTo(obj, oldloc, newloc)
	if not check
		return false                    ! nothing is stopping it


	if check = -1                   ! something already printed
		return true

#ifclear NO_VERBS

#ifset USE_VEHICLES
	if verbroutine = &DoGo or verbroutine = &DoMoveinVehicle
#else
	if verbroutine = &DoGo
#endif
	{
		! "Not while it is still (attached) to..."
		OMessage(attachable, 18, check)
	}
	elseif verbroutine = &DoPushDirTo
		RLibOMessage(attachable, 1, check, object)
	! "You can't move the <blank> while it's attached to the <thing>"
	else
	{
#endif
		! "You can't move whatever is (attached) to it..."
		OMessage(attachable, 19, check)
#ifclear NO_VERBS
	}
#endif

	return true
}
#endif ! USE_ATTACHABLES
#endif ! ifclear NO_OBJLIB

!----------------------------------------------------------------------------
!* REPLACED RESOURCE.H CODE
!----------------------------------------------------------------------------
#ifset _RESOURCE_H
!\ Roody's note: We replace some of the resource.h routines so multimedia is
never played under "minimal" glk interpreters like Gargoyle. Now, our resources
won't be ripped from our games!

If you want to be benevolent and allow pictures and music under Gargoyle,
set the allow_gargoyle global to true.
\!

global allow_gargoyle

replace LoadPicture(resfile, pic)
{
	if not display.hasgraphics or (system(61) and not allow_gargoyle)
		return false

	if pic = ""
	{
		pic = resfile
		resfile = last_resourcefile
	}

	picture resfile, pic

	last_resourcefile = resfile

	if system_status:  return false

	return true
}

!\ PictureinText doesn't work at all in Gargoyle. If you DO want pictures
in Gargoyle, use LoadPicture \!

replace PictureinText(resfile, pic, width, height, preserve)
{
	local i

	if (not display.hasgraphics) or system(61):  return false

	Font(PROP_OFF)

	! Print blank lines the height of the picture, but only if images
	! aren't being routed to an external destination
	!
	if (display.hasgraphics ~= 2)
	{
		for (i=1; i<=height; i++)
			print ""
	}

	if width = 0:  width = 1
	if height = 0: height = 1

	! Create a properly sized window at the bottom-left corner of the
	! screen to load the picture in
	!
	window 1, (display.screenheight-height), width,
		(display.screenheight)
	{
		LoadPicture(resfile, pic)
	}

	! If any number of lines was given to preserve, temporarily define
	! a window of that height in order to make it the lowest-defined
	! window--i.e., the top of the main scrolling region
	!
	if preserve
	{
		window 0
		window preserve
		{}
	}

	print ""
	Font(DEFAULT_FONT)

	if system_status:  return false

	return true
}

replace PlayMusic(resfile, song, volume, loop, force)
{
	if (system(61) and not allow_gargoyle)
		return false
	! If the given song is already playing:
	if audio.current_music_resfile = resfile and
		audio.current_music = song
	{
		if not force
			return
		else
			music 0
	}

	local vol
	vol = volume
	if vol = 0
		vol = 100
	elseif vol < 0
		vol = 0

	if resfile = MUSIC_STOP and not song
	{
		music 0
		audio.current_music = 0
	}
	else
	{
		if not loop
			music resfile, song, vol
		else
			music repeat resfile, song, vol
		audio.current_music = song
		audio.current_music_looping = loop
	}

	audio.current_music_resfile = resfile
	last_resourcefile = resfile
	audio.current_music_volume = volume

	if system_status:  return false

	return true
}

replace PlaySound(resfile, s, volume, loop, force)
{
	if (system(61) and not allow_gargoyle)
		return false
	! Due to the nature of samples, always restart the sound
	force = true

	! If the given sound is already playing:
	if audio.current_sound_resfile = resfile and
		audio.current_sound = s
	{
		if not force
			return
		else
			sound 0
	}

	local vol
	vol = volume
	if vol = 0
		vol = 100
	elseif vol < 0
		vol = 0

	if resfile = SOUND_STOP and not s
	{
		sound 0
		audio.current_sound = 0
	}
	else
	{
		if not loop
			sound resfile, s, vol
		else
			sound repeat resfile, s, vol
		audio.current_sound = s
		audio.current_sound_looping = loop
	}

	audio.current_sound_resfile = resfile
	last_resourcefile = resfile
	audio.current_sound_volume = volume

	if system_status:  return false

	return true
}

routine CheckResourceMusic(resfile,song)
{
	if (system(61) and not allow_gargoyle)
		return false

	local vol, ret

	if check_resource_opcode is switchedon
	{
		ret = ExecOpCode(check_resource_opcode,song,resfile)
		return ret
	}
	vol = 0

	music resfile, song, vol
	ret =  not system_status
	music 0
	return ret

}

routine CheckResourceSound(resfile, s)
{
	if (system(61) and not allow_gargoyle)
		return false

	local vol, ret

	if check_resource_opcode is switchedon
	{
		ret = ExecOpCode(check_resource_opcode,s, resfile)
		return ret
	}

	vol = 0

	sound resfile, s, vol
	ret =  not system_status
	sound 0
	return ret

}

! Roody's note: This routine only works if the interpreter supports
! the check_resource_opcode opcode so you should check for that
! before calling
routine CheckResourceGraphic(resfile, g)
{
	if (system(61) and not allow_gargoyle) or not display.hasgraphics
		return false

	local ret

	if check_resource_opcode is switchedon
	{
		ret = ExecOpCode(check_resource_opcode,g, resfile)
		return ret
	}
}
#endif ! _RESOURCE_H

!----------------------------------------------------------------------------
!* REPLACED WINDOW.H CODE
!----------------------------------------------------------------------------
#ifset _WINDOW_H
! Roody's note: added the window_class type to help out object-tree printing
replace window_class
{
	win_title ""
	type window_class
	win_left	0		! default coordinates
	win_top		0
	win_right	0
	win_bottom	0

	win_textcolor
		return TEXTCOLOR
	win_backcolor
		return BGCOLOR
	win_titlecolor
		return SL_TEXTCOLOR
	win_titleback
		return SL_BGCOLOR

	win_active false

	win_init
	{
		local x, y, titlelen, width, height

		titlelen = string(_win_title_string, self.win_title, 64)

		height = self.win_bottom - self.win_top
		if height++ = 0:  height = 10
		if self.win_title:  height++

		width =  self.win_right - self.win_left
		if width++ = 0:  width = 20
		if titlelen > width
			width = titlelen+2

		! The default position is centered in the middle of
		! the screen
		!
		x = self.win_left
		if x = 0:  x = display.screenwidth/2 - width/2 + 1
		y = self.win_top
		if y = 0:  y = display.screenheight/2 - height/2 + 1

		! Check to make sure the window will actually fit on the
		! screen--and if it doesn't, make it
		!
		if x + width > display.screenwidth + 1
		{
			if width > display.screenwidth
				width = display.screenwidth
			x = display.screenwidth - width + 1
		}
		if y + height > display.screenheight + 1
		{
			if height > display.screenheight
				height = display.screenheight
			y = display.screenheight - height + 1
		}

		! Draw the window, in the supplied color(s), with or
		! without a static title:
		!
		window x, y, (x+width-1), (y+height-1)
		{
			color self.win_textcolor, self.win_backcolor
			cls

			if self.win_title ~= ""
			{
				locate 1, 1
				color self.win_titlecolor,
					self.win_titleback

				print to ((width-titlelen)/2); \
					self.win_title; to width
			}

		}

		! If necessary, redefine the window below the static
		! title:
		!
		if self.win_title ~= ""
		{
			window x, (y+1), (x+width-1), (y+height-1)
			{}			! empty window body
		}

		color self.win_textcolor, self.win_backcolor
		self.win_active = true
	}

	win_clear
	{
		if self.win_active
		{
			color self.win_textcolor, self.win_backcolor
			cls
		}
	}

	win_end
	{
		color TEXTCOLOR, BGCOLOR
		self.win_active = false
	}
}
#endif

!----------------------------------------------------------------------------
!* REPLACED CHECKHELD OBJECT
!----------------------------------------------------------------------------
! Roody's note: Made a checkheld_holder object to keep the checkheld object in
! so its parent is not the nothing object (as ResetCheckheld gets easily
! confused by unintended objects sharing a parent with checkheld.
!----------------------------------------------------------------------------
#ifset USE_CHECKHELD
object checkheld_holder
{}
! The checkheld limit constant as defined by verblib.h is not replaceable, so
! here is a replaceable version
#if undefined NEW_CHECKHELD_LIMIT
constant NEW_CHECKHELD_LIMIT 64
#endif

replace checkheld	! 'active' when active; 'plural' for "~all", etc.
{
	misc #NEW_CHECKHELD_LIMIT
	size 0		! # of managed objects being managed
	is hidden
	in checkheld_holder
}

replace ResetCheckHeld
{
	local i, obj
	for (i=1; i<=checkheld.size; i++)
	{
		obj = checkheld.misc #i
		if obj is checkheld_flag and obj in parent(checkheld)
			move obj to player
		obj is not checkheld_flag
	}
	checkheld.size = 0
	checkheld is not plural
	checkheld is not active
	move checkheld to checkheld_holder
}
#endif  ! #ifset USE_CHECKHELD

!----------------------------------------------------------------------------
!* REPLACED VERBLIB.H ROUTINES
!----------------------------------------------------------------------------

! Roody's note: This DoAskQuestion uses the DetermineSpeaking routine to
! guess the speaking global when it is not set
!----------------------------------------------------------------------------
replace DoAskQuestion
{
	if speaking = 0
	{
		DetermineSpeaking
		if speaking = 0
		{
			VMessage(&DoAskQuestion)        ! "Not talking to anybody..."
			return false
		}
	}
	if xobject
	{
		ParseError(6)
		return false
	}

	return Perform(&DoAsk, speaking, object)
}

! Roody's note: If there's only one person to speak to, make that person the
! obvious recipient of, e.g., a non-specific "ask about...".
! From Future Boy! (Kent Tessman)

routine DetermineSpeaking
{
	local i, spk, count

	for i in location
	{
		if i is living and i is not hidden
		{
			spk = i
			count++
		}
	}

	if count = 1:  speaking = spk
}

! Roody's note: added a special message call for the player object
replace DoDrink
{
	if not CheckReach(object):  return false

	if not object.after
	{
		if object = player
			RlibMessage(&DoDrink)   ! "You can't drink yourself."
		else
			VMessage(&DoDrink)         ! "You can't drink that."
	}
	else
		return true
}

! Roody's note: added a special message call for the player object
replace DoEat
{
	if not CheckReach(object):  return false

	if not object.after
	{
		if object = player
			RlibMessage(&DoEat)   ! "You can't eat yourself."
		else
			VMessage(&DoEat)         ! "You can't eat that."
	}
	else
		return true
}

! Roody's note: replaced so the container/platform check comes before the
! children check.
replace DoEmpty
{
	local a, b, obj, xobj
	local thisobj, nextobj

	CalculateHolding(object)

	if object is container, openable, not open
	{
		VMessage(&DoEmpty, 1)           ! "It's closed."
		return true
	}
	if object is not container, platform
	{
		ParseError(12, object)
		return false
	}
	if not children(object)
	{
		VMessage(&DoEmpty, 2)           ! "It's already empty."
		return true
	}

	thisobj = child(object)
	while thisobj
	{
		nextobj = sibling(thisobj)

		print thisobj.name; ":  ";

		if thisobj is static
			VMessage(&DoEmpty, 3)    ! "You can't move that."
		else
		{
			a = player.holding
			b = thisobj
			obj = object
			xobj = xobject

			if player not in location and
				(parent(player) is platform or
					parent(player) is container) and
				not xobject:

				Perform(&DoPutIn, b, parent(player))
			else
				Perform(&DoDrop, b)

			object = obj
			xobject = xobj
			player.holding = a
			if object.#holding
			{
				if b not in object
					object.holding = object.holding - b.size
			}
		}

		thisobj = nextobj
	}

	run object.after
	return true
}

! Roody's note: Mostly not changed. Just commented out the part where word[1]
! is cleared and made the for loop break earlier. Also tweaked one of the
! messages in RlibMessage so that >ENTER ME has a better response.
replace DoEnter
{
#ifclear NO_OBJLIB
	if not object or object in direction
#else
	if not object
#endif
	{
		local i, obj, count
		for i in location
		{
			if i is enterable
			{
				obj = i
				count++
				if count = 2
					break
			}
		}
		if count = 1
		{
			object = obj
		}
		else
		{
			VMessage(&DoEnter, 1)    ! "Be a little more specific..."
			return false
		}

		return Perform(&DoEnter, object)
	}

	if object = player
	{
		RlibMessage(&DoEnter, 1) ! "You can't enter yourself."
		return
	}
	! To prevent endless loops if the player_character class
	! automatically resets the object to in_obj if word[1] = "in"
!	word[1] = ""    ! causing problems

	if &object.door_to
		return Perform(&DoGo, object)   ! routine
	elseif object.door_to
		return Perform(&DoGo, object)   ! object

	if object is not enterable or Contains(player, object)
		RlibMessage(&DoEnter, 1)    ! "You can't enter that."
	elseif player in object
		VMessage(&DoEnter, 3)    ! already in it
	elseif player not in location
		RlibMessage(&DoGo, 1)       ! "You'll have to get up..."
	elseif object is openable, not open
		VMessage(&DoLookIn, 1)   ! "X is closed."
	else
	{
		move player to object
		if not object.after
			VMessage(&DoEnter, 4)    ! "You get in..."
		if (FORMAT & DESCFORM_I)
			""
		object is not quiet
		DescribePlace(location)
		return true
	}
	return
}

! Roody's note: Fixed a bug Juhana Leinonen found where "X is closed."
! messages weren't properly printing the parent's name.

replace DoExit
{
	local p
	if object = player
	{
		RlibMessage(&DoExit, 1) ! "Trying to get out of your head? Futile."
		return
	}
#ifclear NO_OBJLIB
	! >GO OUT winds up calling DoExit with object = out_obj, thanks to
	! the direction-parsing code in Perform().  English ambiguities being
	! what they are, we correct that interpretation of "out" here, and
	! treat the command as a generic instruction to exit whatever
	! container context we may be in.
	if object = out_obj
		object = nothing

	if object = nothing or object = location
	{
		if player in location and out_obj in direction
		{
			word[1] = out_obj.noun
			word[2] = ""
			return Perform(&DoGo)
		}
	}
	elseif object = d_obj and player in location
	{
		return Perform(&DoGo, object)
	}
#endif

	p = parent(player)

	if object and player not in object
		VMessage(&DoExit, 1)             ! "You aren't in that."
	elseif (p is openable, not open)
	{
		object = p
		VMessage(&DoLookIn, 1)           ! "X is closed."
	}
	else
	{
		if object = nothing
			object = p
		move player to location
		if not object.after
			RlibMessage(&DoExit, 2)      ! "You get out.."
	}
	return true
}

! Roody's note: Juhana found a bug where "X is closed." messages were
! triggered when trying to grab things from a closed enterable container
! the player ALSO is in. Added a quick fix but it's possible DoGet could
! be more efficient, as some of the instances will also be checked by
! CheckReach.
! Changed up the order of things being checked, redirected some clothing
! items to DoTakeOff, added a default "so-and-so is wearing that" message
! for taking other clothes, added optional Anchorhead-style automatic
! examining. Code could probably be cleaner but it should work.

replace DoGet
{
	local b, p

	if Contains(object, player)
	{
		if object is static
			VMessage(&DoGet, 7)	! "You can't take that."
		else
			VMessage(&DoGet, 6)     ! "Not while you're in/on it..."
		return false
	}
	if xobject
	{
		while true
		{
			if not CheckReach(xobject)
				return false
			elseif xobject = object and object ~= player
			{
				ParseError(6) ! That doesn't make any sense.
			}
			elseif object is clothing,worn ! assumes xobject is living because
			                               ! both clothing and worn are used
				return Perform(&DoTakeOff,object)
			else
				break
			return false
		}
	}
	if object in player
	{
		VMessage(&DoGet, 1)      ! "You already have that."
		return true
	}
	elseif object = player
	{
		VMessage(&DoGet, 2)     ! player trying to get player
		return false
	}
	elseif object is living and object is static
	{
		if CheckReach(object)
			VMessage(&DoGet, 3)     ! player trying to get character
		return false
	}

	p = parent(object)

	if p ~= location
	{
		if not CheckReach(p)
			return false
		if (p is openable, not open) and p is container and p ~= parent(player)
		{
			VMessage(&DoGet, 5)     ! "X is closed."
			return false
		}
		if p is living
		{
			if (object is clothing,worn)
			{
				RLibMessage(&DoGet,1,p) ! "so-and-so is wearing that."
				return true
			}
			if p is unfriendly
			{
				VMessage(&DoGet, 4)      ! "X doesn't want to give it to you."
				return true
			}
		}
	}
	if not CheckReach(object)
		return false
	elseif object is static
	{
		VMessage(&DoGet, 7)      ! "You can't take that."
		return true
	}

	! Because the engine calls location.before
	if (parent(object)~=location)
		b = parent(object).before

	if not b
	{
		xobject = parent(object)

		if object not in location
		{
			CalculateHolding(xobject)
			p = xobject
		}

		if Acquire(player, object)
		{
			object is not hidden

			if not object.after
			{
				! Again, to avoid duplication or
				! echoing:
				!
				b = 0
				if xobject ~= location
					b = xobject.after

				if b = false
				{
#ifset AUTOMATIC_EXAMINE ! unexamined objects are automatically examined
					if object is not examined and &object.long_desc
					{
						! "You pick up the <object>.";
						RLibMessage(&DoGet,2)
						print AFTER_PERIOD;
						Perform(&DoLook, object)
					}
					else
#endif
					! "Taken."
						VMessage(&DoGet, 8)
				}
			}
		}
		else
			! "You're carrying too much to take that."
			VMessage(&DoGet, 9)
	}
	return true
}

! Roody's note: I created a global to be set by DoGo called "exit_type"
! It keeps track of whether the successful direction used uses a door
! or direction. Mainly, this just exists so DescribePlace can have some
! kind of info saved in case you want to write code for specifying
! when pronouns should be assigned to room items without extensive
! word array checking.
global exit_type
constant non_door_portal 3

! Roody's note: Makes "you'll have to get up" message more container/platform
! specific also has some code to work with new vehicle replacement. Also got
! rid of jump. Also changed the answer for trying to go to a nearby,
! non-enterable object from "You can't enter that" to "<The object> is right
! here."
! Also, if USE_ELEVATED_PLATFORMS is set, enterable objects with the "elevated"
! attribute allow >DOWN as an exit.

replace DoGo
{
	local moveto, JumpToEnd, skip_ahead ! , vehicle_check
#ifset NO_OBJLIB
	local wordnum, m
#endif

	if object = parent(player) or ! make sure player isn't already in object
		not object ! or (not parent(player) and word[2] = "out")
		skip_ahead = true

	if player not in location and   ! sitting on or in an obj.
		not skip_ahead
	{
		local a, b

		b = parent(player).before	! i.e., a vehicle, etc.
		if b = 1
			return false ! so the error message doesn't take up a turn
#ifclear NO_OBJLIB
		elseif not b
		{
#ifset USE_ELEVATED_PLATFORMS
			if parent(player) is elevated and object = d_obj
				a++
			elseif parent(player) is platform and object = u_obj and
			parent(player) is not elevated
#else
			if parent(player) is platform and object = u_obj
#endif
				a++
			elseif parent(player) is container and object = out_obj
				a++
		}
#endif

		if	not a and not b
		{
#ifset SMART_PARENT_DIRECTIONS
			if not CanGoDir
				VMessage(&DoGo, 2)      ! "You can't go that way."
			else
#endif
			if general ~= 2
				RlibMessage(&DoGo, 1)      ! "You'll have to get up..."
			else
				general = 0
			return false
		}
		else
			return Perform(&DoExit,parent(player))
	}


	if obstacle and not skip_ahead
	{
#ifclear NO_OBJLIB
		VMessage(&DoGo, 1)		! "X stops you from going..."
		return true
#endif
	}

#ifclear NO_OBJLIB
	! See if the object is one of the current direction set

	if object.type = direction and object in direction
	{
		moveto = object
		JumpToEnd = true
	}
#endif

if not JumpToEnd
{
#ifclear NO_OBJLIB
	if not moveto
	{
#else
	wordnum = 1                             ! various phrasings
	if words > 1 and word[2] ~= ""
		wordnum = 2
	if word[2] = "to", "through"
		wordnum = 3
	elseif (word[2] = "in" or word[2] = "inside") and words > 2
		wordnum = 3

	select word[wordnum]
		case "north", "n":      m = n_to
		case "south", "s":      m = s_to
		case "east", "e":       m = e_to
		case "west", "w":       m = w_to
		case "northwest", "nw": m = nw_to
		case "northeast", "ne": m = ne_to
		case "southeast", "se": m = se_to
		case "southwest", "sw": m = sw_to
		case "up", "u":         m = u_to
		case "down", "d":       m = d_to
		case "in", "inside":    m = in_to
		case "out", "outside":  m = out_to

	if not m
	{
#endif
		if not object
		{
			! ParseError(6)  ! doesn't make any sense
			VMessage(&DoGo, 4)	! "Which way...?"
			return
		}

		if not &object.door_to and (word[2] ~= "to","toward","towards")
		{
			if word[2] = "out"
			{
				ParseError(6)  ! doesn't make any sense
				return
			}
			if not object.door_to and object is enterable
				return Perform(&DoEnter, object)
		}

		moveto = object.door_to
		if not moveto
		{
			if object is container and (word[2] ~= "to","toward","towards")
				VMessage(&DoEnter, 2)  ! "You can't enter..."
			else
				RLibMessage(&DoGo, 2) ! "The <object> is right here."
			return
		}
		if moveto = 1
			return true

#ifclear NO_OBJLIB
		if object.type = door
			exit_type = door
		else
			exit_type = non_door_portal
#endif
	}
}  !  if not JumpToEnd bracket

#ifclear NO_OBJLIB

!  may need to add some code for elevated platforms. not sure yet.

	if moveto.type = direction and moveto in direction
	{
		if player not in location and object = out_obj
		{
			if parent(player) is platform
			{
				RlibMessage(&DoGo, 1) ! "You'll have to get up."
				return false
			}
			else
				return Perform(&DoExit)
		}
		elseif player not in location and object = u_obj
		{
			if parent(player) is container
			{
				RlibMessage(&DoGo, 1) ! "You'll have to get out."
				return false
			}
			else
				return Perform(&DoExit, parent(player))

		}

		if not object
		{
			object = moveto
			if object.before
				return true
		}
!		else:  object = moveto
		moveto = location.(moveto.dir_to)

		if &moveto.door_to or moveto.type = door
		{
			moveto = moveto.door_to
			exit_type = door
		}
		elseif moveto.type ~= room and moveto.door_to
		{
			moveto = moveto.door_to
			exit_type = non_door_portal
		}
		elseif moveto is enterable and moveto.type ~= room
			return Perform(&DoEnter, moveto)
	}
#else   ! ifset NO_OBJLIB
	if m
	{
		if player not in location and m = out_to
		{
			if parent(player) is platform
			{
				RlibMessage(&DoGo, 1) ! "You'll have to get up."
				return false
			}
			else
				return Perform(&DoExit)
		}
		elseif player not in location and m = u_to
		{
			if parent(player) is container
			{
				RlibMessage(&DoGo, 1) ! "You'll have to get out."
				return false
			}
			else
				return Perform(&DoExit, parent(player))
		}

		moveto = location.m
		if moveto.door_to
			moveto = moveto.door_to
		exit_type = non_door_portal
	}
#endif  !  end of ifset NO_OBJLIB

	if moveto = false
	{
		exit_type = 0
		if not location.cant_go
			RlibMessage(&DoGo, 3)      ! "You can't go that way."
		return false
	}
	elseif moveto = true                    ! already printed message
	{
		exit_type = 0
		return true                     ! (moveto is never 1)
	}
	elseif player not in location ! and           ! sitting on or in an obj.
!		not vehicle_check  ! make sure it's not a vehicle that can go through
		                   ! this door
	{
		exit_type = 0
		if parent(player) = moveto  ! does the direction lead to parent(player)?
			VMessage(&DoEnter, 3)    ! already in it message
		else
			RlibMessage(&DoGo, 1)              ! "You'll have to get up..."
	}
	else
	{
!\ Recursive call issues
		m = verbroutine                 ! check room exiting
		verbroutine = &DoExit
		if location.before
			return true
		verbroutine = m
\!
#ifclear NO_OBJLIB
		if not exit_type
			exit_type = direction
#endif
		MovePlayer(moveto)

		return true
	}
}

! Roody's note: added a special message call for the player object
replace DoHit
{
	if not CheckReach(object):  return false

	if not object.after
	{
		if object = player
			RlibMessage(&DoHit)   ! "Venting your frustration on yourself..."
		else
			VMessage(&DoHit)         ! "Doesn't accomplish much..."
	}
	else
		return true
}

! Roody's note: Made it so the verboutine is always DoInventory so this routine
!  can be called for inventory windows in graphical games.
replace DoInventory
{
	local tempformat,v

	if word[2] = "tall"
		INVENTORY_MASK = INVENTORY_MASK | LIST_F
	elseif word[2] = "wide"
		INVENTORY_MASK = INVENTORY_MASK & ~LIST_F
	v = verbroutine
	verbroutine = &DoInventory
	tempformat = FORMAT
	FORMAT = FORMAT | INVENTORY_F | GROUPPLURALS_F | NOINDENT_F
	FORMAT = FORMAT | INVENTORY_MASK
	list_nest = 0
	if not WhatsIn(player)
		VMessage(&DoInventory)   ! "You aren't carrying anything."
	FORMAT = tempformat
	verbroutine = v

	return true
}

! Roody's note: This version of DoListen gives more default responses.
replace DoListen
{
	if object
	{
		if not object.after
		{
			if object is living and object ~= player
				RLibMessage(&DoListen, 1)  ! "So-and-so is quiet."
			else
				RLibMessage(&DoListen, 2)  ! "That would serve no purpose."
		}
	}
	else
		RLibMessage(&DoListen, 3)  ! "You hear nothing unexpected."
	return true
}

!\ Roody's note:  RoomSounds exists just for a quick way to do location
sounds (responses for >LISTEN without specific objects).  Add it to a room's
before property like so:

	before
	{
		RoomSounds(location)
		{
			"No sounds but the wind."
		}
	}
\!

routine RoomSounds(obj)
{
	if verbroutine = &DoListen and not object
		return location
	else
		return false
}

!\ Roody's note: The following DoUnlock and DoLock routines allow for multiple
key objects per locked item. A more "feature-full" version of this kind of
thing is Jason McWright's multikey.h extension.
\!
replace DoUnlock
{
	local a, list_key, cant_reach
	if not CheckReach(object):  return false

	!\ because setupdirectionobjects can sometimes dictate an xobject where we
	don't want it, we'll get rid of directional xobjects here \!
#ifclear NO_OBJLIB
	if xobject and xobject.type = direction
		xobject = 0
#endif

	if xobject ~= 0
	{
		a = InList(object,key_object,xobject)
		if not a
		{
			VMessage(&DoUnlock, 1)  ! "Doesn't seem to work..."
			return true
		}
	}
	if object is not locked
	{
		VMessage(&DoUnlock, 3)           ! already unlocked
		return true
	}
	if object.key_object and not xobject
	{
		for (a=1; a <= object.#key_object ; a++)
		{
			if Contains(player, object.key_object #a)
			{
				if FindObject(object.key_object #a,location) = 1
				{
					xobject = object.key_object #a
					list_key = true
					break
				}
				else
					cant_reach = object.key_object #a
			}
		}
		if not xobject or xobject is quiet
		{
			if xobject is quiet
				RLibMessage(&DoUnlock, 1) ! "It is unclear what you'd like to
				                          !  unlock the <blank> with."
			elseif cant_reach
				RLibMessage(&DoUnLock, 2, cant_reach)     ! "You can't reach the..."
			else
				VMessage(&DoUnlock, 2)           ! no key that fits
			return true
		}
	}

	if object is not locked
		VMessage(&DoUnlock, 3)           ! already unlocked
	else
	{
		object is not locked
		if xobject
			xobject is not quiet
		if not object.after
		{
			if not xobject.after
				RLibMessage(&DoUnlock, 3, list_key)   ! "Unlocked."
		}
	}
	return true
}

replace DoLock
{
	local a,cant_reach,list_key
	if not CheckReach(object):  return false

	if xobject ~= 0
	{
		a = InList(object,key_object,xobject)
		if not a
		{
			VMessage(&DoUnlock, 1)  ! "Doesn't seem to work..."
			return true
		}
	}
	if object is locked
	{
		VMessage(&DoLock, 1)             ! already locked
		return true
	}
	if object.key_object and not xobject
	{
		for (a=1; a <= object.#key_object ; a++)
		{
			if Contains(player, object.key_object #a)
			{
				if FindObject(object.key_object #a,location) = 1
				{
					xobject = object.key_object #a
					list_key = true
					break
				}
				else
					cant_reach = object.key_object #a
			}
		}
		if not xobject or xobject is quiet
		{
			if xobject is quiet
			{
				RLibMessage(&DoLock, 1) ! "It is unclear what you'd like to
								  !  lock the <blank> with."
			}
			elseif cant_reach
				RLibMessage(&DoUnLock, 2, cant_reach)     ! "You can't reach the..."
			else
				VMessage(&DoUnlock, 2)           ! no key that fits
			return true
		}
	}

	if object is locked
		VMessage(&DoLock, 1)             ! already locked
	elseif object is open
		VMessage(&DoLock, 2)             ! "Have to close it first..."
	else
	{
		if xobject
			xobject is not quiet
		object is locked
		if not object.after
		{
			if not xobject.after
				RLibMessage(&DoLock, 2, list_key)     ! "Locked."
		}
	}
	return true
}

! Replaced LOOK routines\

!\ Roody's note: I added a flag called NO_LOOK_TURNS. #set it if you want your
game to not count LOOK moves (LOOK, EXAMINE <object>, LOOK THROUGH <object)
as turns.

Added children-listing to all transparent objects.

Added code for smarter children listing. Added Zork-style default
responses for containers with no long_desc. Added "object = -1" so
pronouns are set.\!

replace DoLook
{
	local i,skip_ahead, no_fullstop, has_children, count

	if not light_source
		VMessage(&DoLook, 1)     ! "It's too dark to see anything."
	else
	{
		if ( object is transparent or !(object is living, transparent) or
			object is platform or (object is container and
			(object is open or object is not openable))) and
			object is not quiet ! and object is not already_listed
		{
			for i in object
			{
				i is not already_listed
				if i is not hidden
				{
					has_children = true
					count++
				}
			}
		}

		if not object.long_desc
		{
#ifclear FORCE_DEFAULT_MESSAGE
			if object is container and
			object is not quiet and object is not living
			{
				if (object is openable,open)
					print "It's open.";
				Perform(&DoLookIn,object) ! so we get "it is closed." if closed
				skip_ahead = true
			}
			elseif has_children
				no_fullstop = true
			else
#endif
				! "Looks just like you'd expect..."
				VMessage(&DoLook, 2)
		}

		if (object is transparent or !(object is living, transparent) or
			object is platform or (object is container and
			(object is open or object is not openable))) and
			object is not quiet ! and object is not already_listed
		{
			has_children = false
			for i in object
			{
				if i is not hidden and i is not already_listed
				{
					has_children = true
					break
				}
			}
		}

		if i and object ~= player and not skip_ahead
		{
			if count = 1
				parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
			local tempformat
			tempformat = FORMAT
			FORMAT = FORMAT | NOINDENT_F
			list_nest = 0
			if not no_fullstop
				print ""
			WhatsIn(object,has_children)
			FORMAT = tempformat
			parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
		}

		run object.after
#ifset AUTOMATIC_EXAMINE   ! objects are examined automatically when picked up
		if object is not examined
			object is examined
#endif
#ifclear NO_LOOK_TURNS
		return true
#endif
	}
}

replace DoLookAround
{
	DescribePlace(location, true)
#ifclear NO_LOOK_TURNS
	return true
#endif
}

!\ Roody's note: I was unsure whether to have LOOK IN work like other LOOK
verbs when the NO_LOOK_TURNS flag is set, as it could be argued that LOOK IN
is more of an action-oriented verb, like SEARCH. For instance, I have LOOK UNDER
still take a turn (so if you DON'T want that, you'll have to replace it).

Just the same, I decided that with NO_LOOK_TURNS, LOOK IN defaults to
not-taking-a-turn, so if you want it to take a turn, you'll have to replace it.

Also, non-container, transparent items are now allowed.

The original Hugo routine only allowed "look in" to work on objects within
reach.  I've added code so ALL transparent objects can be looked in even if
the player is in an enterable object (like a chair).

Of course, there might be some openable objects that you'd want the player
to be able to look in even if they're in a parent object.  My best advice is
to give the object the transparent attribute when it is opened and remove
the attribute when it's closed.

Conversely, if there's a transparent object you DON'T want the player to be
able to look inside without being near (like a pool of water several yards
away), you might want to remove the transparent attribute when the player
enters a parent object (like a lifeguard chair) and give it back when the
player exits it.\!

replace DoLookIn
{
	local tempformat

	if object is not transparent
	{
		if not CheckReach(object):  return false
	}


	if not light_source
		VMessage(&DoLook, 1)     ! "It's too dark to see anything."
	elseif object is not container and object is not transparent and
		object is not platform
		ParseError(12,object) ! "You can't do that with the thing."
	elseif object is living
	{
	!\ I don't know how useful this will be, but I put in an after
	check for characters so you can have character-specific responses to
	LOOK IN <PERSON>. \!
		if object.after
			return true
		else
			ParseError(12,object) ! "You can't do that with so-and-so."
	}
	else
	{
		if object is container and ((object is openable and
			object is not open) and object is not transparent):
			VMessage(&DoLookIn, 1)           ! "It's closed."
		else
		{
			if not object.after
			{
				object is not quiet

				tempformat = FORMAT
				FORMAT = FORMAT | NOINDENT_F
				list_nest = 0

				parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
				local i
				i = WhatsIn(object)
				if not i
					VMessage(&DoLookIn, 2)   ! "It's empty."
				elseif i = 1
					parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET

				FORMAT = tempformat
			}
		}
#ifclear NO_LOOK_TURNS
		return true
#endif
	}
}

! Roody's note- Replaced to disallow looking through characters
replace DoLookThrough
{
	if object is not static
	{
		if not CheckReach(object)
			return false
	}

	if object is transparent and object is not living
	{
		if object is container or children(object)
			return Perform(&DoLookIn, object)
		else
			return Perform(&DoLook, object)
	}

#ifclear NO_OBJLIB
	elseif object.type = door
	{
		if object is not open
			VMessage(&DoLookIn, 1)   ! "It's closed."
		else
		{
			! "Through it you can see..."
			VMessage(&DoLookThrough, 1)

			if location = object.found_in #1
				print The(object.found_in #2); "."
			else
				print The(object.found_in #1); "."
		}
#ifclear NO_LOOK_TURNS
		return true
#endif
	}
#endif  ! NO_OBJLIB
	else
		VMessage(&DoLookThrough, 2)  ! "You can't see through that."
}

! Roody's note: Added a special message call for the player object
replace DoLookUnder
{
	if not CheckReach(object):  return false

	if object = player
		RlibMessage(&DoLookUnder)
	else
		VMessage(&DoLookUnder, 1)        ! "You don't find anything."
	return true
}

!\ Roody's note: roodylib, by default, supports PUSH and PULL. It redirects them
to DoMove if verbstub hasn't been included.  \!

replace DoMove
{
	if not CheckReach(object):  return false

	if object = player
	{
		RlibMessage(&DoMove, 1) ! "You can't move yourself."
		return
	}
#if defined DoPush
	if word[1] = "push", "shove", "press"
		return Perform(&DoPush, object)
#endif

#if defined DoPull
	if word[1] = "pull", "yank", "tug"
		return Perform(&DoPull, object)
#endif

	VMessage(&DoMove)                ! "You can't move that."
	return true
}

!\ Roody's note: Fixes a bug found by Juhana Leinonen where, if the player is
the only thing in the object being opened, an extra line is printed. This has a
fix for it although I haven't decided if it's an optimal solution.
\!
! Added Zork-style content listing to objects without object.after's
! and added "object = -1" so new pronoun settings stick
! and smarter content-listing

replace DoOpen
{
	local tempformat, light_check, skip_ahead, force_def

	tempformat = FORMAT
#ifset FORCE_DEFAULT_MESSAGES
	force_def = true
#endif
	if not CheckReach(object):  return false

	if object is not openable
	{
		VMessage(&DoOpen, 1)             ! "You can't open that."
		return
	}
	elseif object is open
		VMessage(&DoOpen, 2)             ! "It's already open."
	elseif object is locked
		VMessage(&DoOpen, 3)             ! "It's locked."
	else
	{
		object is open
		object is moved
		local x, i
		if not Contains(object,player) and object is not transparent
		{
			for x in object
			{
				if x is not hidden
				{
					i = true
					x is not already_listed
				}
			}
		}
		if i
		{
			if not light_source
				light_check = FindLight(location)  ! in case the light source
                                               ! has been revealed
		}
		if not object.after
		{
			if not i or object is quiet or force_def
				VMessage(&DoOpen, 4)     ! "Opened."
			else
			{
				list_count = 0
				for x in object
				{
					if x is not hidden
						list_count++
				}
				if list_count = 1
					parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
				RLibMessage(&DoOpen,1) ! "opening the <blank> reveals"
				ListObjects(object)
				parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
				skip_ahead = true
			}
		}
		else
			skip_ahead = true ! object.after has to list the contents
			                  ! if it exists

		if i and object is not quiet and
			not skip_ahead
		{
			print ""
			if children(object) = 1
				parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
			tempformat = FORMAT
			FORMAT = FORMAT | NOINDENT_F
			list_nest = 0
			WhatsIn(object)
			parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
		}
	}
	FORMAT = tempformat
	if light_check
		Perform(&DoLookAround)
	return true
}

! Roody's note: First off, I changed the grammar for DoPutIn so &DoPutIn
! message one gets called at all, but I'm also replacing that message here
! since I didn't especially like the original wording.

replace DoPutIn
{
	if not xobject
		RlibMessage(&DoPutIn, 1)    ! "Put it in what?"
	elseif object is clothing and object is worn
		VMessage(&DoDrop, 1)     ! "Have to take it off first..."
	elseif xobject = location
		return Perform(&DoDrop,object)
	elseif xobject is container, openable, not open
		VMessage(&DoPutIn, 2)    ! "It's closed."
	elseif object = xobject
		VMessage(&DoPutIn, 3)    ! putting something in itself
	elseif xobject is not container and xobject is not platform
		VMessage(&DoPutIn, 7)    ! "Can't do that with..."
	elseif Contains(object, xobject)
		VMessage(&DoPutIn, 4)    ! putting a parent in its child
	elseif object in xobject
		VMessage(&DoPutIn, 8)	! "It's already in..."
	elseif CheckReach(object)
	{
		if CheckReach(xobject)
		{
			if Acquire(xobject,object)
			{
				if not object.after
				{
					if not xobject.after
						! "You put X in/on Y."
						VMessage(&DoPutIn, 5)
				}
			}
			else
				! "There's no room..."
				VMessage(&DoPutIn, 6)
		}
	}
	return true
}

!----------------------------------------------------------------------------
!\ Note about xverbs- I cleaned up the code to a bunch of these and tweaked
them so they can be called by EndGame/ProcessKey for more consistent
behavior. \!

! Roody's note: Changed DoQuit to give one final ending message

#if undefined PRESS_ANY_KEY
constant PRESS_ANY_KEY "[ press any key to exit ]"
#endif

#ifclear NO_XVERBS
replace DoQuit
{
	PrintScore
	VMessage(&DoQuit)                        ! "Are you sure?"
	GetInput
	if YesorNo = true
	{
		QuitGameText
		quit
	}
	else
		RLibMessage(&DoQuit,1) ! "Continuing on."
}

replace DoRestart
{
	if word[1] ~= "yes"
	{
		if MAX_SCORE
			PrintScore
		VMessage(&DoRestart, 1)                  ! "Are you sure?"
		GetInput
	}
	if YesorNo = true
	{
		SaveSettings("restart")
		if not restart
			VMessage(&DoRestart, 2)  ! "Unable to restart."
		else
			return true
	}
	else
	{
		RlibMessage(&DoRestart, 1) ! "\nContinuing on."
		if (FORMAT & DESCFORM_I)
			""
#ifclear USE_DARK_ROOM
		elseif not FindLight(location)
			""
#endif
		DescribePlace(location, true)
	}
}

!----------------------------------------------------------------------------
replace DoRestore
{
	SaveSettings("restore")
	if verbroutine = &EndGame
		""
	if restore
	{
		LoadSettings
		RestoreResponse
		return true
	}
	else:  VMessage(&DoRestore, 2)           ! "Unable to restore."
}

!\ Roody's note: Mainly to match the new DoUndo behavior, I added a similar
 response routine for restore. I also added a screen clear since I figure
 it's sort of like jumping to another part of the book. \!

routine RestoreResponse
{
#ifset NO_ACCESSIBILITY
	local cheap
#endif
	if not (system(61) or cheap = 1)
		InitScreen
	PrintStatusLine
	VMessage(&DoRestore, 1)         ! "Restored."
	if (FORMAT & DESCFORM_I)
		""
#ifclear USE_DARK_ROOM
	elseif not FindLight(location)
		""
#endif
	DescribePlace(location, true)
}

!----------------------------------------------------------------------------
! Roody's note: Modified to work with any statustype with a MAX_SCORE
replace DoScore
{
	if not MAX_SCORE
		VMessage(&DoScore)               ! no scorekeeping
	else
		PrintScore
}

replace DoScriptOnOff
{
	if word[2] = "on" or words = 1
	{
		if betalib is special
			RLibMessage(&DoScriptOnOff, 1) ! "Transcription is already on."
		elseif (not scripton)
			VMessage(&DoScriptOnOff, 1) ! "Unable to begin transcription."
		else
		{
			betalib is special
#ifset BETA
			RLibMessage(&DoScriptOnOff, 2) ! "Transcription on. Type * ..."
#else
			VMessage(&DoScriptOnOff, 2) ! "Transcription on."
#endif
		}
	}
	elseif word[2] = "off"
	{
		if betalib is not special
			RLibMessage(&DoScriptOnOff, 3) ! "Transcription is not currently on."
		elseif (not scriptoff)
			VMessage(&DoScriptOnOff, 3) ! Unable to end transcription."
		else
		{
			betalib is not special
			VMessage(&DoScriptOnOff, 4) ! "Transcription off."
		}
	}
}

#ifclear NO_RECORDING
replace DoRecordOnOff
{
	if VerbWord = "playback"
	{
		if word[2] = "off"
		{
			if betalib is not playback_on
				RLibMessage(&DoRecordOnOff,1) ! "No playback in progress."
			else
			{
!				betalib is not skip_pauses
				betalib is not playback_on
				RLibMessage(&DoRecordOnOff,2) ! "Playback completed."
			}
		}
		elseif betalib is record_on
			RLibMessage(&DoRecordOnOff,3) ! "No playback while recording."
		elseif betalib is playback_on
			RLibMessage(&DoRecordOnOff,4) ! "Playback already in progress."
		else
		{
			if not system(61) ! not a minimal port
			{
				RLibMessage(&DoRecordOnOff,5) ! "MORE skipping spiel"
				TopPause
				PrintStatusLine
				""
			}
			if not playback
				VMessage(&DoRecordOnOff, 1)  ! "Unable to begin..."
			else
			{
				VMessage(&DoRecordOnOff, 2)  ! "Playback beginning..."
				betalib is switchedon
			}
		}
	}
	elseif word[2] = "on" or words = 1
	{
		if betalib is playback_on
			RLibMessage(&DoRecordOnOff,7) ! "No recording during playback."
		elseif betalib is record_on
			RLibMessage(&DoRecordOnOff,8) ! "Recording already in progress."
		elseif not recordon
			VMessage(&DoRecordOnOff, 3)  ! "Unable to begin..."
		else
		{
			betalib is record_on
			VMessage(&DoRecordOnOff, 4)  ! "Recording on."
		}
	}
	elseif word[2] = "off"
	{
		if betalib is not record_on
			RLibMessage(&DoRecordOnOff,9) ! "No recording in progress."
		elseif not recordoff
			VMessage(&DoRecordOnOff, 5)  ! "Unable to end..."
		else
		{
			betalib is not record_on
			VMessage(&DoRecordOnOff, 6)  ! "Recording off."
		}
	}
}
#endif

replace DoUndo
{
	if verbroutine = &EndGame
		""
	if not UNDO_OFF
	{
		SaveSettings("undo")
		parser_data[LAST_PARSER_STATUS] &= ~PARSE$_F
		if undo
		{
			LoadSettings
			UndoResponse
			parser_data[LAST_SINGLE_OBJECT] = 0
			return true
		}
		else
			RLibMessage(&UndoResponse,2)  ! "Unable to UNDO."
		return
	}
	else
		RLibMessage(&UndoResponse,2) ! "UNDO is not currently allowed."
}

#ifset SHOW_COMMANDS

routine PrintCommand(arr,n)
{
	while array arr[n] ~= "" and n < array arr[] and
		array arr[n] ~= "then"
	{
		select array arr[n]
			case "~and"
			{
!				if array arr[(n-1)] = "~and"
				if not VerbCheck(array arr[n+1])
					print " and";
				else
					print ",";
			}
			case "~all"
				print "all";
			case "~except" : print "except";
			case "~oops" : print "oops";  ! hopefully never called
			case "~any" : print "any";
						case "l"
			{
				if n = 1 or word[n-1] = "~and"
					print "look";
			}
			case "x"
			{
				if n = 1 or word[n-1] = "~and"
					print "examine";
			}
			case "g" : print "again";
			case else
				print array arr[n];
		if array arr[n+1] ~= "~and", ""
			print " ";
		n++
	}
}

#endif ! SHOW_COMMANDS
#endif ! ifclear NO_XVERBS

object undolib "undolib"
{
	in init_instructions
	save_info
	{
		if word[LAST_TURN] ~= "undo"
			return false
		if parser_data[LAST_PARSER_STATUS] & PARSE$_F
			return false
#ifset SHOW_COMMANDS
		if oldword[1] or (last_actor and last_actor ~= player)
		{
			Font(ITALIC_ON)
			print "[ undoing ";
			Font(BOLD_ON|ITALIC_OFF)
			print ">";
			if last_actor and last_actor ~= player
			{
				print last_actor.name;
				if oldword[1] ~= ""
					print ", ";
			}
			PrintCommand(oldword,1)
			print " ";
			Font(BOLD_OFF|ITALIC_ON)
			print "]"
			Font(DEFAULT_FONT|ITALIC_OFF)
			return true
		}
#endif
			return false
	}
#ifset SHOW_COMMANDS
	execute
	{
		SaveWordSetting("command")  ! SHOW_COMMANDS printed something
	}
#endif
}

!\ Roody's note: I thought it'd be better if there were a separate routine
  for what gets printed after a successful routine, so authors can easily
  configure the response without replacing the whole routine. Since this
  new replaceable routine should cover the same uses as the USE_AFTER_UNDO
  routine address array system, I have gotten rid of that. \!

routine UndoResponse
{
	local v
	PrintStatusLine
#ifset SHOW_COMMANDS
		local a
		a = CheckWordSetting("command") ! check if a command was printed
		if a
			word[a] = ""
		else
#endif
			RlibMessage(&UndoResponse,1) ! "Undone."

	if (FORMAT & DESCFORM_I)
		""
#ifclear USE_DARK_ROOM
	elseif not FindLight(location)
		""
#endif
	parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
	v = verbroutine
	verbroutine = &DoLookAround
	DescribePlace(location, true)
	verbroutine = v
}

! Roody's note: Fixes a typo from verblib.h version
replace DoStand
{
	if parent(parent(player)) = nothing     ! i.e., a room
		VMessage(&DoEnter, 3)           ! "A bit redundant..."
	else
		return Perform(&DoExit , (parent(player)))
}

! Roody's note: Added some other situations where >GET could be called.
replace DoTakeOff
{
	if parent(object) ~= player and
		not (object is worn and object is clothing)
	{
		! So >REMOVE LOCK will >TAKE LOCK, etc.
		return Perform(&DoGet, object)
	}
	if object is not clothing
	{
		VMessage(&DoTakeOff, 1)         ! "Can't do that..."
		return false
	}

	if object is not worn
		VMessage(&DoTakeOff, 2)          ! "You're not wearing that."
	else
	{
		if parent(object) ~= player
		{
			xobject = parent(object)
			if not CheckReach(xobject)
				return false
		}
		if xobject and not xobject.after
		{
			RLibMessage(&DoTakeOff, 1) ! "So-and-so doesn't let you."
		}
		else
		{
			object is not worn
			if not object.after
				VMessage(&DoTakeOff, 3)  ! "You take it off."
		}
	}
	return true
}

!\ Roody's note: Replaced so "keep waiting?" prompts can be ignored during
recording playback. \!
replace DoWait(count)                   ! count argument is from DoWaitUntil
{
	if object = 0
		count = WAIT_TURN_COUNT
	elseif count = 0
		count = object

	VMessage(&DoWait)                ! "Time passes..."
	event_flag = 0
	while --count
	{
		Main
		system(32) !(PAUSE_100TH_SECOND)  ! potential updating
		local a
#ifset DEBUG
		a = (debug_flags & D_PLAYBACK_HELPER)
#endif
		if event_flag and not a
			if not KeepWaiting
				return
	}
	event_flag = 0
	return true
}

! Roody's note: I noticed the original routine doesn't technically stop
! waiting when the character gets there
replace DoWaitforChar
{
	local count

	if object is not living
	{
		ParseError(6)
		return false
	}

	if object in location
	{
		VMessage(&DoWaitforChar, 1)      ! "They're right here..."
		return true
	}

	VMessage(&DoWait, 1)                     ! "Time passes..."
	event_flag = 0
	do
	{
		Main
		if object in location
		{
			! character has arrived
			VMessage(&DoWaitforChar, 2)
			event_flag = 2
!			return
		}
		if event_flag
		{
			if not KeepWaiting
				return
		}
		count++
	}
	while (STATUSTYPE=2 and count<60) or (STATUSTYPE~=2 and count<20)

	VMessage(&DoWaitforChar, 3)     ! char. hasn't arrived yet

	event_flag = 0
}

!----------------------------------------------------------------------------
!* REPLACED VERBSTUB.H ROUTINES
!----------------------------------------------------------------------------
#ifclear _VERBSTUB_H
! Roody's note: SEARCH is a pretty common verb. Roodylib adds it regardless.
routine DoSearch
{}
#endif
! Roody's note: Fixes some pronoun stuff. Suggested by Mike Snyder.
replace DoSearch
{
	if object = player
	{
		"Search ";
		The(player,true)
		" indeed."
	}
	elseif object is container and child(object)
		Perform(&DoLookIn, object)
	elseif object is living
	{
		print CThe(object); MatchPlural(object, "doesn't", "don't");
		" let ";
		The(player,true)
		if object.pronouns
		{
			" search ";
			if object.pronouns #2
				print object.pronouns #2;
			else
				print object.pronoun;
		}
		print "."
	}
	else
	{
		CThe(player)
		MatchPlural(player, "doesn't", "don't")
		" find anything new."
	}
	return true
}

!\ Roody's note - Roodylib replaces DescribePlace to fix a couple
things and add a little more functionality. Initial descriptions are still
run in SUPERBRIEF mode, just in case important code is executed there.
The DESCFORM_I FORMAT mask, if set, prevents a blank line from being printed
before the room title. Fixed the verbroutine check so that DescribePlace
can be used for rooms that the player is not in (for faking player movement).

Also, I sent the room name printing to RLibMessages so it is easier to
configure them.
\!

! Roody's note: Here is the routine to replace if you'd like there to
! be some instances where pronouns are not set to room objects. Have
! it return false for pronouns not to be set.

routine AssignPronounsToRoom
{
!	An example of a check. In this case, pronouns would not be set
!   when the player enters doors or "non_door_portals"
!	if (word[1] = "examine","x","l","look","watch") or
!	exit_type = direction
!	{
!		return true
!	}
	if (verbroutine = &MovePlayer, &DoLookAround) or ! not word[1] or
		XverbCheck(word[1]) !(word[1] = "restart","restore")
		return true
	else
		return false
}

#ifset USE_RELATIVE_DESCRIPTIONS
! Roody's note:  This routine establishes the "rules" for when DescribePlace
! should alter text based on the player being in a container or platform
routine RelativeParent(obj)
{
	if player not in location and parent(player) is container
		return true
	else
		return false
}

! Roody's note:  This routine allows changing the word used for where an object
! is relative to whatever the player is in.
routine RelativeText(obj)
{
	if obj = location and player not in location and
		parent(player) is container
	{
		print "outside ";
	}
	elseif obj is container
		print "inside ";
	else
		print "on ";
	The(parent(player))
}
#endif

replace Describeplace(place, long)
{
	local obj, count, notlisted, tempformat, charcount

	if not place
		place = location

	if AssignPronounsToRoom
		parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET

   ! Since, for example, a room description following entering via
   ! DoGo does not trigger before/after properties tied to looking
   ! around:
   !
#ifclear NO_VERBS
	if verbroutine = &MovePlayer
	{
		if place is not visited and verbosity ~= 1
			return Perform(&DoLookAround)
		elseif long = true or verbosity = 2
			return Perform(&DoLookAround)
	}
#endif

	exit_type = 0  ! clear the exit_type global

	if not light_source
	{
		Message(&DescribePlace, 1)     ! "It's too dark to see..."
		return
	}

	place is known

   ! Print the name of the location as a heading
!	RLibMessage(&DescribePlace,1,place)
	PrintRoomName(place)

	override_indent = false

	if place is not visited and verbosity ~= 1
	{
		if &place.initial_desc or &place.long_desc
			Indent
		if not place.initial_desc
			run place.long_desc
	}
	elseif long = true or verbosity = 2
	{
		if &place.long_desc or &place.initial_desc
			Indent
#ifset NEW_ROOMS
		if &place.initial_desc and place.first_visit = counter and
			(parser_data[VERB_IS_XVERB] or word[1] = "undo")
			run place.initial_desc
		else
#endif
			run place.long_desc
	}
	elseif place is not visited and verbosity = 1
	{
		if &place.initial_desc
			Indent
		run place.initial_desc
	}

	if &place.list_contents and (FORMAT & DESCFORM_F)
		print ""        ! for double-space-after-heading formatting

   ! A location may contain an overriding listing routine, as may any
   ! parent, in the list_contents property
   !
	if not place.list_contents
	{
		list_nest = 0

      ! For double-space-after-heading formatting:
		if (FORMAT & DESCFORM_F)
		{
			for obj in place
			{
				if obj is not hidden and
					(player not in obj or children(obj) > 1)
				{
					print ""
					break
				}
			}
		}

      ! List contents of chair, vehicle, etc. player is in
		if player not in location
		{
#ifset USE_RELATIVE_DESCRIPTIONS
			if RelativeParent(parent(player)) and children(parent(player)) > 1 and
				player not in place and place = location and
				not (FORMAT & LIST_F)
			{
				tempformat = FORMAT
				FORMAT = FORMAT | FIRSTCAPITAL_F | ISORAREHERE_F
				DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT | OVERRIDEHERE_F
				if (FORMAT & LIST_F)
				{
					FORMAT = FORMAT & ~LIST_F       ! clear it
					FORMAT = FORMAT | TEMPLIST_F
				}
				list_nest = 0
				! called WhatsIn and not ListObjects so the parent's listcontents
				! property will be checked
				WhatsIn(parent(player))
				FORMAT = tempformat
			}
			else
			{
#endif
				list_nest = 1
				WhatsIn(Parent(player))
#ifset USE_RELATIVE_DESCRIPTIONS
			}
#endif
		}

      ! List all characters, if any
		count = 0
		for obj in place
		{
			if obj is hidden or obj is not living or
				player in obj
				obj is already_listed
			else
				obj is not already_listed
		}
		for obj in place
		{
			if obj is not already_listed
			{
				print newline
				ShortDescribe(obj)
				if obj is not already_listed
					count++
			}
		}

		list_count = count
		count = 0

		if list_count           ! if characters are to be listed
		{
			charcount++
			tempformat = FORMAT
			FORMAT = FORMAT | FIRSTCAPITAL_F | ISORAREHERE_F
#ifset USE_RELATIVE_DESCRIPTIONS
			if RelativeParent
				DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT | OVERRIDEHERE_F
#endif
			if list_count > 1
				FORMAT = FORMAT | USECHARNAMES_F
			Indent
			if (FORMAT & LIST_F)
			{
				FORMAT = FORMAT & ~LIST_F       ! clear it
				FORMAT = FORMAT | TEMPLIST_F
			}
			list_nest = 0
			ListObjects(place)
			FORMAT = tempformat
		}

		for obj in place
		{
#ifset USE_ATTACHABLES
         ! Exclude all attachables for now (and characters)

			if obj is living or obj.type = attachable or
				player in obj
#else
			if obj is living or player in obj
#endif
				obj is already_listed
			else
				obj is not already_listed
		}

		for obj in place
		{
#ifset USE_PLURAL_OBJECTS
         ! ...And don't list identical objects yet, either

			if (obj.identical_to).type = identical_class
			{
				if obj is not hidden
				count++
			}
			elseif player not in obj
#else
			if player not in obj
#endif
			{
				if obj is not already_listed and
					obj is not hidden
				{
					ShortDescribe(obj)
					if obj is not already_listed
						notlisted++
				}
			}
		}

		if notlisted or count
		{
			list_count = notlisted + count
			tempformat = FORMAT
			FORMAT = FORMAT | FIRSTCAPITAL_F | ISORAREHERE_F
#ifset USE_RELATIVE_DESCRIPTIONS
			if RelativeParent
			{
				DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT | OVERRIDEHERE_F
				if charcount
					DESCRIBEPLACEFORMAT = DESCRIBEPLACEFORMAT | ALSO_F
			}
#endif
			Indent
			if (FORMAT & LIST_F)
			{
				FORMAT = FORMAT & ~LIST_F       ! clear it
				FORMAT = FORMAT | TEMPLIST_F
			}
			list_nest = 0
			ListObjects(place)
			FORMAT = tempformat
		}

#ifclear NO_OBJLIB
#ifset USE_ATTACHABLES
		for obj in place
		{
         ! Print attachables last
			if obj.type = attachable and obj is not hidden
			{
				ShortDescribe(obj)
				if obj is not already_listed
					Message(&DescribePlace, 2, obj)
			}
		}
#endif

		print newline
		override_indent = false

      ! Finally, list contents of scenery objects (unless we've
      ! already done so as the parent of the player)
      !
		for obj in place
		{
			if obj.type = scenery
			{
				obj is known
				if player not in obj and
           !    (obj is open or obj is not openable)
			!	((obj is container and (obj is open or obj is transparent))  or
			!	obj is platform) and obj is not quiet
				(obj is open or obj is not openable or obj is platform or
				obj is transparent) and obj is not quiet
				{
					list_nest = 1
					WhatsIn(obj)
				}
			}

         ! For scenery-derived objects that may change the type
			elseif obj is static, hidden
				obj is known
		}
#ifset USE_ATTACHABLES
		print newline
		count = 0
		local no_good, good,a
		for obj in player
		{
			no_good = 0
			good = 0
			if obj.type = attachable
			{
				if Inlist(obj,attached_to,player)
					no_good = true
				if not no_good
				{
					for (a=1; a<=obj.#attached_to; a++)
					{
						if obj.attached_to #a
						{
							if not Contains(player,obj.attached_to #a)
							{
								if FindObject(obj.attached_to #a,place)
								{
									good = true
									break
								}
							}
						}
					}
					if good
					{
						if count++
							print AFTER_PERIOD;
						else
							Indent
						RLibMessage(&DescribePlace,1, obj) ! "The <blank> you are
						                                   !  holding is tied to the
                                                     !  <blank>."
					}
				}
			}
		}
#endif  ! ifset USE_ATTACHMENTS
#endif  ! ifclear NO_OBJLIB

		print newline
		need_newline = false
		parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
	}
}

routine PrintRoomName(a)
{
	if not (FORMAT & DESCFORM_I)
		print ""
	else
		print newline
	Font(BOLD_ON)
	print capital a.name;

	! Print ", in <something>" if necessary
	if location = a and player not in a
	{
		if parent(player).prep
			print ", "; parent(player).prep; " ";
		elseif parent(player) is platform
			print ", "; ON_WORD; " ";
		else
			print ", "; IN_WORD; " ";
		print Art(parent(player))
	}
	print newline
	Font(BOLD_OFF)
}

!----------------------------------------------------------------------------
!* ADDITIONAL VERB AND UTILITY ROUTINES
!----------------------------------------------------------------------------

!\ Roody's note - If you want your game to not allow verbosity changes, #set
NO_MODE_CHANGE before Roodylib grammar is included. Then the grammar will call
the following routine which fakes a "I don't understand that" message.

You can use DoFakeRefuse for any other standard verb you want to fake a refusal,
too.
\!

routine DoFakeRefuse
{
	print CThe(player); \
				! " can't use the word \""; \
		MatchPlural(player, "doesn't", "don't"); \
		" need to use the word \""; \
		word[1] ;
#ifset AMERICAN_ENGLISH
		".\""
#else
		"\"."
#endif
}

! Roody's note - Mike Snyder pointed out that it's not optimal that "kick", by
! default, points to DoHit.  Sending the command to its own verb routine makes
! it easier to replace or catch with before routines.
routine DoKick
{
	return Perform(&DoHit, object)
}

!\ Roody's note - DoPushDir figures out what direction the player is trying to
push something, for accurate response messages (after finding the direction,
it calls DoPushDirTo.
\!
#ifclear NO_OBJLIB
routine DoPushDir
{
	SetupDirectionObjects
	return Perform(&DoPushDirTo, object, xobject)
}
#endif

!\ Roody's note- DoPushDirTo prints a "can't do that" message or
processes an object-pushing. \!

routine DoPushDirTo
{
	RLibMessage(&DoPushDirTo,1) ! "That would not help the present situation."
	return false ! let's not use up a turn
}

! Roody's note: Checkheld versions of DoWear and DoTakeOff
#ifset USE_CHECKHELD
routine DoTakeOff_CheckHeld		! See above re: ImplicitTakeForDrop
{
	if parent(object) ~= player and
	not (object is worn and object is clothing)
	{
		! So >REMOVE LOCK will >TAKE LOCK, etc.
		return Perform(&DoGet, object)
	}
	if (word[1] = "remove" and word[2] ~= "~all") or
	(word[1] = "take" and not (word[2] = "~all" or word[3] = "~all"))
	{
		return Perform(&DoTakeOff, object)
	}
	return CallVerbCheckHeld(&DoTakeOff, location)
}

routine DoWear_CheckHeld		! See above re: ImplicitTakeForDrop
{
	return CallVerbCheckHeld(&DoWear, location)
}
#endif

#ifclear NO_XYZZY
routine DoXYZZY
{
	! text suggested by Rob O'Hara. Approved by Ben Parrish.
	print CThe(player); " mumble"; MatchSubject(player);
	" an ancient reference to an archaic game. Nothing happens."
}
#endif

!----------------------------------------------------------------------------
!* OTHER ROUTINES
!----------------------------------------------------------------------------

!\ Roody's note: Future Boy!'s AddPropValue and RemovePropValue are like
the routines AddToScope and RemoveFromScope except they aren't limited to
one property (like in those cases, "scope").
As such, they are very useful routines. Written by Kent Tessman.
\!

! AddPropValue(obj, prop, value)
! Adds 'value' as a property value for obj, if possible
! if given a "no_repeats" argument, quits out if it finds an element with
! the desired value

! (AddPropValue is used by the "disambiguation helper" system)
routine AddPropValue(obj, prop, val,no_repeats)
{
	local i
	for (i=1; i<=obj.#prop; i++)
	{
		if obj.prop #i = val and no_repeats
			return i
		elseif obj.prop #i = 0
		{
			obj.prop #i = val
			return i
		}
	}
	return false
}


! RemovePropValue(obj, prop, value)
! Removes 'value' as a property value for obj, if applicable
! if given an "all_instances" argument, clears all elements with that value
routine RemovePropValue(obj, prop, val, all_instances)
{
	local i
	for (i=1; i<=obj.#prop; i++)
	{
		if obj.prop #i = val
		{
			obj.prop #i = 0
			if not all_instances
				return i
		}
	}
}

! Roody's note: Since the end of the game is called just by changing the
! value of the endflag global variable, that can be a little unintuitive to
! new authors. Here is a dummy routine for calling the end of the game.

routine CallFinish(end_type)
{
	endflag = end_type
}

! Roody's note: CanGoDir is an exit-checking routine so you can check to make
! sure the direction the player is trying to go in is valid before printing
! a relevant message. It hasn't had a lot of use and may not work in all
! instances.

! Used by the SMART_PARENT_DIRECTIONS system

#ifclear NO_OBJLIB
routine CanGoDir
{
	local m
	if object.type ~= direction and object is not enterable and
		object.type ~= door
		return false
	select true
		case (object in direction)
			m = location.(object.dir_to)
		case (object.type = door)
		{
			if location = object.between #1
				m = object.between #2
			else
				m = object.between #1
		}
		case else
		{
			if object.door_to
				m = object.door_to
			else
				return true
		}
	return m
}
#endif !  ifclear NO_OBJLIB

! Roody's note: This is used by Roodylib's word array state-saving system to
! make sure previous turns' info has been deleted (note: it deletes from the
! *end* of the word array)
routine ClearWordArray
{
	local n
	for (n=LAST_TURN;n> 0 ; n-- )
	{
		if word[n] = ""
			break
		else
			word[n] = ""
	}
}

!\ Roody's note: ClearPronoun is a routine for clearing pronouns (like if you
have a disappearing fake object). Feed it the object and it will clear every
pronoun global variable set to that value. \!

routine ClearPronoun(obj)
{
	select obj
		case it_obj: it_obj = 0
		case her_obj : her_obj = 0
		case him_obj : him_obj = 0
		case them_obj : them_obj = 0
}

! Roody's note: CoolPause is a routine I wrote for newmenu.h, but I find it
! so useful that I have decided to add it to Roodylib. Anyhow, it's a routine
! for doing "press a key" text. The page and no_newline arguments are mainly
! for the new menu system so that different pause text can be given to menu
! pages (and to avoid an extra line if called from EndGame)

!\ ALSO: This used to have code by default to change display.title_caption
 to some pause text. I decided to take that code out and put it into a wrapper
 routine in "extraroutines.hug" so go look there if you want it. \!

routine CoolPause(pausetext,page,no_newline)
{
!#ifset NO_ACCESSIBILITY
!	local cheap
!#endif

#ifclear NO_ACCESSIBILITY
	if (cheap & CHEAP_ON)
	{
!			if there is a pausetext value, prints pausetext.
!			 otherwise prints default.
		RlibMessage(&CoolPause,1,pausetext,page) ! "[PRESS A KEY TO CONTINUE]" !;
		pause
	}
	else
	{
#endif
!			if pausetext exists, prints pausetext. otherwise prints default.
!			 text is italicized
		RlibMessage(&CoolPause,2,pausetext,page) ! "press a key to continue"
		HiddenPause
#ifclear NO_ACCESSIBILITY
	}
#endif
!	print newline
	if not no_newline
		""
}

!\Roody's note: TopPause used to be a part of CoolPause (allowing the option
to put pause text in the status line) but I gave it its own routine just
for code readability.

(I find it nice sometimes to put the text in the status bar because then the
pause text does not interrupt game transcripts) \!
routine TopPause(pausetext)
{
	local force_bottom
#ifclear NO_ACCESSIBILITY
	if (cheap & CHEAP_ON)
		force_bottom = true
#endif
#ifset NO_STRING_ARRAYS
	force_bottom = true
#endif
	if force_bottom
		return CoolPause(pausetext)

	Font(BOLD_OFF | ITALIC_OFF | UNDERLINE_OFF | PROP_OFF)
	if not system(61)
	{
		window display.statusline_height
		{
			cls
		}
	}
	window  1 ! display.statusline_height
	{
		local y
		y = display.linelength
		color SL_TEXTCOLOR, SL_BGCOLOR
		cls			! make sure we've drawn the entire status bar in the
						!  proper colors
!		locate 1,1
		text to _temp_string
		if pausetext
			print pausetext;
		else
			RLibMessage(&TopPause,1) ! "PRESS A KEY TO CONTINUE";
		text to 0

		local alength
		alength = StringLength(_temp_string)
		print to (display.linelength/2 - alength/2);
		StringPrint(_temp_string)
	}
	color TEXTCOLOR, BGCOLOR, INPUTCOLOR
	Font(DEFAULT_FONT)
	HiddenPause ! PauseForKey
}

! Roody's note: GetNumber just exists so authors don't have to remember
! the word[]/parse$ thing when converting player's input to a number. Change
! the wordnum value if for some reason you want to check something other than
! word[1]

routine GetNumber(wordnum)
{
	local ret
	if not wordnum
		wordnum = 1
	! parse$ is set to -1 in regular command parsing and it
	! is set to 0 if the command "input" is used
	if word[wordnum] ~= -1,0
		ret = StringToNumber(word[wordnum])
	else
		ret = StringToNumber(parse$)
	return ret
}

! Roody's note: EnterNumber is an example routine using the above routine.
! Call with text t1 and t2 arguments if you want to change the prompt text
! without changing RlibMessage messages

routine EnterNumber(low,high,t1,t2)
{
	local ret
	if t1
		print t1;
	else
		RlibMessage(&EnterNumber,1) ! "Enter a number >";
	while true
	{
		input
		""
		if words = 1
			ret = GetNumber
		else
			word[1] = ""
		if ret
		{
			if high
			{
				if ret <= high and ret >= low
					return ret
			}
			elseif low
			{
				if ret >= low
					return ret
			}
			else
				return ret
		}
		elseif not low and word[1] = "0"
			return ret
		if t2
			print t2;
		else
		! "Enter a valid number between X and Y >";
			RlibMessage(&EnterNumber,2,low,high)
	}
}

!\ Roody's note: GrandParent is like Contains except it returns the
"grandparent" object. Written by Kent Tessman for Future Boy!. \!

! GrandParent returns an object's room object (presuming the room object is
! the last object in the tree before nothing)

routine GrandParent(obj)
{
	local gp
	gp = obj
	while parent(gp)
	{
		gp = parent(gp)
	}
	return gp
}

!\
Roody's note: This is an adapted version of Kent Tessman's PauseForKey routine
from Future Boy!. It uses trickery so the cursor doesn't show up
during a pause. This version, unlike the original, does not have code for
handling prompts, since I figured you can just handle that separately.
\!
routine HiddenPause
{
	local key

#ifset DEBUG
	if (debug_flags & D_PLAYBACK_HELPER) and betalib is playback_on
		return
#endif
	key = system(11) ! READ_KEY
	if system_status or system(61)  or ! MINIMAL_INTERFACE
	hidden_cursor_check is special
	{
		pause
		key = word[0]
	}
	else
	{
		while true
		{
			key = system(11) ! READ_KEY
			if key: break
			system(32) ! PAUSE_100TH_SECOND
		}
	}
	return key
}

! Roody's note: This routine serves as a reminder that when you change
! the player_person global variable, you also need to change the pronouns
! for your player object to match.

routine MakePlayer(player_object, playerperson)
{
	if not player_object
		return  ! just to avoid setting the player as the nothing object or
			     ! some other nonsense
#ifclear NO_OBJLIB
#ifset DEBUG
	if player_object.type ~= player_character
	{
		print newline
		Font(BOLD_ON)
		print "[WARNING:  The player global is set to an object that is
		not of type \"player_character\".]"
		Font(BOLD_OFF)
	}
#endif
	player = player_object
	location = parent(player)
	if playerperson
	{
		player_person = playerperson
		select player_person
			case 1
			{
				player.pronouns #1 = "I"
				player.pronouns #2 = "me"
				player.pronouns #3 = "my"
				player.pronouns #4 = "myself"
			}
			case 2
			{
				player.pronouns #1 = "you"
				player.pronouns #2 = "you"
				player.pronouns #3 = "your"
				player.pronouns #4 = "yourself"
			}
			case 3
			{
				if player is plural
				{
					player.pronouns #1 = "they"
					player.pronouns #2 = "them"
					player.pronouns #3 = "their"
					player.pronouns #4 = "themselves"
				}
				if player is female
				{
					player.pronouns #1 = "she"
					player.pronouns #2 = "her"
					player.pronouns #3 = "her"
					player.pronouns #4 = "herself"
				}
				else
				{
					player.pronouns #1 = "he"
					player.pronouns #2 = "him"
					player.pronouns #3 = "his"
					player.pronouns #4 = "himself"
				}
			}
#ifset DEBUG
			case else
			{
				print newline
				Font(BOLD_ON)
				print "[WARNING:  Incorrect playerperson value given to
					MakePlayer.]"
				Font(BOLD_OFF)
			}
#endif
	}
#endif
}

! Roody's note: Provide a variable and a single
! and plural form of a word and it will print
! the proper word when called.
! This is basically a variant of MatchPlural in hugolib.h (which
!  works off of objects).
!  Example:  MatchVariable(counter,"move","moves")
routine MatchVariable(var, word1, word2)
{
	print " ";
	if var = 1
		print word1;
	else
		print word2;
}

! Roody's note: This is based on Kent Tessman's PauseForKey routine where
! it has the option of printing a prompt, but it doesn't have the built-in
! cursor-locating and font adjustment of the original.

routine GetKeyPress(p)	! Where p is a prompt, if it ends up being used
{
	if p
		print p;
	return HiddenPause
}

! Roody's note: SpeakerCheck just replaces one of the common bits of code
!  in the main routine, mostly so the main routine becomes something authors
!  don't really need to touch (and just looks nicer).  You can replace this
!  routine if your game requires a more intensive check, like calling
!  the GrandParent routine or FindObject.

routine SpeakerCheck
{
	if speaking not in location
		speaking = 0
}

!----------------------------------------------------------------------------
!* DOVERSION SYSTEM
!----------------------------------------------------------------------------
! Roody's note: I figured it'd be nice if Roodylib provided a DoVersion
! routine by default. It's designed to be configurable and use whatever parts
! that you like, but you can always just straight up replace it with whatever
! you'd like.
!
! Just the same, you can #set NO_VERSION to disallow it.
!----------------------------------------------------------------------------

#ifclear NO_VERSION
#if defined GAME_TITLE ! skip this whole section if GAME_TITLE is not define

! Roody's note: I thought it'd be convenient to hold the compilation year
! in a global.

global build_year

object versionlib "versionlib"
{
	in init_instructions
	type settings
	execute
	{
!\  This automatically generates the current year at build time, stored in the
    global build_year    \!
		if word[LAST_TURN] ~= "undo","restore"
		{
			string(_temp_string, serial$, 8)
			local i,n, factor = 1
			for (i=7; i>5; i--)
			{
				n = _temp_string[i]
				if n >= '0' and n <= '9'
				{
					n -= '0'
					build_year += (n*factor)
				}
				elseif i = 0 and n = '-'
					build_year = -build_year
				else
					build_year = 0

				factor *= 10
			}
			build_year = build_year + 2000
		}
	}
}

! Roody's note: Replace this if you'd like to change the order or further tweak
! the DoVersion response.
routine DoVersion
{
	print GameTitle
#if defined BLURB
	 print BLURB ! "An Interactive Blahblahblah"
#endif
#ifclear NO_COPYRIGHT
	Copyright
#endif
	PrintBanner
	ReleaseAndSerialNumber
#if defined IFID
	print "IFID: "; IFID
#endif
#ifset BETA
	BetaNotes
#endif
#ifset DEMO_VERSION
	DemoNotes
#endif
	OtherNotes
}

routine TerpInfo
{
	if opcodeterp is not switchedon
		return
!	select hugor.os
!		case 1 : print "Windows ";
!		case 2 : print "MacOS ";
!		case 3 : print "Linux ";
	if opcodeterp.terp_name = "Hugor"
	{
		print " / Hugor v"; number opcodeterp.version ;"."; \
		number opcodeterp.version #2 ; "."; number opcodeterp.version #3
	}
}
! Roody's note: I changed TITLECOLOR to a global. Set it to something else in
! SetGlobalsAndFillArrays if you'd like to provide a special title color.

global TITLECOLOR = DEF_FOREGROUND

routine GameTitle
{
	color TITLECOLOR
	Font(BOLD_ON | ITALIC_OFF)
	print GAME_TITLE;
	Font(BOLD_OFF | ITALIC_OFF)
	color TEXTCOLOR
#ifset DEMO_VERSION
	print "\B (demo version)\b";
#endif
#ifset HUGOFIX
	print "\I (HugoFix Debugging Suite Enabled)\i";
#endif
}

routine Copyright
{
#if defined AUTHOR
	print "Copyright \#169 ";
#if defined FIRST_PUBLICATION
	print FIRST_PUBLICATION;
#endif
#if undefined FIRST_PUBLICATION
	print number build_year;
#endif
	" by ";
	print AUTHOR
#endif
}

routine PrintBanner
{
	print BANNER; " / ";
	print "Roodylib "; ROODYVERSION ;
	if opcodeterp is switchedon
	{
		TerpInfo
	}
	print newline
}

routine ReleaseAndSerialNumber
{
	string(_temp_string, serial$, 8)
#if defined RELEASE
	print "Release "; RELEASE; " / ";
#endif
#if clear BETA
	print "Serial Number ";
	StringPrint(_temp_string, 0, 2)
	StringPrint(_temp_string, 3, 5)
	StringPrint(_temp_string, 6, 8)
#else
	" \BBETA BUILD # 20";
	StringPrint(_temp_string, 6, 8)
	StringPrint(_temp_string, 0, 2)
	StringPrint(_temp_string, 3, 5)
	Font(BOLD_OFF)
#endif
	print newline
}

#ifset BETA
routine BetaNotes
{
	color TITLECOLOR
	"\n\BDO NOT DISTRIBUTE!\b";
	color TEXTCOLOR
	" This beta release is intended for testing only, not for
	distribution to the general public.";
#if defined EMAIL
   " Please report any errors,
	bugs, etc. to \I<";
	print EMAIL;">\i."
#endif
	print newline
	if betalib is not special
		"HI, TESTERS:  Please type \B>SCRIPT ON\b and send in your transcripts."
}
#endif ! ifset BETA

#ifset DEMO_VERSION
routine DemoNotes
{
#if defined WEBSITE
	"\n(Like this demo?  Visit \B";
	print WEBSITE; "\b for the full version.)"
#endif
}
#endif ! DEMO_VERSION

! Roody's note: This routine provides "new players type COMMAND" text. I use
! it to list the command for accessibility options in interpreters that may
! support screenreader.  If your game has help/about text, replace this
! routine and change the commented line to fit your game.
routine OtherNotes
{
	local a, b
!	a = "New players type \"help\""
#ifclear NO_ACCESSIBILITY
	if screenreader_capable is special or (TERP & GLK_TERP) or
		(TERP & SIMPLE_TERP)
		b++
#endif
	if a or b
	{
		print "(";
		if a
			print a;
		if a and b
			print "."; AFTER_PERIOD;
		if b
			print "Type \"a11y\" for accessibility commands";
		if a and b
			print ".";
		")"
	}
}
#endif ! if defined GAME_TITLE
#endif ! ifclear NO_VERSION

!----------------------------------------------------------------------------
!* REPLACED LAST LIBRARY OBJECT
!----------------------------------------------------------------------------

! Roody's note: We replace the "last_library_object" last so it is
! gets a higher object number than any previous object replacements.
!----------------------------------------------------------------------------
#ifset DEBUG
replace last_library_object
{}
#endif

!----------------------------------------------------------------------------
!* ROODYLIB MESSAGES
!----------------------------------------------------------------------------

! Roody's note:  RLibMessage handles both hugolib and verb routine messages
! as it's not really necessary to split them up into multiple routines.
! Still, objlib-esque messages are handled by RLibOMessage, which can be
! found further below.

routine RLibMessage(r, num, a, b)
{
	! Check first to see if the NewRLibMessages routine provides a
	! replacement message:
	if NewRLibMessages(r, num, a, b):  return

	select r
#ifset BETA
		case &BetaInit
		{
			select num
				case 1 : "This is a beta release! If you'd like to start a
				transcript right away, press \BB\b. Otherwise, push any other key
				to begin without starting a transcript."
				case 2
				{
					print "Starting beta transcript";
#if defined GAME_TITLE
					print " for "; GAME_TITLE ;
#endif
					if (TERP & NORMAL_TERP) and opcodeterp is not switchedon
					{
						"."
						return
					}
					print " (";
					if opcodeterp is switchedon
					{
						if opcodeterp.terp_name = "Hugor"
						{
							print "Hugor v"; number opcodeterp.version ;"."; \
								number opcodeterp.version #2 ; "."; \
								number opcodeterp.version #3 ; " ";
						}
						select opcodeterp.os
							case 0: print "Unknown";
							case 1: print "Windows";
							case 2: print "MacOS";
							case 3: print "Linux";
							case 4: print "iOS";
							case 5: print "Android";
							case 6: print "Browser";
					}
					else
					{
						select TERP
							case SIMPLE_TERP: print "minimal Hugo port";
							case GLK_TERP : print "glk Hugo port";
					}
					print "). Preface input with an asterisk to
				leave notes for the author ("; prompt ;"* the potato isn't
				edible!)."
				}
				case 3 : "No transcript started."
		}
#endif ! ifset BETA
		case &CheckReach
		{
			select num
				case 1
				{
					print capital The(a) ; IsorAre(a,true);
					if parent(a).prep
						print " "; parent(a).prep; " ";
					else
						print " inside ";
					print The(parent(a)); "."
				}
		}
		case &CoolPause
		{
			select num
				case 1  ! default cheapglk "press a key"
				{
					Indent
					if a
						print a! ;
					else
						print "[PRESS A KEY TO CONTINUE]" !;
				}
				case 2  ! default normal "press a key"
				{
					Indent
					Font(ITALIC_ON)
					if a
						print a ! was ;
					else
						"press a key to continue" ! was ;
					Font(ITALIC_OFF)
				}
		}
		case &TopPause
		{
			select num
				case 1: print "[PRESS A KEY TO CONTINUE]";
		}
		case &DescribePlace
		{
#ifset USE_ATTACHABLES
			select num
				case 1
				{
					print CThe(a) ; " that "; The(player) ; IsorAre(player);
					print " holding is "; a.attached_desc ; " ";
					ListAttachments(a)
					print ".";
				}
#endif ! USE_ATTACHABLES
		}
#ifclear NO_ACCESSIBILITY
		case &DoCheapToggle
		{
			select num
				case 1: "\"Cheap\" mode toggled off."
				case 2: "\"Cheap\" mode toggled on."
		}
#endif
		case &DoDrink
		{
			print CThe(player); " can't ";
			print word[1]; " ";
			print player.pronoun #4; "."
		}
		case &DoEat
		{
			print CThe(player); " can't ";
			print word[1]; " ";
			print player.pronoun #4; "."
		}
		case &DoEnter
		{
			select num
			case 1:
			{
				local x = 1
				print CThe(player); " can't ";
!				if object is platform
!					print "get on ";
!				else
!					print "enter ";
				while VerbCheck(word[x])
				{
					print word[x]; " ";
					x++
				}
				if object = player
				{
					print player.pronoun #4;
				}
				else
					print The(object);
				"."
			}
		}
		case &DoExit
		{
			select num
				case 1
				{
					print "Trying to get out of "; player.pronoun #3; " head";
					if player is plural and player_person ~= 2
					{
						print "s";
					}
					print "? Futile."
				}
				case 2
				{
					print CThe(player); " get"; MatchSubject(player); " ";
					PrintOppositePreposition(object)
					print " of "; The(object); "."
				}
		}
		case &EnterNumber
		{
			select num
				case 1: print "Enter a number >";
				case 2
				{
					print "Enter a valid number";
					if b
					{
						print " between "; number a ;
						print " and "; number b ;
					}
					print " >";
				}
		}
		case &DoGet
		{
			select num
				case 1
				{
					print CThe(a); IsorAre(a,1); " wearing"; \
					MatchPlural(object,"that","those");"."
				}
				case 2 ! called with the AUTOMATIC_EXAMINE switch
				{
					print capital player.name; " pick";
					MatchSubject(player)
					print " up "; The(object);".";
				}
		}
		case &DoGo
		{
			select num
				case 1
				{
				! thought I might put some elevated platform code here
				!  but haven't yet
					print CThe(player); " will have to get ";
					PrintOppositePreposition(parent(player))
					print " of "; The(parent(player)); " first."
				}
				case 2	!  going to a non-enterable object in the same room
					print Cthe(object) ; IsorAre(object,true) ;" right here."
				case 3
				{
					print CThe(player); " can't go that way."
				}
		}
		case &DoHit
		{
			print "Venting "; player.pronoun #3; " frustrations
			on "; player.pronoun #4; " won't accomplish much."
		}
		case &DoListen
		{
			select num
				case 1   ! default character-listening
					print CThe(object); IsorAre(object,true); " quiet."
				case 2   ! default object-listening
					"That would serve no purpose."
				case 3   ! default location-listening
				{
					print capital player.pronoun #1;
					MatchPlural(player, "hears", "hear")
					" nothing unexpected."
				}
		}
		case &DoLookUnder
		{
			print CThe(player); \
			MatchPlural(player, "doesn't", "don't"); \
			" find anything "; word[2]; " "; \
			player.pronoun #4; "."
		}
		case &DoMove
		{
			select num
				case 1
				{
					print CThe(player); " can't ";
					print word[1]; " ";
					print player.pronoun #4; "."
				}
		}
		case &DoOpen
		{
			select num
				case 1: print "Opening "; The(object);" reveals";
		}
		case &DoPushDirTo
		{
        ! Let's set default DoPushDirTo messages
			select num
				case 1: "That would not help the present situation."
				case 2
				{
					print "Try "; word[1] ; "ing "; Art(object); " in a direction."
				}
				case 3
				{
					if word[1] = "push","press"
						a = "es "
					else
						a = "s "
					print CThe(player); " "; word[1];
					if player is not plural
					{
						print a;
					}
					print " "; The(object); " over to the..."
				}
				case 4:  print CThe(object); " slows to a stop."
		}
	case &DoPutIn
	{
		select num
		case 1:
		{
			print "You'll have to be a little more specific
				about exactly where you'd like ";
			if player_person ~= 2:  print The(player, true); " ";
			print "to "; word[1] ; " that."
		}
	}
#ifclear NO_XVERBS
		case &DoQuit
		{
			select num
				case 1
					"Continuing on."
		}
		case &DoRestart
		{
			select num
				case 1 : print "\nOk, continuing on."
		}
		case &DoRecordOnOff
		{
			select num
				case 1: "No playback in progress."
				case 2: "Playback completed."
				case 3: "No playback while recording."
				case 4: "Playback already in progress."
				case 5
					"Pressing the \"+\" key at a MORE prompt may skip
					through the rest of the MORE prompts, depending on your
					interpreter."
				case 6: print "Skip in-game pauses and \"Keep waiting?\" prompts
				during playback? ";
				case 7: "No recording during playback."
				case 8: "Recording already in progress."
				case 9: "No recording in progress."

		}
		case &DoScriptOnOff
		{
			select num
				case 1: "Transcription is already on."
				case 2
				{
					print "Transcript started";
					if location
					{
						print " at \""; location.name;
					}
					if STATUSTYPE and STATUSTYPE ~= 4
					{
						if STATUSTYPE = TIME_STATUS, MILITARY_TIME
						{
#ifset AMERICAN_ENGLISH
							print ".\" ";
#else
							print "\". ";
#endif
							print "The game time is "; HoursMinutes(counter);
						}
						else
						{
							print " at turn "; number counter ;
						}
						print " and"; MatchVariable(score, "point","points"); \
						 " have been scored";
					}
					". Preface input with an asterisk to
					leave notes for the author (";
					print prompt ;"* the potato isn't
					edible!)."
				}
				case 3: "Transcription is not currently on."
				case 4: "Comment recorded!"
				case 5: "Comment not recorded! (Transcription is not on.)"
		}
#endif ! NO_XVERBS
		case &DoTakeOff
		{
			select num
				case 1
					print CThe(xobject); " doesn't let "; player.pronoun #2;"."
		}
		case &DoUnlock
		{
			select num
				case 1
				{
					print "Be more specific about what you want to unlock "; The(object);
					print " with."
				}
				case 2
				{
					print capital player.pronoun #1;
					print " can't reach "; The(a); ", which is currently in ";
					print The(parent(a)); "."
				}
				case 3
				{
					if a
						print "(with "; The(xobject); ")"
					print "Unlocked."
				}
		}
		case &DoLock
		{
			select num
				case 1
				{
					print "Be more specific about what you want to lock "; The(object);
					print " with."
				}
				case 2
				{
					if a
						print "(with "; The(xobject); ")"
					print "Locked."
				}
		}
	case &EndGame
	{
		select num
		case 1
		{
			print "\nThe game has ended.  Do you want to (R)ESTART";
#ifclear NO_XVERBS
			print ", R(E)STORE a saved game";
#ifset NO_UNDO
			print ",";
#endif
#endif
#ifclear NO_UNDO
			if not UNDO_OFF         ! if not otherwise overridden
				print ", (U)NDO your last turn,";
#endif
			print " or (Q)UIT? ";
		}
		case 2
		{
			print "Enter (R)ESTART";
#ifclear NO_XVERBS
			print ", R(E)STORE";
#ifset NO_UNDO
			print ",";
#endif
#endif
#ifclear NO_UNDO
			if not UNDO_OFF
				print ", (U)NDO,";
#endif
			print " or (Q)UIT: ";
		}
	}
#ifset LIST_CLOTHES_FIRST
		case &ListClothesFirst
		{
			select num
				case 1
				{  ! "You are wearing";
					if (FORMAT & USECHARNAMES_F)
!						print capital a.name;
						CArt(a)
					else
						print capital a.pronoun;
						IsOrAre(a,1)
						print " wearing";
				}
		}
#endif
		case &ListObjects
		{
			select num
				case 1: print " "; a;
				case 2: print ":"
		}
		case &Parse
		{
			select num
				case 1: "You will need to type that in again."
		}
		case &QuitGameText
		{
			"\IThanks for playing!\i"
		}
		case &UndoResponse
		{
			select num
				case 1:	"Undone."
				case 2
				{
					if UNDO_OFF
						"UNDO is not currently allowed."
					else
						print "Unable to undo."
				}
		}
		case &WhatsIn
		{
			select num
				case 1: print ":"
				case 2
				{
					print "Also ";
					if a.prep
						print a.prep; " ";
					elseif a is platform
						print "sitting on ";
					else
						print "inside ";
				}
				case 3
				{
					if a.prep
						print capital a.prep; " ";
					elseif a is platform
						print "Sitting on ";
					else
						print "Inside ";
				}
		}
		case &YesorNo
		{
			select num
			case 1
			{
				print "Please answer YES or NO: ";
			}
			case 2
				print "YES or NO: ";
		}
}

! Roody's note:  New routine called by DoGo (when on or in something) or
! successfully exiting an object with DoExit
routine PrintOppositePreposition(obj)
{
#ifset USE_SUPERCONTAINER
	if obj.type = SuperContainer
	{
		if InList(obj, contents_in, player)
			print "out";
		else
			print "off";
	}
	else
#endif
	if obj.prep #2
		print obj.prep #2;
	elseif obj is platform
		print "off";
	else
		print "out";
}

routine NewRlibMessages(r, num, a, b)
{
	select r
		case else : return false
	return true ! this line is only reached if we replaced something
}

#ifclear NO_OBJLIB
routine RlibOMessage(obj, num, a, b)
{
	! Check first to see if the New<blank>OMessages routine provides a
	! replacement message:
	!
	if NewRlibOMessages(obj, num, a, b):  return

	select obj

	case character
	{
		select num
			case 1
			{
				print CThe(xobject) ; " allows "; The(player,true) ; " to take ";
				print The(object); "."
			}
	}
	case door
	{
		select num
			case 1
			{
				print "(and then opening it)"
			}
			case 2
			{
				print "(unlocking "; Art(self);
				if a
					print " with "; The(a);
				" first)"
			}
			case 3
			{
				if actor ~= player
					""
				CThe(actor)
				print " open"; MatchSubject(actor); " "; \
				The(self); " and ";

				if actor in location
					print "leave"; MatchSubject(actor);
				elseif location = self.between #((parent(actor) = \
					self.between #1) + 1)
					print "come"; MatchSubject(actor); " in";
				print ", closing it behind ";
				if actor is not plural or actor = player
					print actor.pronoun #4; "."
				else
					print "themselves."
			}
	}
#ifset USE_VEHICLES
	case vehicle ! added in case I need to add any elevated special text
	{
		select num
		case 1
		{
			print "To walk, "; The(player); " will have to
				get "; self.prep #2; " of "; The(self); \
				".  Otherwise, try \""; self.vehicle_verbs; \
				"\" ";
			if self.#vehicle_verbs > 1
				print "or \""; self.vehicle_verbs #2; "\" ";
			print "and a direction."
		}
		case 2
		{
			print CThe(player); IsorAre(player); \
				" not ";
			if word[1] = "ride", "gallop", "trot", "canter"
				print "on ";
			else
				print "in ";
			"anything at the moment."
		}
		case 3
			"Good luck with that."
	}
#endif
#ifset USE_ATTACHABLES
	case attachable
	{
		select num
		case 1
		{
			local i, td
			print CThe(player); " can't move "; The(b); " while ";
			for (i = 1;i <= a.attached_to ;i++ )
			{
				if a.attached_to #i ~= b and a.attached_to #i is not mobile and
					a.attached_to #i is static
				{
					td = a.attached_to #i
					break
				}
			}
			if td
			{
				print The(a); IsorAre(a, true); " still "; \
				a.attached_desc; " to "; The(td);
			}
			else
			{
				print " still "; \
				a.attached_desc; " to "; The(a);
			}
			print "."
		}
		case 2
		{
			print CThe(player); " can't "; VerbWord; " "; \
			The(object); " "; object.attach_prep; " ";
			if xobject = player
				print player.pronoun #4; "."
			else
			   print The(xobject); "."
		}
	}
#endif
}

routine NewRlibOMessages(obj, num, a, b)
{
	select obj

!	case <object>
!	{
!		select num
!		case 1:
!	}

		case else : return false

	return true ! this line is only reached if we replaced something
}
#endif ! ifclear NO_OBJLIB

#endif ! _ROODYLIB_H
