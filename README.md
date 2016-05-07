Adventure Game (SIMPLE MAZE)

This is an extremely simplified version of a game like Colossal Cave Adventure assigned by my Operating Systems Course. This is written in C.

In order to run the program do the following:

1) Download all the files into the same directory. 
2) Open a command prompt, navigate to inside the folder and type "Make all". 
3) Run the game by typing "./adventure" in the command prompt (while in the same folder).

To run the unit test follow 1 and 2 and then type "Make runtests". Results will be in unittestresult.out.
The current test coverage is : Lines executed:81.15% of 260. 
Test coverage when function "play_game()" is removed is : Lines executed:96.26% of 214

This program will generate a maze, write files to a disk, correctly read files from the disk into a maze and then allow the player to solve the Maze.

The files are named after the room. The files are written as:

ROOM NAME: <room name>
CONNECTION 1: <room name>
…
ROOM TYPE: <room type>

Room type "START_ROOM" is the start of the maze. "END_ROOM" is the end of the maze.

Players are prompted with screen:

CURRENT LOCATION: <room name>
POSSIBLE CONNECTIONS: <list of connections>
WHERE TO? > <input data>

A valid, correctly capitalized and spelled input will move user from room to room. If input is incorrect it will be generate the following error message:

HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.

Once the end room is successfully navigated to the end game prompt will generate. It will look something like this:

YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!
YOU TOOK 2 STEPS. YOUR PATH TO VICTORY WAS:

Wally-Room

Lounge

%

The room files are housed directory "stinero.rooms.<pid>". The created directory is not deleted after the program has completed. All unnecessary files can be deleted by typing "Make clean".

The room is held in a struct. Most of the program logic around building the maze and reading it from a file depends on this so I have explained it here.

typedef struct room {
	int acceptingConnections; 	-> Used when the maze is being created.
	int roomsConnected; 		-> Used to show how many rooms are connected to this room. Serves as an Index for connectedRooms.
	int roomNumber;			-> Integer room number. Used to see which room.
	int * connectedRooms;		-> Array of connected rooms.
	int endRoom;			-> 0 if not the end room, 1 if end room.
	int startRoom;			-> 0 if not the start room, 1 if start room.
} room;


The files rely heavily on int room_number(char *name); *char room_number(int) which will translate a character array room name (e.g. Wally-Room) to a number (e.g. 1). This amount of coupling is not ideal
and int he future I would probably just 

Thank you for reviewing. I hope you like it!
