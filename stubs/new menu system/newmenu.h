!----------------------------------------------------------------------------
!* NEW MENU
!----------------------------------------------------------------------------

#ifclear _NEWMENU_H
#set _NEWMENU_H
object menulib "menu"
{
	type settings
	in init_instructions
	execute
	{
		local i

		if word[LAST_TURN] ~= "undo","restore"
		{
			for (i=(menu_category + 1);i<=objects ;i++ )
			{
				if i.type = menu_category and parent(i) = nothing and
				i ~= menu_category
					move i to menu_pages
			}
		}
	}
}
attribute option_toggle alias special 

! menu_category properties
property title_gap alias initial_desc
property options_gap alias reach
property page_text_color alias holding
property page_bg_color alias exclude_from_all
property title_color alias list_contents
property title_bg  alias capacity
property menu_link alias door_to
property toggle_code alias door_to

! option/hint_option properties

property menu_text alias long_desc
property hint1 alias n_to
property hint2 alias ne_to
property hint3 alias e_to
property hint4 alias se_to
property hint5 alias s_to
property hint6 alias sw_to
property hint7 alias w_to
property hint8 alias nw_to
property hint9 alias u_to
property hint10 alias d_to
property alt_title alias misc
property alt_name alias misc ! just because I think I'm going to forget the
                             ! name of the property from time to time

property option_available alias in_to
property hints_available alias in_to
property hints_revealed alias out_to

object menu_pages
{}

class menu_category
{
	type menu_category
	title_gap  0 ! lines between menu title and "[N]ext key"
	options_gap 1 ! lines between "[N]ext key" and menu options
	option_available true
	page_bg_color 0
	page_text_color 0
	title_color 0
	title_bg 0
}

class option
{
	type option
	option_available true
}

option hint_option "(hint_option)"
{
	type hint_option
	menu_text
		Help_Hints(self)
	hints_available 1
	hints_revealed 0
}

routine MakeMenu(menu_title, width, recurse)
{
	local category, old_category, testlink
#ifset NO_ACCESSIBILITY
	local cheap
#endif

	if not menu_title
		menu_title = child(menu_pages) ! main_menu

	if not recurse
	{
		if verbroutine = &EndGame
			""
		if not (system(61) or cheap)
			color BGCOLOR, BGCOLOR
		MenuMessage(&MakeMenu,1,menu_title) ! "Opening menu..."
		""
		color TEXTCOLOR, BGCOLOR, INPUTCOLOR
	}

   while true
	{
		if not (cheap or system(61))
			SetPageColors(menu_title)

		if not (cheap or (TERP & SIMPLE_TERP))
		{
			window 0
			color MENU_TEXTCOLOR, MENU_BGCOLOR
			cls
		}
			category = Menu(menu_title, width, old_category)
			old_category = category
			if category is option_toggle
				run category.toggle_code
			else
			{
				testlink = category.menu_link
				if testlink
					category = testlink
			}
		if category and category is not option_toggle
		{
			if category.type = menu_category
				MakeMenu(category,width,(recurse + 1))
			else
			{
				if not (cheap or system(61))
					color MENU_BGCOLOR, MENU_BGCOLOR
				if not cheap
					MenuMessage(&MakeMenu,2,category.name)
				if not (cheap or system(61))
				{
					SetPageColors(category)
					color MENU_TEXTCOLOR, MENU_BGCOLOR
				}
				do
				{
					if display.needs_repaint and not cheap and
						is_fluid_layout is not special
					{
						color MENU_BGCOLOR, MENU_BGCOLOR
						MenuMessage(&MakeMenu,3) ! "[WINDOW RESIZED]"
						""
						MenuMessage(&MakeMenu,2,category.name)
						color TEXTCOLOR, BGCOLOR, INPUTCOLOR
					}
					display.needs_repaint = false
					color TEXTCOLOR, BGCOLOR, INPUTCOLOR
					if not (cheap or (TERP & SIMPLE_TERP))! simple_port
					{
						if category.alt_title
							CenterTitle(category.alt_title)
						else
							CenterTitle(category.name)
					}
					else
					{
						Indent
						print "\_  ";
						Font(BOLD_ON)
						if category.alt_title
							print category.alt_title
						else
							print category.name
						Font(BOLD_OFF)
						""
					}
					if not (cheap or system(61))
						locate 1,LinesFromTop
					run category.menu_text
					print newline
					""
					if category.type ~= hint_option
						CoolPause(0,category)
				}
				while (display.needs_repaint = true  )
			}
		}
		elseif category is not option_toggle
		{
			if not recurse
			{
				if not (cheap or (TERP & SIMPLE_TERP))
					InitScreen
				if not verbroutine ~= &EndGame
					AfterMenu
				else
					PrintStatusLine
			}
			return
		}
	}
}

routine AfterMenu
{
#ifclear NO_ACCESSIBILITY
	if not (cheap & CHEAP_ON)
#endif
		PrintStatusline
	Font(DEFAULT_FONT) ! just in case
	MenuMessage(&AfterMenu,1) ! "...and now returning to the story."
	if (FORMAT & DESCFORM_I)
		""
	DescribePlace(location, true)
}

replace Menu(menu_par, width, selection)
{
	local i, column, mouseclick, titlegap, optionsgap, num
	local n
	local sel = 1

#ifset NO_ACCESSIBILITY
	local cheap
#endif

	titlegap = menu_par.title_gap
	optionsgap = menu_par.options_gap

	if system(61)
	{
		titlegap = 0
		optionsgap = 1
	}
	if width = 0:  width = 20
	for i in menu_par
	{
		if i.option_available
		{
			num++
			n = string(_temp_string,i.name)
			if n > width : width = n
		}
	}

	if width > (display.linelength-1):  width = display.linelength-1

	! Default selection is first child if not otherwise given
	if selection = 0:  selection = child(menu_par)

	while not selection.option_available
	{
		selection = younger(selection)
	}

	Font(BOLD_OFF | ITALIC_OFF | UNDERLINE_OFF | PROP_OFF)
	while true
	{
		if not (cheap or system(61)) ! system(61)
		{
			color MENU_BGCOLOR, MENU_BGCOLOR
			if display.needs_repaint and is_fluid_layout is not special
			{
				window 0
				display.needs_repaint = false
			}
			cls
		}
		if cheap or (TERP & SIMPLE_TERP) or
		( (TERP & GLK_TERP) and
		( num + 5 + titlegap + optionsgap ) >= (display.screenheight/3*2))
			return CheapMenu(menu_par,num)
		column = display.linelength/2 - width/2
		window ( num + 5 + titlegap + optionsgap )
		{
			if (TERP & GLK_TERP) ! system(61) and not simple_port
				cls

			! This section "fakes" CenterTitle, so it appears to be
			! a regular title window, when in actuality, the whole menu
			! is in the status window
			if not (TERP & SIMPLE_TERP)
				locate 1,1
			Font(PROP_OFF)
			n = string(_temp_string, menu_par.name)
			color MENU_SELECTCOLOR, MENU_SELECTBGCOLOR ! shouldn't affect glk
			print to (display.linelength/2 - n/2);
			print menu_par.name;
			print to display.linelength ! make sure we color the line completely
			for (i=0; i<titlegap;i++)
			{
				""
			}
			color MENU_TEXTCOLOR, MENU_BGCOLOR
			! end of fake CenterTitle
			Font(BOLD_OFF|ITALIC_OFF|UNDERLINE_OFF|PROP_OFF) ! shouldn't affect
																			 ! glk
			MenuMessage(&Menu, 1)	! print key commands

			for (i=0; i<optionsgap;i++)
			{
				""
			}

			sel = 1
			i = child(menu_par)
			while i
			{
				if i.option_available
				{
					if i = selection
					{
						if system(61) ! glk or minimum port
							print to (column - 2);"* ";
						else
							locate (column + 1), display.cursor_row
						color MENU_SELECTCOLOR, MENU_SELECTBGCOLOR  ! shouldn't affect
																				  ! glk?
						print i.name; to (column+width)
						color MENU_TEXTCOLOR, MENU_BGCOLOR
					}
					else
					{
						locate (column + 1), display.cursor_row !print to column;
						print i.name ; to (column+width)
					}
					sel++
				}
				i = younger(i)
			}
		}
		word[0] = HiddenPause
		if word[0] = MOUSE_CLICK and display.pointer_x >= column and
					display.pointer_x <= column+width
		{
			mouseclick = (num + 1) + display.pointer_y
			i = PickNthChild(mouseclick, menu_par, &IsOptionAvailable )
			if i.name
			{
					selection = i
					word[0] = ENTER_KEY
			}
			mouseclick = 0
		}
		elseif word[0] = MOUSE_CLICK
		{
			if (display.pointer_y + num + 5 + titlegap + optionsgap ) = 3 + titlegap
			{
				if display.pointer_x >= (display.screenwidth - 10)
					word[0] = ESCAPE_KEY
				elseif display.pointer_x <= 11
					word[0] = 'n'
			}
			elseif (display.pointer_y + num + 5 + titlegap + optionsgap ) = 4 + titlegap
			{
				if display.pointer_x >= (display.screenwidth - 16)
					word[0] = ENTER_KEY
				elseif display.pointer_x <= 15
					word[0] = 'p'
			}
		}
		if word[0] >= '0' and word[0] <= '9'
		{
			i = word[0] - '0'
			if i = 0:  i = 10
			if i <= num
			{
				i = PickNthChild(i, menu_par, &IsOptionAvailable )
				if i.name
					selection = i
			}
		}
		select word[0]
			case 'N', 'n', DOWN_ARROW, RIGHT_ARROW
			{
				while true
				{
					if not younger(selection)
						selection = child(menu_par)
					else
						selection = younger(selection)
					if selection.option_available and selection.name ~= ""
						break
				}
			}
			case 'P', 'p', UP_ARROW, LEFT_ARROW
			{
				while true
				{
					if not elder(selection)
						selection = youngest(menu_par)
					else
						selection = elder(selection)
					if selection.option_available and selection.name ~= ""
						break
				}
			}
			case 'Q', 'q', ESCAPE_KEY
			{
				if not (cheap or system(61))! cos ! system(61)
				{
					window !0
					{cls}
				}
				return 0
			}
			case ENTER_KEY
			{
				if not (cheap or system(61))! cos ! system(61)
				{
					window !0
					{cls}
				}
				return selection
			}
	}
!	}
}
#if undefined PickNthChild
routine PickNthChild(num, par, qualify_routine)
{
	local x, n, qual
	x = child(par)
	while x
	{
		if qualify_routine
		{
			qual = call qualify_routine(x)
			if qual
				n++
		}
		else
			n++
		if n = num
			return x
		x = younger(x)
	}
	return false
}
#endif

routine IsOptionAvailable(obj)
{
	if obj.option_available
		return true
	return false
}

routine CheapMenu(menu_par,num)
{
	Font(PROP_ON)
	local sel,i
#ifset NO_ACCESSIBILITY
	local cheap
#endif
	while true
	{
		if not (cheap or (TERP & SIMPLE_TERP))
			cls
		if not (TERP & SIMPLE_TERP)
			CenterTitle(menu_par.name)
		if display.needs_repaint
			display.needs_repaint = false
		print newline
		Indent
		print "\_  ";
		Font(BOLD_ON)
		print menu_par.name
		Font(BOLD_OFF)
		""
		sel = 1
		i = child(menu_par)
		while i
		{
			if i.option_available
			{
				Indent
				print number sel++;". ";
				print i.name
			}
			i = younger(i)
		}
		print ""
		MenuMessage(&Menu, 2)		! "Select the number of your choice"
		if num > 9
		{
			GetInput
			if word[1] = "q","quit"
			{
				""
				return 0
			}
			if word[1]
				sel = StringToNumber(word[1])
			else
				sel = StringToNumber(parse$)
		}
		else
		{
			pause
			if word[0] = 'q','Q', '0', ESCAPE_KEY
			{
				printchar word[0]
				print newline
				if not (cheap or (TERP & SIMPLE_TERP))
					cls
				""
				return 0
			}
			else
				sel = word[0] - 48
		}
		if num <= 9
		{
			printchar word[0]
			print newline
		}
		""
		if sel > 0 and (sel <= num) ! sel)
		{
			if not (cheap or (TERP & SIMPLE_TERP))
				cls
			return PickNthChild(sel, menu_par, &IsOptionAvailable)
		}
		else
			"Not a valid option.\n"
	}
}

routine Help_Hints(obj)
{
	local i
#ifset NO_ACCESSIBILITY
	local cheap
#endif
	for (i = 0; i<=obj.hints_revealed; i++)
	{
		if i
			""
		run obj.(hint1+i)
		print newline
	}
	""
	while &obj.(hint1+i) ~= 0 ! i.e., no more topics
	{
		Font(BOLD_ON)
		MenuMessage(&Help_Hints,1) ! "[Press 'H' for another hint, or 'Q' to
										!	quit]";
		Font(BOLD_OFF)
		print newline
		word[0] = HiddenPause
		if word[0] = 'Q', 'q', ESCAPE_KEY
			return
		if word[0] = 'H', 'h'
		{
			obj.hints_revealed++
			run obj.(hint1+i++)
			print newline
			""
		}
	}
	Font(BOLD_ON)
	if not cheap
	{
		MenuMessage(&Help_Hints,2) ! "[No more hints.  Press any
	                           !  key...]";
		Font(BOLD_OFF)
		HiddenPause
	}
	else
	{
		MenuMessage(&Help_Hints,3) ! "[No more hints.  Leaving page.]";
		Font(BOLD_OFF)
	}

	return
}

routine ShowPage(page)
{
#ifset NO_ACCESSIBILITY
	local cheap
#endif
 	local reset_indent
	if not (cheap or system(61))
		window 0
	if verbroutine = &EndGame
		""
	if not (cheap or (TERP & SIMPLE_TERP))
	{
		color BGCOLOR, BGCOLOR
		MenuMessage(&ShowPage,1,page) ! "[OPENING PAGE]"
		""
		cls
	}
	do
	{
		if display.needs_repaint and not cheap and
			is_fluid_layout is not special
		{
			color BGCOLOR, BGCOLOR
			MenuMessage(&MakeMenu,3) ! "[WINDOW RESIZED]"
			""
		}
		display.needs_repaint = false
		color TEXTCOLOR, BGCOLOR, INPUTCOLOR
		if not (cheap or (TERP & SIMPLE_TERP))
		{
			if page.alt_title
				CenterTitle(page.alt_title)
			else
				CenterTitle(page.name)
		}
		if not cheap and not system (61)
			locate 1,LinesFromTop
		elseif cheap
		{
			if not (FORMAT & NOINDENT_F)
			{
				reset_indent = true
				FORMAT = FORMAT | NOINDENT_F
			}
		}
		run page.menu_text
		if reset_indent
			FORMAT = FORMAT & ~NOINDENT_F
		print newline
		if not cheap and
			not (page.type = hint_option and verbroutine = &EndGame)
			""
		if page.type ~= hint_option and not (cheap or (TERP & SIMPLE_TERP))
			CoolPause(0,page,(verbroutine = &EndGame))
	}
	while (display.needs_repaint = true  )

	if not (cheap or (TERP & SIMPLE_TERP))
	{
		InitScreen
		if verbroutine ~= &EndGame
			AfterMenu
	}
	else
		PrintStatusLine
}

routine SetPageColors(page)
{
	if not (menu_category.page_text_color or menu_category.page_bg_color)
	{
		if not (MENU_TEXTCOLOR or MENU_BGCOLOR) ! must not have been set
		{
			MENU_TEXTCOLOR = TEXTCOLOR
			MENU_BGCOLOR = BGCOLOR
		}
		menu_category.page_text_color = MENU_TEXTCOLOR
		menu_category.page_bg_color = MENU_BGCOLOR
	}

	if not (menu_category.title_color or menu_category.title_bg)
	{
		if not (MENU_SELECTCOLOR or MENU_SELECTBGCOLOR)
		{
			MENU_SELECTCOLOR = SL_TEXTCOLOR
			MENU_SELECTBGCOLOR = SL_BGCOLOR

		}
		menu_category.title_color = MENU_SELECTCOLOR
		menu_category.title_bg = MENU_SELECTBGCOLOR
	}

	if page.page_text_color or page.page_bg_color
	{
		MENU_TEXTCOLOR = page.page_text_color
		MENU_BGCOLOR = page.page_bg_color
	}
	else
	{
		MENU_TEXTCOLOR = menu_category.page_text_color
		MENU_BGCOLOR = menu_category.page_bg_color
	}
	if page.title_color or page.title_bg
	{
		MENU_SELECTCOLOR = page.title_color
		MENU_SELECTBGCOLOR = page.title_bg
	}
	else
	{
		MENU_SELECTCOLOR = menu_category.title_color
		MENU_SELECTBGCOLOR = menu_category.title_bg
	}
}

routine MenuMessage(r, num, a, b)
{
	if NewMenuMessages(r, num, a, b):  return
	select r
		case &MakeMenu
		{
			select num
				case 1 : "Opening menu..."
				case 2
				{
					if (TERP & SIMPLE_TERP)
					{
						local l,i
						l = string(_temp_string, a)
						print to (40 - l/2-1);
						print "+";
						for (i=1;i<=l ;i++ )
						{
							print "-";
						}
						print "+"
						print to (40 - l/2-1);
						print "|";
						print a;
						print "|"
						print to (40 - l/2 - 1);
						print "+";
						for (i=1;i<=l ;i++ )
						{
							print "-";
						}
						print "+"
					}
					else
					{
						Indent
						print "\_ "; a
						""
					}
				}
				case 3 : "Window resized. Redrawing screen."
		}
		case &Menu
		{
			select num
				case 1
				{
					if system(61)
						locate 2, display.cursor_row
					else
						locate 1, display.cursor_row
					print "[N]ext item"; to (display.linelength - 11); \
		    				"[Q]uit menu"
					if system(61)
						locate 2, display.cursor_row
					print "[P]revious item"; to (display.linelength - 17);
					print "[Enter] to select"
				}
				case 2
				{
					! The CheapGlk version now works off numbers to make
					! navigation as easy as possible in simple terps
					print "Select the number of your choice or \"Q\" to exit: ";
				}
		}
		case &Help_Hints
		{
			select num
				case 1
					"[Press 'H' for another hint, or 'Q' to quit]"; !;
				case 2
					print "[No more hints.  Press any key...]";
				case 3
					print "[No more hints.  Leaving page.]";
		}
		case &ShowPage
		{
			select num
				case 1
				{
!					"[OPENING PAGE]"
					if a.alt_name
						print "(Opening \""; a.alt_name;"\")"
					else
						print "(Opening \""; a.name;"\")"
				}
				case 2
					"[CLOSING PAGE]"
		}
		case &AfterMenu
		{
			select num
				case 1: print "\I...and now returning to the story.\i"
		}
		case else : return false
	return true ! this line is only reached if we replaced something
}

routine NewMenuMessages(r, num, a, b)
{
	select r
		case else : return false
	return true ! this line is only reached if we replaced something
}
#ifclear NO_ACCESSIBILITY
replace cheaplib "cheap"
{
	type settings
	in init_instructions
	save_info
	{
		local a
		select cheap
			case 0 : a = SaveWordsetting("off")
			case 1 : a = SaveWordSetting("on")
			case 2 : a = SaveWordSetting("menus")
		word[(a-1)] = prompt
		return true
	}
	execute
	{
		local a,b
		b = cheap
		a = CheckWordSetting("cheap")
		if a
		{
			if word[(a+1)]
				prompt = word[(a + 1)]
			select word[(a+2)]
				case "off" : cheap = 0
				case "on" : cheap = 1
				case "menus" : cheap = 2
		}
		elseif TERP = GLK_TERP
			cheap = 2
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
	!	"The following commands automatically display menus as numerated lists:"
		Indent
		"\BSIMPLE MENUS\b - Displays in-game menus as numerical lists in the
		main window. All other behavior (screen clearing, status lines) are the
		same."
		Indent
		"\BCLEARSCREEN OFF\b - Any in-game screen-clearing is ignored. This
		might work with some screen readers and also targets console interpreters
		where screen clearing doesn't work well anyway. Menu options are
		numbered."
		Indent
		"\BNORMAL\b - Reverts to allow in-game screen sclearing."
	}
}

replace DoAccessibilityHelp
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
	"\BSIMPLE MENUS\b - Displays in-game menus as numerical lists in the
	main window. All other behavior (screen clearing, status lines) are the
	same."
!	"The following commands automatically display menus as numerated lists:"
	Indent
	"\BCLEARSCREEN OFF\b - Any in-game screen-clearing is ignored. This
	might work with some screen readers and also targets console interpreters
	where screen clearing doesn't work well anyway. Menu options are numbered."

	"\nTo revert back to regular screen-clearing and menus, type \BNORMAL\b."
}

replace DoAccessibility
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
				print " and menus";
				" restored."
				if (b & CHEAP_ON)
					PrintStatusLine
			}
			else
			{
				print "Normal screen-clearing";
				print " and menus";
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
				print " and menus";
				" restored."
				if (b & CHEAP_ON)
							PrintStatusLine
					}
					else
					{
				print "Normal screen-clearing";
				print " and menus";
				" already set."
					}
				}
				case "never","off"
				{
					if not (cheap & CHEAP_ON)
					{
						cheap = true
				print "Screen-clearing disabled";
				print " and simple menus enabled";
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

replace DoCheapOnOff
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
				print " and menus";
				" restored."
					if (b & CHEAP_ON)
						PrintStatusLine
				}
				else
				{
					print "Normal screen-clearing";
					print " and menus";
					" already set."
				}
			}
			case "on"
			{
				if not (cheap & CHEAP_ON)
				{
					cheap = true
				print "Screen-clearing disabled";
				print " and simple menus enabled";
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
#endif ! NO_ACCESSIBILITY
#endif ! _NEWMENU_H
