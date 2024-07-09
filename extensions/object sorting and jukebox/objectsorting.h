#ifclear _OBJECTSORTING_H
#set _OBJECTSORTING_H

!\ MovePosition - moves object "obj" in parent "par" to position "pos"
( pos = 1 makes it the oldest child, bigger numbers make it younger) \!
routine MovePosition(obj,par,pos)
{
	local n = 1
	while child(par)
	{
		move child(par) to display
	}

	while child(display)
	{
		if n++ = pos
			move obj to par
		else
		{
			if child(display) = obj
				move obj to display
			move child(display) to par
		}
	}
}

! MixObjects - Randomizes order of children of parent "par"
routine MixObjects(par)
{
	local n, obj

	while child(par)
	{
		move child(par) to display
	}

	while child(display)
	{
		n = children(display)
		n = random(n)
		n--
		obj = child(display)
		while n
		{
			obj = sibling(obj)
			n--
		}
		move obj to par
	}
}

! MoveFromTo - with object-based designs, I sometimes need to move all
! children from one object to another. Here is a routine for convenience.
routine MoveFromTo(first,second)
{
	while child(first)
	{
		move child(first) to second
	}
}

! ReverseOrder - Reverses the order of parent's (par's) children
routine ReverseOrder(par)
{
	while child(par)
	{
		move child(par) to display
	}

	while child(display)
	{
		move youngest(display) to par
	}
}

!\ SortChildren- Sorts children of parent "par" by numerical criteria "crit".
If reverse argument is true, puts them in reverse order (lowest to highest).
See BySize example after this for example criteria routine (which would be
called like SortChildren(jellybean_jar, &BySize)
\!

routine SortChildren(par, crit, reverse)
{
	local x, p, next
	local best_choice = -32767
	while child(par)
	{
		for x in par
		{
			p = call array crit(x)
			if p > best_choice
				best_choice = p
		}
		x = child(par)
		while x
		{
			next = younger(x)
			if best_choice = call array crit(x)
				move x to display
			x = next
		}
		best_choice = -32767
	}

	while child(display)
	{
		if reverse
			move youngest(display) to par
		else
			move child(display) to par
	}
}

!\
Example criteria routine

routine BySize(obj)
{
	return obj.size
}
\!

! TakeRandom - returns a random child from parent "par"
routine TakeRandom(par)
{
	local n, obj
	n = children(par)
	n = random(n)
	n--
	obj = child(par)
	while n
	{
		obj = sibling(obj)
		n--
	}
	return obj
}

!\ "floats" objects in parent "par" to the top as determined by rules
   returned by routine "arg" (see sample below)  \!
routine Float(par,arg)
{
	local obj, qualify
	while child(par)
	{
		move child(par) to display
	}

	obj = youngest(display)

	while elder(obj)
	{
		qualify = call arg(obj)
		if qualify
		{
			move obj to par
			obj = youngest(display)
		}
		else
			obj = elder(obj)
	}

	while child(display)
	{
		move child(display) to par
	}
}

!\  Sample routine call and argument routine:
routine DoFloat
{
	Float(basket, &IsEdible)
}

routine IsEdible(obj)
{
	return (obj is edible)
}  \!

#endif !  _OBJECTSORTING_H