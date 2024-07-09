!\ DoScope and DoScopeRooms - Here are two debugging routines for reminding you
what objects are in scope of various rooms. "SCOPE" will just list the items
in scope of the current location, while "SCOPE ROOMS" will list all objects
in scope in every room.

IMPORTANT NOTE: All rooms and objects must be defined *after* the
player character object.    \!

routine DoScope
{
	print "Total objects: "; number objects
	print "Highest object number: "; number (objects - 1)
	"Objects in scope:"
	local a
	for (a=player ; a< objects ; a=a+1)
	{
		if a ~= location
		{
			if FindObject(a,location)
			{
				print "\_  ";
				print a.name
			}
		}
	}
}

#ifclear NO_OBJLIB
routine DoScopeRooms
{
	local a, b, c
	print "Total objects: "; number objects
	print "Highest object number: "; number (objects - 1)
	a = player
	do
	{
		if a.type = room
		{
			c = 0
			print ""
			print "Room: "; a.name
			print "In scope:"
			for (b=player ; b< objects ; b=b+1)
			{
				if b.type ~= room and b~= player
				{
					if FindObject(b,a)
					{
						print "\_  "; b.name
						c++
					}
				}
				elseif b = player and a = location
				{
					print "\_  "; b.name
					c++
				}
			}
			if not c
				print "\_  "; "nothing"
		}
		a++
	}
	while a< objects
}
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
	before
	{
#ifset HUGOFIX
		actor DoVerbTest
		{
			DoVerbTest
			return true
		}
#endif
	}
	capacity 100
	holding 0
	is hidden, living, transparent, static
	is plural       ! because player_person defaults to 2
}

!\  DoVerbTest is another debugging utility. Type >VERBTEST <OBJECT>
    to see what the responses are to a host of verbs. Hopefully, it'll remind
	 you of important responses to be yet written, like if you have a food
	 object but forgot to code a fitting DoEat response.

	 NOTE: It might act a little wonky with doors and such, as it tests
	 DoGo and DoEnter, among other things.
\!
routine DoVerbTest
{
	local a, b, c, d, t
	a = parent(object)

	while b < 26
	{
		b++
		t = 0
		Font(ITALIC_ON)
		select b
			case 1
			{
				if player ~= parent(object)
				{
					t = "Getting object"
					c = &DoGet
				}
			}
			case 2
			{
				t = "Wearing object"
				c = &DoWear
			}
			case 3
			{
				if parent(object) = player
				{
					t = "Removing object"
					c = &DoTakeOff
				}
			}
			case 4
			{
				if parent(object) = player
				{
					t = "Dropping object"
					c = &DoDrop
				}
			}
			case 5
			{
				if object is openable
				{
					t = "Opening object"
					c = &DoOpen
				}
			}
			case 6
			{
				if object is openable
				{
					t = "Closing object"
					c = &DoClose
				}
			}
			case 7
			{
				if object is lockable
				{
					t = "Locking object"
					c = &DoLock
				}
			}
			case 8
			{
				if object is lockable
				{
					t = "Unlocking object"
					c = &DoUnlock
				}
			}
			case 9
			{
				if object is switchable
				{
					t = "Switching on object"
					c = &DoSwitchOn
				}
			}
			case 10
			{
				if object is switchable
				{
					t = "Switching off object"
					c = &DoSwitchOff
				}
			}
			case 11
			{
				t = "Listening to object"
				c = &DoListen
			}
			case 12
			{
				t = "Eating object"
				c = &DoEat
			}
			case 13
			{
				t = "Drinking object"
				c = &DoDrink
			}
			case 14
			{
				t = "Hitting object"
				c = &DoHit
			}
			case 15
			{
				t = "Hello-ing object"
				c = &DoHello
			}
			case 16
			{
				t = "Examining object"
				c = &DoLook
			}
			case 17
			{
				if object is platform or object is container
				{
					t = "Looking in/on object"
					c = &DoLookIn
				}
			}
			case 18
			{
				t = "Looking through object"
				c = &DoLookThrough
			}
			case 19
			{
				t = "Looking under object"
				c = &DoLookUnder
				word[2] = "under"
			}
			case 20
			{
				t = "Go-ing object"
				c = &DoGo
			}
			case 21
			{
				t = "Entering object"
				c = &DoEnter
			}
			case 22
			{
				t = "Sitting on object"
				c = &DoSit
			}
			case 23
			{
				t = "Exiting object"
				c = &DoExit
			}
			case 24
			{
				t = "Moving object"
				c = &DoMove
			}
			case 25
			{
				t = "Searching object"
				c = &DoSearch
			}
!			case 26
!			{
!				t = "Smelling object"
!				c = &DoSmell
!			}
		Font(ITALIC_OFF)
		if c
		{
			if d
			{
				""
			}
			print t
			Perform(c, object)
			d = c
		}
		c = 0
		if parent(object) ~= a, player
			move object to location
	}
!	c = 0
#ifset _VERBSTUB_H
	""
	b = 0
	d = 0
	Font(BOLD_ON)
	"VERBSTUB verbs"
	Font(BOLD_OFF)
	""
	while b < 14
	{
		b++
		t = 0
		Font(ITALIC_ON)
		select b
			case 1
			{
				t = "Getting object again:"
				c = &DoGet
			}
			case 2
			{
				t = "Waving to object"
				c = &DoWaveHands
			}
			case 3
			{
				if parent(object) = player
				{
					t = "Waving object"
					c = &DoWave
				}
			}
			case 4
			{
				if parent(object) = player
				{
					t = "Throwing object"
					c = &DoThrowAt
				}
			}
			case 5
			{
				t = "Climbing object"
				c = &DoClimb
			}
			case 6
			{
				t = "Pushing object"
				c = &DoPush
			}
			case 7
			{
				t = "Pulling object"
				c = &DoPull
			}
			case 8
			{
				t = "Kissing object"
				c = &DoKiss
			}
			case 9
			{
				t = "Touching object"
				c = &DoTouch
			}
			case 10
			{
				t = "Tying object"
				c = &DoTie
			}
			case 11
			{
				t = "Untying object"
				c = &DoUntie
			}
			case 12
			{
				t = "Burning object"
				c = &DoBurn
			}
			case 13
			{
				t = "Cutting object"
				c = &DoCut
			}
			case 14
			{
				t = "Digging object"
				c = &DoDig
			}
		Font(ITALIC_OFF)
		if c
		{
			if d
				""
			print t
			Perform(c, object)
			d = c
		}
		c = 0
		if parent(object) ~= a, player
			move object to location
	}
#endif  !ifset USE_VERBSTUB
}