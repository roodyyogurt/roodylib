! Roody's note: Most of this code is from Kent Tessman's Future Boy! with
! just a couple of additions of my own. Now, Hugo doesn't do real-time games
! (like Infocom's Border Zone), but it can catch the current time and the
! following code allows for figuring out the time difference betwen this
! or that time (useful for music jukebox extensions or other misc uses).
!
! Since it's rare for games to use this, include this file
! to enable it.
!----------------------------------------------------------------------------
#ifclear _TIMESYSTEM_H

#ifclear _SYSTEM_H
#include "system.h" ! these routines use this extension
#endif

! we alias these time object properties just because we can
property tm_year alias u_to
property tm_month alias d_to
property tm_day alias s_to
property tm_hour alias n_to
property tm_minute alias e_to
property tm_second alias w_to

! Roody's note : An object class to hold times.
class time_object
{
	tm_year 0
	tm_month 0
	tm_day 0
	tm_hour 0
	tm_minute 0
	tm_second 0
}

#ifset _RESOURCE_H
time_object song_start
{}

time_object current_time
{}

time_object time_difference
{}
#endif

! Roody's note: Sets the current time to a time_object provided as an argument.
routine GetCurrentTime(current)
{
	current.tm_year   = GetSystemTimeValue(TIME_YEAR)
	if current.tm_year = 0
		return false

	current.tm_month  = GetSystemTimeValue(TIME_MONTH)
	current.tm_day    = GetSystemTimeValue(TIME_DAY)
	current.tm_hour   = GetSystemTimeValue(TIME_HOURS)
	current.tm_minute = GetSystemTimeValue(TIME_MINUTES)
	current.tm_second = GetSystemTimeValue(TIME_SECONDS)

	return true
}

! Roody's note: Checks if the year provided is a leap year.
routine IsLeapYear(year)
{
	! Years not divisible by 4 are not leap years
	if mod(year, 4)
		return false

	! Years divisible by 400 are leap years
	if mod(year, 400) = 0
		return true

	! Years divisible by 100 are not leap years
	if mod(year, 100) = 0
		return false

	! If we get here, it's a leap year
	return true
}

! Roody's note: Determines difference between "current" and "previous"
! time_objects, saving to "result" time_object.
routine CalculateTimeDifference(current, previous, result)
{
	local years, months, days, hours, minutes, seconds

	years = current.tm_year - previous.tm_year
	if years < 0
	{
		! current must be later than previous
		return false
	}

	months = current.tm_month - previous.tm_month
	if months < 0
	{
		months = months + 12
		years--
	}

	days = current.tm_day - previous.tm_day
	if days < 0
	{
		local d
		if previous.tm_month = 4, 6, 9, 11
			d = 30
		elseif previous.tm_month = 2
		{
			if IsLeapYear(previous.tm_year)
				d = 29
			else
				d = 28
		}
		else
			d = 31

		days = days + d
		months--
	}

	hours = current.tm_hour - previous.tm_hour
	if hours < 0
	{
		hours = hours + 24
		days--
	}

	minutes = current.tm_minute - previous.tm_minute
	if minutes < 0
	{
		minutes = minutes + 60
		hours--
	}

	seconds = current.tm_second - previous.tm_second
	if seconds < 0
	{
		seconds = seconds + 60
		minutes--
	}

	result.tm_year = years
	result.tm_month = months
	result.tm_day = days
	result.tm_hour = hours
	result.tm_minute = minutes
	result.tm_second = seconds

	return true
}

! Roody's note: Adds time_object "time1" to time_object "time2", saving it to
! time_object "result"
routine AddTimes(time1, time2, result)
{
	local d
	if time1.tm_month = 4, 6, 9, 11
		d = 30
	elseif time1.tm_month = 2
	{
		if IsLeapYear(time1.tm_year)
			d = 29
		else
			d = 28
	}
	else
		d = 31

	result.tm_year = 0
	result.tm_month = 0
	result.tm_day = 0
	result.tm_hour = 0
	result.tm_minute = 0
!	result.tm_second = 0

	result.tm_second = time1.tm_second + time2.tm_second
	if result.tm_second >= 60
	{
		result.tm_second = result.tm_second - 60
		result.tm_minute++
	}

	result.tm_minute = result.tm_minute + time1.tm_minute + time2.tm_minute
	if result.tm_minute >= 60
	{
		result.tm_minute = result.tm_minute - 60
		result.tm_hour++
	}

	result.tm_hour = result.tm_hour + time1.tm_hour + time2.tm_hour
	if result.tm_hour >= 24
	{
		result.tm_hour = result.tm_hour - 24
		result.tm_day++
	}

	result.tm_day = result.tm_day + time1.tm_day + time2.tm_day
	if result.tm_day >= d
	{
		result.tm_day = result.tm_day - d
		result.tm_month++
	}

	result.tm_month = result.tm_month + time1.tm_month + time2.tm_month
	if result.tm_month >= 12
	{
		result.tm_month = result.tm_month - 12
		result.tm_year++
	}

	result.tm_year = result.tm_year + time1.tm_year + time2.tm_year
}

! Roody's note: Copies one time_object to another one
routine CopyTimeValue(time_orig, time_copy)
{
	time_copy.tm_year   = time_orig.tm_year
	time_copy.tm_month  = time_orig.tm_month
	time_copy.tm_day    = time_orig.tm_day
	time_copy.tm_hour   = time_orig.tm_hour
	time_copy.tm_minute = time_orig.tm_minute
	time_copy.tm_second = time_orig.tm_second

	return true
}

! Roody's note: Prints a time_object, with or without seconds.
routine PrintTimeValue(time, no_seconds)
{
	local printed

	if time.tm_year
	{
		NumberWord(time.tm_year)
		" year";
		if time.tm_year > 1
			print "s";
		printed++
	}

	if time.tm_month
	{
		if printed
			print ", ";
		NumberWord(time.tm_month)
		" month";
		if time.tm_month > 1
			print "s";
		printed++
	}

	if time.tm_day
	{
		if printed
			print ", ";
		NumberWord(time.tm_day)
		" day";
		if time.tm_day > 1
			print "s";
		printed++
	}

	if time.tm_hour
	{
		if printed
			print ", ";
		NumberWord(time.tm_hour)
		" hour";
		if time.tm_hour > 1
			print "s";
		printed++
	}

	if time.tm_minute
	{
		if printed
			print ", ";
		NumberWord(time.tm_minute)
		" minute";
		if time.tm_minute > 1
			print "s";
		printed++
	}

	if time.tm_second and (no_seconds = false or printed = 0)
	{
		if printed
		{
			if printed > 1
				",";
			" and ";
		}
		NumberWord(time.tm_second)
		" second";
		if time.tm_second > 1
			print "s";
	}
}

! A routine that counts the seconds since the "then_object" time object
routine SecondsSince(then_object)
{
	local a
	if then_object.tm_year or then_object.tm_day or then_object.tm_month
		return 1000
	a = then_object.tm_second + (then_object.tm_minute * 60) \
	    + (then_object.tm_hour * 60 * 60)
	return a
}

! A simple routine for comparing two time objects
routine IsTimeLonger(first, second)
{
	if first.tm_year > second.tm_year
		return true
	elseif first.tm_month > second.tm_month
		return true
	elseif first.tm_day > second.tm_day
		return true
	elseif first.tm_hour > second.tm_hour
		return true
	elseif first.tm_minute > second.tm_minute
		return true
	elseif first.tm_second > second.tm_second
		return true
	else
		return false
}

! Returns true if first time object is earlier than second
routine IsEarlier(obj1, obj2)
{

	if obj1.tm_year > obj2.tm_year
		return false
	elseif obj1.tm_month > obj2.tm_month
		return false
	elseif obj1.tm_day > obj2.tm_day
		return false
	elseif obj1.tm_hour > obj2.tm_hour
		return false
	elseif obj1.tm_minute > obj2.tm_minute
		return false
	elseif obj1.tm_second > obj2.tm_second
		return false
	return true
}

! Returns true if obj time object is between time1 and time2
routine IsBetween(obj, time1, time2)
{
	if IsEarlier(time1, obj) and IsEarlier(obj, time2)
		return true
}

! This will print the day of the week, given the supplied time object
! I think the formula I used might only work for the years 2000-2099
! If anyone wants to write a more robust version of this (despite
! how rarely a game needs to know what day of the week it is), feel
! free to write it and send it my way
routine DayOfWeek(obj)
{
	local l,m, i, ret
	l = IsLeapYear(build_year)
	i = obj.tm_year - 2000
	i = i/4  + i
	select obj.tm_month
		case 1
			m = 6
		case 2
			m = 2
		case 3
			m = 2
		case 4
			m = 5
		case 5
			m = 0
		case 6
			m = 3
		case 7
			m = 5
		case 8
			m = 1
		case 9
			m = 4
		case 10
			m = 6
		case 11
			m = 2
		case 12
			m = 4
	i = m + obj.tm_day + i
	if (obj.tm_month = 1,2) and l
		i = i - 1
	i = mod(i,7)
	select i
		case 0
			ret = "Sunday"
		case 1
			ret = "Monday"
		case 2
			ret = "Tuesday"
		case 3
			ret = "Wednesday"
		case 4
			ret = "Thursday"
		case 5
			ret = "Friday"
		case 6
			ret = "Saturday"
	return ret			
}
#endif ! #ifclear _TIMESYSTEM_H