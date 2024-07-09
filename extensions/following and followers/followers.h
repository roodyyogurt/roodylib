!----------------------------------------------------------------------------
!* FOLLOW
!----------------------------------------------------------------------------
! This is code from Kent Tessman's "Future Boy!" adapted for general use.

! If your game happens to have more than 8 possible followers at a given time,
! define MAX_FOLLOWERS as something larger before roodylib is included
#ifclear _FOLLOWERS_H
#set _FOLLOWERS_H

#if undefined MAX_FOLLOWERS
constant MAX_FOLLOWERS 8
#endif

array follower_list[MAX_FOLLOWERS]
array moved_follower[MAX_FOLLOWERS]
attribute following
global followers
global following_paused
global silent_follow

property follow_message		! for locations, a suffix such as
									! " you down the stairs."

property disallow_follow 	! return true if circumstances disallow
									! following (for characters or locations)

property pause_following	! for NPCs

routine AddFollower(char)
{
	if followers = MAX_FOLLOWERS or char is following
		return false
	follower_list[followers++] = char
	char is following
	return true
}

routine RemoveFollower(char)
{
	local i
	for (i=0; i<MAX_FOLLOWERS; i++)
	{
		if follower_list[i] = char
		{
			while (i < MAX_FOLLOWERS-2)
			{
				follower_list[i] = follower_list[i+1]
				i++
			}
			follower_list[MAX_FOLLOWERS-1] = 0
			followers--
			char is not following
			return
		}
	}
}

!\
	StartFollowing and StopFollowing are just two "helper" routines so authors
	don't need to remember that the following mechanism is handled by a
	daemon (and therefore needs to be activated or deactivated)
\!

routine StartFollowing
{
	return Activate(follow_daemon)
}

routine StopFollowing
{
	return Deactivate(follow_daemon)
}

daemon follow_daemon
{}

event in follow_daemon
{
	! Had to add DoCrawl for >CRAWL beneath the trees
!	if (verbroutine = &DoGo, &DoExit, &DoCrawl) and location ~= old_location
! ^ Future Boy! has a DoCrawl verb
	if (verbroutine = &DoGo, &DoExit, &DoEnter) and location ~= old_location
		MoveFollowers
}

#if undefined Grandparent
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
#endif

routine MoveFollowers
{
	local i, mf

	if followers = 0
		return

	if location.disallow_follow
		return

	for (i=0; i<followers; i++)
	{
		if Grandparent(follower_list[i]) = old_location and
			Grandparent(follower_list[i]) ~= location and
			not follower_list[i].pause_following and
			not follower_list[i].disallow_follow
		{
			move follower_list[i] to location
			moved_follower[mf] = follower_list[i]
			mf++
		}
	}

	if (mf) and not silent_follow
	{
		print ""
		print capital PrintArrayList(moved_follower, mf);
		" follow";
		if mf = 1 and moved_follower[0] is not plural:  "s";
		if not location.follow_message
		{
			print " ";
			if player_person = 1,2
				print player.pronoun #2;
			else
				print player.name;
			"."
		}
		event_flag = true
	}
	silent_follow = 0
}


#if undefined PrintArrayList
! taken from misc.h ... called from MoveFollowers:

! PrintArrayList(list, count)
! Prints count elements of the array list.

routine PrintArrayList(list, count)
{
	local i
	for (i=1; i<=count; i++)
	{
		if i > 1:  " ";
		The(array list[i-1])
		if i < count and count > 2:  ",";
		if i = count-1:  print " "; AND_WORD;
	}
	return false
}
#endif ! if undefined PrintArrayList

#endif ! _FOLLOWERS_H