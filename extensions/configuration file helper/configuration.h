!----------------------------------------------------------------------------
!* CONFIGURATION FILE HANDLER
!----------------------------------------------------------------------------

! Roody's note:
! This configuration file manager exists so that a game that uses several
! different systems saving information to a configuration file (like, say,
! colorlib.h, music settings, and whatever else a game may need to save) can
! easily write all of these things without a big headache to the author.
!
! Just declare a DATA_FILE constant before roodylib is included. This is the
! data file that will be created:
!
!constant DATA_FILE "nddata" ! creates a "nddata file"

! Example configuration saving thingie:
!\

 If your DATA_FILE is going to be shared between two different games
  (like in Roody Yogurt's "Halloween Horror" game), make sure that the 
  DATA_FILE has the same name in both and configuration files are 
  included in the same order

 If your game requires an interpreter that writes to configuration files
 properly, setting the CONFIGURATION_CHECK flag will insert a test at the
 beginning of the game.  

  This configuration check needs the "timesystem.h" extension (from the
  "time management" folder) included before "configuration.h" is included.

\!
!----------------------------------------------------------------------------
#ifclear _CONFIGURATION_H

!\ FILE_CHECK_START - a "file start" version of FILE_CHECK. Is the first thing
written to a data file, so LoadSettings can check for it and cut out early if
it's not there.
\!
constant FILE_CHECK_START     4659

!\ configlib is an init_instructions object that calls each configuration file's
setup property. This allows pre-game questions like, "Do you want color?" and so
on. Have the setup property return true if InitScreen should be called
afterward.

	It also goes through all of the configuration objects and determines their
	names' StringSum values, saving them in their name_sum properties.
\!

property name_sum alias size
property load_info alias ne_to
! A save_info property already exists, thanks to RoodyLib
!property save_info alias e_to
property setup alias se_to ! put a property routine here if you want
                           ! any code to run at the start of the game
			   ! (like a multimedia game asking the player
			   ! if he or she wants to play with music)
property first_time alias s_to ! if you want to ask a settings question
                               ! only the first time someone plays the game
			       ! set this to true (see example code)

object configlib
{
	in init_instructions
	execute
	{
		local i, l

		if not child(config_instructions)
			return
		if not CheckWordSetting("undo") and not CheckWordSetting("restore")
		{
			for i in config_instructions
			{
				l = string(_temp_string, i.name)
				i.name_sum = StringSum(_temp_string)
			}
		}
		if not CheckWordSetting("undo")
		{
			LoadConfigSettings
			if not CheckWordSetting("restore")
			{
				for i in config_instructions
				{
					if i.setup
						InitScreen
				}
#ifset CONFIGURATION_CHECK
				if not CheckTerpConfigAbility
					Config_Error
#endif
			}
		}
	}
}

#ifset CONFIGURATION_CHECK
time_object before_save
{}

time_object after_save
{}

time_object time_diff
{}

routine CheckTerpConfigAbility
{
	local x
	GetCurrentTime(before_save)
	SaveConfigSettings
	GetCurrentTime(after_save)
	readfile DATA_FILE
	{
		x = readval
		x = readval
		before_save.tm_year = readval
		before_save.tm_month = readval
		before_save.tm_day = readval
		before_save.tm_hour = readval
		before_save.tm_minute = readval
		before_save.tm_second = readval
	}
	CalculateTimeDifference(after_save, before_save, time_diff)
	x = SecondsSince(time_diff)
	if x < 0 or x > 5
		return false
	return true
}

routine Config_Error
{}
#endif ! CONFIGURATION_CHECK

!\ config_instructions is an object for holding all of the configuration file
objects \!

object config_instructions
{
	type settings
}

#ifset CONFIGURATION_CHECK
object config_config "Config File Terp Check v1"
{
	in config_instructions
	name_sum 0  ! we don't need a value here but we need the slot
	load_info
	{
		before_save.tm_year = readval
		before_save.tm_month = readval
		before_save.tm_day = readval
		before_save.tm_hour = readval
		before_save.tm_minute = readval
		before_save.tm_second = readval
	}
	save_info
	{
		writeval before_save.tm_year
		writeval before_save.tm_month
		writeval before_save.tm_day
		writeval before_save.tm_hour
		writeval before_save.tm_minute
		writeval before_save.tm_second
	}
}
#endif

routine LoadConfigSettings
{
	readfile DATA_FILE
	{
		local a, i
		while true
		{
			a = readval
			if a++ ~= FILE_CHECK_START
			{
				break
			}
			for i in config_instructions
			{
				a = readval
				if a ~= i.name_sum
					break
				else
					run i.load_info
			}
			a = readval
			break
		}
		if a ~= FILE_CHECK
			return false
		else
			return true
	}
}

routine SaveConfigSettings
{
	if not child(config_instructions)
		return

	writefile DATA_FILE
	{
		local i
		writeval FILE_CHECK_START
		for i in config_instructions
		{
			writeval i.name_sum
			run i.save_info
		}
		writeval FILE_CHECK
	}
	return (not system_status) ! I don't think this actually works but oh well
}

!\ StringSum adds up the ASCII value of a string. This extension uses
it to differentiate one configuration file object from another. \!

routine StringSum(arr)
{
	local i, sum

	while (array arr[i] and i < array arr[])
	{
		sum += array arr[i]
		i++
	}

	return sum
}
#endif  ! _CONFIGURATION_H