!----------------------------------------------------------------------------
!* NEW PRINTSTATUSLINE
!----------------------------------------------------------------------------
!\
Like the other "settings" stuff further down, Roodylib's PrintStatusLine
uses objects to keep track of its settings. The intended benefits of using it
are:
- built in support for varying screen widths and screen size changes
- "out-of-the-box" compatibility with several status-line-changing extensions,
  like newconverse and newautomap.
- makes it easy to have non-standard statuses that opt for player state
  instead of turns/score
\!

#ifclear _NEWSTATUSLINE_H
#set _NEWSTATUSLINE_H

#ifset VERSIONS
#message "NewPrintStatusLine.h Version 1.0"
#endif

! PrintStatusLine object properties
property find_height alias u_to ! property that points to routines for
                                !   determining status window height
property draw_window alias e_to ! routines with instructions for drawing the
                                !   status window
property bottom_justified alias d_to ! have this return true for status windows
                                    !   where regular status information shares
												!  a window with other information and you
												!	 want the regular status information to
												!	 be printed at the bottom of the window
!property terp_type alias w_to   ! this property gets set to the current
!                                !   interpreter type automatically
property status_override alias nw_to ! status_override - normally, the status
                                     !   window object with the highest
                                     !   find_height number gets drawn (so have
												!	 those properties return 0 when not in
                                    !    use), but sometimes more than one
												!	 extension *could* be drawn, so we use
                                    !   status_override to have one override the
												!	 other (have it return true to initiate
                                    !    such override)
property status_top alias sw_to     ! top line number of the status window, if
                                    ! applicable
property chosen_window alias nw_to  ! set to the window instructions object
                                    !  whose draw_window property will be
												!	executed. authors can ignore this.

!\ a PrintStatusLine object in which we will put our instruction objects in (by
inclusion of extensions and what not. PrintStatusLine will call this directly).
\!
object printstatuslib
{
	in settings
	type settings
	find_height
	{
		local highest, i, a
		if children(self) ~= 1
		{
			i = child(self)
			while i
			{
				a = i.find_height
				if i.status_override or a > highest
				{
					self.chosen_window = i
					highest = a
					if i.status_override
						break
				}
				i = younger(i)
			}
		}
		else
		{
			i = child(self)
			self.chosen_window = i
			highest = i.find_height
		}

		self.status_top = (self.chosen_window).status_top
		return highest
	}
	draw_window
	{
		run (self.chosen_window).draw_window
	}
	chosen_window 0
!	terp_type NORMAL_TERP
	bottom_justified 1
	status_top 0
}

! Roody's note: This object checks for a big change in the status line size
! that'd require the window to be cleared.
object statuslinelib "statuslinelib"
{
	in init_instructions
	type settings
!#ifclear NO_FANCY_STUFF
	save_info
	{
		if display.statusline_height > 2 and not system(61)
		{
			SaveWordSetting(display.statusline_height)
			return true
		}
		return false
	}
!#endif
	execute
	{
		if word[LAST_TURN] = "undo","restore"
		{
!#ifclear NO_FANCY_STUFF
			local a
			a = CheckWordSetting(self.name)
			display.statusline_height = word[(a+1)]
!#endif
		}
	}
}

replace PrintStatusline
{
	local newstatusheight, window_top
#ifset NO_ACCESSIBILITY
	local cheap
#endif

	if cheap = 1
		return

	! figure out the size our window will be
	newstatusheight = printstatuslib.find_height
	window_top = printstatuslib.status_top

	if not window_top
		window_top = 1

	if not TERP
		run roodylib.execute

	! clear/remove the window if the status window height has changed
	if (newstatusheight < display.statusline_height) and not system(61) and
	display.statusline_height
	{
		window 1, window_top, display.screenwidth,
		(display.statusline_height + window_top - 1)
		{cls} ! clear whatever's there
		window 0
	}

	display.statusline_height = newstatusheight

	Font(BOLD_OFF | ITALIC_OFF | UNDERLINE_OFF | PROP_OFF)
	if not (TERP & NORMAL_TERP)
	{
		window display.statusline_height
		{
			if not (TERP & SIMPLE_TERP)
			{
				cls
				locate 1,1
			}
			run printstatuslib.draw_window
		}
	}
	else
	{
		window 1, window_top, display.screenwidth,
		(display.statusline_height + window_top - 1)
		{
			color SL_TEXTCOLOR, SL_BGCOLOR
			cls
			locate 1,1
			run printstatuslib.draw_window
		}
	}
	color TEXTCOLOR, BGCOLOR, INPUTCOLOR
	Font(DEFAULT_FONT)
	ExtraWindows
}

property counter_length alias se_to

! Here is an example status window instruction object. It and its routines
! draw a regular status window
object statuswindow
{
	in printstatuslib
	find_height
	{
		return (call &FindStatusHeight)
	}
	draw_window
	{
		return (call &WriteStatus)
	}
	counter_length 0
}
!\ Note: These properties *could* just say "return FindStatus". I just used
the above syntax to give a clue as to how one would change a value "mid-game".
If you want statuswindow.find_height to point to *another* routine, you could
have a line like this:
	statuswindow.find_height = call &FindNewStatusHeight  \!


! routine for finding the height of the regular status info
routine FindStatusHeight
{
	local len1, b
	if not light_source
		len1 = 11
	else
		len1 = string(_temp_string,location.name)
	if STATUSTYPE
	{
		b = GetStatusLength(len1)
		statuswindow.counter_length = b
	}
	else
	{
		b = 1
		while display.screenwidth < (b*len1)
		{
			b++
		}
		return b
	}
	if (b + len1 + 4)<display.screenwidth ! let's force a 4 character gap between
	{                              ! the two fields
		return 1
	}
	else
		return 2
}

! Roody's note: Replace this if you want to use the top right area
! for something else ("HUNGRY", "TIRED", or whatever)
replace STATUSTYPE4
{}

! routine for drawing the regular status
routine WriteStatus
{
	local row,a
	if printstatuslib.bottom_justified and
		not (TERP & SIMPLE_TERP)
	{
		if display.statusline_height > 1
			row = display.statusline_height - 1
		else
			row = display.statusline_height
		locate 1, row
	}
	else
		row = 1
	PrintStatusLineLocation ! "in the dark" or location name
	if display.statusline_height = 1 and STATUSTYPE
	{
		a = display.linelength
		print to (a - \
			( statuswindow.counter_length + \
			(TERP = SIMPLE_TERP)* 2 - \
			(TERP = GLK_TERP)));
	}
	elseif STATUSTYPE and display.statusline_height = 2
	{
		if not (TERP & SIMPLE_TERP)
			locate 1, ++row
		else
			""
		print "\_";
	}
	PrintStatusType
}

#endif  ! _NEWSTATUSLINE_H