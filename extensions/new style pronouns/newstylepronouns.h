!----------------------------------------------------------------------------
!* "NEW_STYLE_PRONOUNS"
!----------------------------------------------------------------------------
!\
Roody's note: hugolib.h has a pronoun system that is automatically turned off
(due to Kent being unhappy with certain behaviors). Here in Roody-land, though,
we like to live on the EDGE and dance with danger.

It is different from the normal system as it provides pronoun objects that
redirect actions to the applicable pronoun global variable (it_obj, him_obj,
etc.).  In Hugo's normal behavior, "it" (or other pronouns) get replaced with
the name of the current it_obj during the Parse stage.  This system processes
the command as referring to the it_object then applies the behavior to the
it_obj later.

Originally, I thought that one of the perks of this system was that it didn't
always print "(Assuming you mean the so-and-so)" text (which can be nice in a
game with more behind-the-scenes trickery to hide), but that turned out to be
due to some mishandled code.

So, the nicest thing about this system is that it avoids all of the word
array manipulation the regular system uses, if it bothers you that Hugo
is modifying the word array behind the curtain.

The main thing to watch out if you use the new style pronouns is that
verb definitions that use object-specific grammar tokens will not allow
referring to those objects with pronouns.

So, if you use the new style pronouns, you should replace those grammar tokens
with routine grammar tokens, like:

The box1-object-specific verb definition:
verb "test"
*  (box1)                   DoTest

would be replaced with:
verb "test"
*  (TestRoutine)                   DoTest

with this accompanying grammar routine:
routine TestRoutine(obj)
{
	if CheckObject(obj, box1)
		return true
}

\!

! If NEW_STYLE_PRONOUNS is not set, the Parse routine will actually change
! the input line by inserting the object name.  This is a different
! approach using actual stand-in objects.

object it_object "that" ! to match "You don't see that." ParseError message.
{
	is known, hidden
	nouns "it"
	type it_object
#ifset ROUTINE_GRAMMAR_HELPER
	grammar_rules 0
#endif
	misc { return it_obj }
	parse_rank 1000
	found_in
	{
		local obj
		obj = self.misc
		if FindObject(obj, location)
			return location ! parent(obj)
		else
			return false
	}
}

it_object him_object "him"
{
	is living
	nouns "him"
	misc { return him_obj }
}

it_object her_object "her"
{
	is living, female
	nouns "her"
	misc { return her_obj }
}

it_object them_object "them"
{
	is plural
	nouns "them"
	misc { return them_obj }
}


object parse_pronouns
{
	in preparse_instructions
	type settings
	execute
	{
		move it_object to parent(it_obj)
		move him_object to parent(him_obj)
		move her_object to parent(her_obj)
		move them_object to parent(them_obj)
		return false
	}
}

object newpronounslib "newpronouns"
{
	type settings
	in init_instructions
	execute
	{
		if word[LAST_TURN] ~= "undo","restore"
		{
			! Only need to set up all attributes once, so that a given
			! pronoun will satisfy any grammatical attribute requirements
			if it_object is not 127
			{
				local i
				for (i=0; i<128; i++)
				{
					it_object is i
					him_object is i
					her_object is i
					them_object is i
				}
			}
		}
	}
}

replace ExcludeFromPronouns(obj)
{
	if obj >= it_object and obj <= them_object
		return true
	elseif obj = player:  return true
#ifclear NO_OBJLIB
	elseif obj.type = direction : return true
#endif
	return false
}

!\ ObjectisLight replacement - Ran into a problem if Hugo thinks the it_object
  is providing light, so this extra code redirects ObjectisLight to the
  applicable object. \!

replace ObjectisLight(obj)
{
	local nextobj

	if obj = it_object:		obj = it_obj
	elseif obj = him_object:	obj = him_obj
	elseif obj = her_object:	obj = her_obj
	elseif obj = them_object:	obj = them_obj

	if obj is light
	{
		light_source = obj
		return light_source
	}
	if obj is not container or
		(obj is container and obj is not openable) or
		(obj is container and obj is openable and
			(obj is open or obj is transparent))
	{
		for nextobj in obj
		{
			if ObjectisLight(nextobj)
				return light_source
		}
	}
}

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
		local count
		for (count=2; count<=words and word[count]~=""; count++)
		{
			select word[count]
				! Make sure pronoun refers to something
				case "it", "them", "him", "her"
				{
					select word[count]
						case "it" :     a = it_obj
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
				}
		}

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

replace DeactivateParser
{
	last_actor = actor
	parser_data[LAST_PARSER_STATUS] &= ~PRONOUNS_SET
	if object.type = it_object or xobject.type = it_object
	{
		SetPronouns
		ApplyPronouns(object, xobject)
	}
	else
	{
		AssignPronoun(object)
		parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
	}
	parser_data[PARSER_STATUS] &= ~PARSER_ACTIVE
	SaveOldWord
}

! Roody's note: SetPronouns exists as its own routine so you can easily
! establish rules for what object gets the pronoun depending on the verb.
! For instance, in >PUT OBJECT IN THING, you may want to set the pronoun to
! THING instead of OBJECT
routine SetPronouns
{
	local i
	if object = it_object
	{
		select verbroutine
		case &DoOpen, &DoClose: i = openable
		case &DoLock, &DoUnlock : i = lockable
		case &DoWear, &DoTakeOff : i = clothing
		case &DoLook
			{
			if word[1] = "read","peruse"
				i = readable
			}
		case &DoSwitchOn, &DoSwitchOff, &DoSwitchOnOff : i = switchable
	}

	if i and it_obj is not i
	{
		if previous_object is i and (previous_object is not living, plural)
			it_obj = previous_object
		elseif previous_xobject and previous_xobject is i and
			(previous_xobject is not living, plural)
			it_obj = previous_xobject
	}

	if ((xobject = him_object and object.pronouns = "he") or
		(xobject = her_object and object.pronouns = "she")) and
		verbroutine = &DoAsk, &DoTalk
	{
		local w
		w = word[CurrentCommandWords]
		if previous_xobject and w = previous_xobject.pronoun #2 and
			previous_xobject ~= object.misc
		{
			select w
				case "him" : him_obj = previous_xobject
				case "her" : her_obj = previous_xobject
		}
		elseif w = previous_object.pronoun #2 and
			previous_object ~= object.misc
		{
			select w
				case "him" : him_obj = previous_object
				case "her" : her_obj = previous_object
		}

	}
}

routine ApplyPronouns(obj,xobj)
{
	local number_pronouns = 0
	if object = it_object:		object = it_obj
	elseif object = him_object:	object = him_obj
	elseif object = her_object:	object = her_obj
	elseif object = them_object:	object = them_obj
	if object ~= obj
	{
		replace_pronoun[number_pronouns] = object
		number_pronouns++
	}
	if xobject = it_object:		xobject = it_obj
	elseif xobject = him_object:	xobject = him_obj
	elseif xobject = her_object:	xobject = her_obj
	elseif xobject = them_object:	xobject = them_obj
	if xobject ~= xobj
	{
		replace_pronoun[number_pronouns] = xobject
		number_pronouns++
	}
	if number_pronouns
	{
		parser_data[LAST_PARSER_STATUS] |= PRONOUNS_SET
		PrintReplacedPronouns(number_pronouns)
	}
}

replace MakeParserNotes(obj,is_found)
{
	select obj
		case it_object : obj = it_obj
		case him_object : obj = him_obj
		case her_object : obj = her_obj
		case them_object : obj = them_obj

	if not obj
		return

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
#ifclear NEW_STYLE_PRONOUNS
					! If we built an object phrase using
					! OLD_STYLE_PRONOUNS, avoid printing a
					! potential nonsense name:
					if parser_data[PARSER_STATUS] & PRONOUNS_SET
						print "thing like that";
					else
#endif
						print " \""; parse$; "\"";
					print ", nor"; IsorAre(player, true); " "; The(player); \
					" likely to, even if such a thing exists."
\!
				print CThe(player); \
					MatchPlural(player, "hasn't", "haven't");
					print " encountered any";
					print " \""; parse$; "\"";
				print ".";
				print "  (If you're sure you need to refer to that, try
					putting it another way.)"
			}
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
				if obj.type = it_object
					print " "; obj.name;
				elseif (obj.pronouns #2)
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
		case else : return false
	return true
}