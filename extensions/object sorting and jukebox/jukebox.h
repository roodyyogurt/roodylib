#ifclear _MUSICJUKEBOX_H
#set _MUSICJUKEBOX_H

object jukebox "music manager"
{}

time_object current_song_length
{}

global current_song

property length alias u_to
property file alias d_to
property artist alias s_to

class song
{}

routine PlayJukebox
{
	local a,success_play, retry
	while true
	{
		a = child(jukebox)
		if not a or retry > 5
		{
			current_song = 0
			music 0
			audio.current_music = 0
			jukebox is not switchedon
			return
		}
		if not is_music_playing is switchedon
		{
			current_song_length.tm_minute = a.length
			current_song_length.tm_second = a.length #2
		}
#if defined MUSIC_RESOURCE_FILE
		success_play = PlayMusic(MUSIC_RESOURCE_FILE, a.file,100,0,1)
#endif
#if undefined MUSIC_RESOURCE_FILE
		success_play = PlayMusic(0, a.file,100,0,1)
#endif
		move a to jukebox
		if success_play
		{
			current_song = a
			jukebox is switchedon
			break
		}
		else
			retry++
	}
}

routine StopJukebox
{
	current_song = 0
	music 0
	audio.current_music = 0
	jukebox is not switchedon
}

routine PlaySong(songfile,loop)
{
	local success_play
	if not loop
	{
		current_song_length.tm_minute = songfile.length
		current_song_length.tm_second = songfile.length #2
	}
#if defined MUSIC_RESOURCE_FILE
	success_play = PlayMusic(MUSIC_RESOURCE_FILE, songfile.file,100,loop,true)
#endif
#if undefined MUSIC_RESOURCE_FILE
	success_play = PlayMusic(0, songfile.file,100,loop,true)
#endif
	if not success_play
	{
		current_song = 0
		music 0
		audio.current_music = 0
	}
	else
		current_song = songfile
}

replace PlayMusic(resfile, song, volume, loop, force)
{
	if (system(61) and not allow_gargoyle)
		return false
	! If the given song is already playing:
	if audio.current_music_resfile = resfile and
		audio.current_music = song
	{
		if not force
			return
		else
			music 0
	}

	local vol
	vol = volume
	if vol = 0
		vol = 100
	elseif vol < 0
		vol = 0

	if resfile = MUSIC_STOP and not song
	{
		music 0
		audio.current_music = 0
	}
	else
	{
		if not loop
		{
			music resfile, song, vol
			if not system_status and not (is_music_playing is switchedon)
				GetCurrentTime(song_start)
		}
		else
			music repeat resfile, song, vol
		audio.current_music = song
		audio.current_music_looping = loop
	}

	audio.current_music_resfile = resfile
	last_resourcefile = resfile
	audio.current_music_volume = volume

	if system_status:  return false

	return true
}

routine CheckSongEnd
{
	if is_music_playing is switchedon
	{
		if not ExecOpCode(is_music_playing)
			audio.current_music = 0
		return
	}
	GetCurrentTime(current_time)
	CalculateTimeDifference(current_time, song_start, time_difference)
	if IsTimeLonger(time_difference, current_song_length)
		audio.current_music = 0
}

routine NowPlaying
{
	CheckSongEnd
	if not audio.current_music
	{
		if jukebox is switchedon
			PlayJukebox
	}
	if not audio.current_music
		"There is no song currently playing."
	else
	{
		if current_song.artist
			print "\"";
		print current_song.name;
		if current_song.artist
		{
			print "\" by ";
			print current_song.artist;
		}
		print " is currently playing."
	}
}

object jukebox_player
{
	in main_instructions
	execute
	{
		if audio.current_music
			CheckSongEnd
		if not audio.current_music and jukebox is switchedon
			PlayJukebox
	}
}

#endif ! _MUSICJUKEBOX_H