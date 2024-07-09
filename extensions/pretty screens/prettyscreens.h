replace LinesFromTop
{
	if not (display.hasvideo or top_justified_check is special)
		return display.windowlines
	else
		return 2
}

object parse_redraw
{
	in preparse_instructions
	type settings
	execute
	{
		if display.needs_repaint
		{
			if is_fluid_layout is not special
			{
				if RepaintScreen
					RedrawScreen
			}
			else
				display.needs_repaint = false
		}
		return false
	}
}
!#endif ! NO_FANCY_STUFF
!\ Roody's note: RedrawScreen is a generic routine for redrawing the screen
after a screen-size change. Ideally, it should be called in PreParse.
\!

!global oldwidth

routine RedrawScreen
{
	! if the screen size has changed, we'll clear the screen,
	! print the player's command, and redraw the status line before
	! proceeding to interpret the command

	InitScreen
	PrintStatusLine
	ShowCommand

}

!\ Roody's note: Split the screen-clearing stuff into another routine
   so EndGame and such could use it, too.   \!

#if undefined REPAINT_TIME
constant REPAINT_TIME 30
#endif

replace RepaintScreen
{
	display.needs_repaint = false
#ifclear NO_ACCESSIBILITY
!\ This little section is a little useless since it really only applies to
	non-simple interpreters with cheap mode turned on for testing purposes. \!
	if (cheap & CHEAP_ON)
	{
		CenterTitle(CheapTitle,0,1,1)
		return
	}
#endif ! CHEAP
	"[Detected screen size change; redrawing screen...]"
	""
	local get_key, a
	get_key = system(11) ! READ_KEY
	if not (system_status or system(61)) ! 61 = MINIMAL_PORT
	{
		while a < REPAINT_TIME ! 100
		{
			if system(11)
				break
			system(32) ! wait 1/100th second
			a++
		}
	}
	return true
}

!\ Roody's note: ShowCommand retypes the typed (valid)  command after the
 screen has been cleared during a screen size.  \!

routine ShowCommand
{
	print prompt;
	local i = 1, showfullstops, v, skip_word
	if INPUTCOLOR ~= MATCH_FOREGROUND
		color INPUTCOLOR
	while word[i] ~= ""
	{
		if word[i] ~= "~and"
			v = v or VerbCheck(word[i])
		select word[i]
			case "~and"
			{
				if not VerbCheck(word[(i+1)]) or
					(v and word[(i-1)] = "~and") or
					(VerbCheck(word[(i+1)]) and v)
				{
					if word[(i+1)] = "~and"
						skip_word = true
					else
						print "and";
				}
				else
				{
					print ",";
!					if word[(i+1)] = "~and"
!						print " ";
				}
			}
			case "~all"
				print "all";
			case "~except" : print "except";
			case "~oops" : print "oops";  ! hopefully never called
			case "~any" : print "any";
			case else
				print word[i];
		if word[++i] ~= ""
		{
			if skip_word
				skip_word = false
			elseif not (word[i] = "~and" and VerbCheck(word[(i+1)]) and not v)
				print " ";
		}
		elseif word[++i] ~= ""
		{
			showfullstops = true
			print ". ";
		}
		if word[i] = "" and showfullstops
			print ".";
	}
	print ""
	color TEXTCOLOR, BGCOLOR, INPUTCOLOR
}
