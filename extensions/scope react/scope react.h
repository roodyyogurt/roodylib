!----------------------------------------------------------------------------
!* SCOPE_REACT
!----------------------------------------------------------------------------

!\ With this extension, the react_before/react_after properties of scope
items are also checked.  \!

object scope_objects
{}

object scopelib "scope organizer"
{
	in init_instructions
	type settings
	execute
	{
		if word[LAST_TURN] ~= "undo"
		{
			if word[LAST_TURN] ~= "restore" ! new game
			{
				! let's organize the scope objects at start of game
				local i, n
				for (i=1;i<=objects ;i++ )
				{
			#ifset USE_PLURAL_OBJECTS
					n = 0
					if i.type = plural_class, identical_class
						n = 1
			#endif
					if i.#found_in or i.#in_scope and not n
					{
						if i.type ~= i and parent(i) = nothing and i.type ~= fuse and
							i.type ~= daemon
							move i to scope_objects
					}
				}
			}
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

	for i in scope_objects
	{
		if InList(i, found_in, location)
			r = i.react_before
		if not r and InList(i, in_scope, player)
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
			break
		}
	}

	if verbroutine = &MovePlayer
		return r

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

	for i in scope_objects
	{
		if InList(i, found_in, location)
			r = i.react_after
#ifset DEBUG
		if debug_flags & D_PARSE and r
		{
			print "\B["; i.name;
			if debug_flags & D_OBJNUM
				print " ["; number i; "]";
			print ".react_after returned "; number r; "]\b"
		}
#endif
		if InList(i, in_scope, player)
			r = i.react_after
#ifset DEBUG
		if debug_flags & D_PARSE and r
		{
			print "\B["; i.name;
			if debug_flags & D_OBJNUM
				print " ["; number i; "]";
			print ".react_after returned "; number r; "]\b"
		}
#endif
	}
}