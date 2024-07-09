followers.h is an extension for games in which one or more NPCs follow the player from room to room.

following.h is an extension for games that have NPCs moving around with scripts, so if the player sees them leave a room, >FOLLOW <character> automatically works.

As mentioned in following.h, to work properly, one will need to add the following code to the game's player_character object:

	after
	{
		actor MovePlayer
		{
			local x
			while x < movednpcs[]
			{
				movednpcs[x] = 0
				x++
			}
		}
	}