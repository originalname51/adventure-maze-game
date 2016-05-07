/*
 * adventure.c
 *
 *  Created on: Apr 23, 2016
 *      Author: rob
 *
 *
 *      First program will generate a maze using an array of "Rooms" (containing start/end room, connected rooms
 *      the room number and if the room is accepting new connections.
 *
 *      Then program will write this to a file.
 *
 *      Program will then read the file back into the program to make an array of rooms from the file (this satisfies
 *      the assigned "read data from a file" task.
 *
 *      Program then has user go and play the game, outputting correct font.
 *
 *      */

#include "adventure.h"

/*
 * Play Game intakes a complete room layout and will
 * have the user attempt to navigate to the end-room.
 * */
void playGame(room * layout) {

	/*
	 * Rooms holds the rooms visited.
	 * Current Room holds the INDEX IN LAYOUT of the current room player is in.
	 * Choice is attempted INDEX IN LAYOUT the user is going to.
	 * flag is set to 1 for valid user choice.
	 * */

	dynArray * rooms;
	int currentRoom;
	int choice;
	int i;
	int flag;
	char input[256];
	int END_ROOM_NUMBER;

	currentRoom = -1;
	rooms = createDynArray(15);

	//Initializing indices of start and end room for while loop.
	for (i = 0; i < MAZE_LENGTH; i++) {
		if (layout[i].endRoom == 1) {
			END_ROOM_NUMBER = i;
		}
		if (layout[i].startRoom == 1) {
			currentRoom = i;
		}
	}

	assert(currentRoom != -1);

	/*
	 * Will continue until user successfully moves to the end-room.
	 * */
	while (layout[currentRoom].roomNumber != layout[END_ROOM_NUMBER].roomNumber) {
		/*
		 * This will send formatted output to the user, listing current room, connected rooms, and ask them
		 * Where to?
		 *
		 */
		printf("\nCURRENT LOCATION: %s\n",
				room_name(layout[currentRoom].roomNumber));
		printf("POSSIBLE CONNECTIONS: ");
		for (i = 0; i < layout[currentRoom].roomsConnected; i++) {
			if (i + 1 != layout[currentRoom].roomsConnected) {
				printf("%s, ",
						room_name(layout[currentRoom].connectedRooms[i]));
			} else {
				printf("%s.\n",
						room_name(layout[currentRoom].connectedRooms[i]));

			}
		}
		printf("WHERE TO? >");

		/*
		 * fgets used as it has protection against buffer overloading.
		 * Will take input and then take away newline.
		 * */
		fgets(input, 256, stdin);
		input[strlen(input) - 1] = '\0';

		flag = 0;
		//Will set flag to 0 if room is legitimate choice AND connected to currentRoom
		if (room_number(input) != -1) {
			choice = room_number(input);
			for (i = 0; i < layout[currentRoom].roomsConnected; i++) {
				if (choice == layout[currentRoom].connectedRooms[i]) {
					flag = 1;
				}
			}
		}

		//Will update indices AND add where the user is going to the array. Else error message.
		if (flag == 1) {
			for (i = 0; i < MAZE_LENGTH; i++) {
				if (choice == layout[i].roomNumber) {
					currentRoom = i;
					addArray(rooms, layout[currentRoom].roomNumber);
				}
			}
		} else {
			printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		}
	}

	printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\nYOU TOOK");
	printf(" %d STEPS. YOUR PATH TO VICTORY WAS:\n", arraySize(rooms));
	for (i = 0; i < arraySize(rooms); i++) {
		printf("%s\n", room_name(getArray(rooms, i)));
	}
	printf("%%\n"); //Used following : http://stackoverflow.com/questions/17774821/how-do-i-print-the-percent-sign-in-c
	freeArray(rooms);

	return;
}

int arraySize(dynArray *arr) {
	assert(arr != NULL);
	return arr->size;

}
//Free's memory in dynArray destroying it.
void freeArray(dynArray *arr) {
	free(arr->data);
	free(arr);
	return;
}

/*
 Adds data to array. Will resize array if needed.
 * */
void addArray(dynArray * array, int room) {
	int i;
	int *newArray;
	int *trash;
	if (array->size == array->cap) {
		array->cap = 2 * array->cap;
		newArray = malloc(array->cap * sizeof(int));
		//copy array over.
		for (i = 0; i < array->size; i++) {
			newArray[i] = array->data[i];
		}
		trash = array->data;
		array->data = newArray;
		free(trash);
	}
	array->data[array->size] = room;
	array->size++;

	return;
}

//returns value of array at index.
int getArray(dynArray * array, int index) {
	assert(index < array->size);
	return (array->data[index]);
}

//Creates a dynamic array.
//Calls initDynArr to initialize internal functions.
dynArray * createDynArray(int cap) {
	dynArray * newArray;

	newArray = malloc(sizeof(dynArray));
	initDynArr(newArray, cap);

	return newArray;
}

//Initializes internal dynArry structures
void initDynArr(dynArray * arr, int cap) {
	arr->data = malloc(cap * sizeof(int));
	assert(arr->data != 0);
	arr->size = 0;
	arr->cap = cap;
}

/*
 *http://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
 *http://stackoverflow.com/questions/23456374/why-do-we-use-null-in-strtok
 *http://stackoverflow.com/questions/9406475/why-is-strtok-changing-its-input-like-this
 * ^ Work Cited ^
 *
 * This is a core function to the program. It will take an array of character arrays as input. These
 * Are the filenames of the program.
 *
 * It will fopen all filenames from program.
 * Then it will read the program line by line.
 * Depending on the line it will assign data to the program.
 *
 * */
void make_maze_from_files(room * layout, char ** fileNames) {
	int i;
	int pid;
	char *c;
	char s[50];
	char new[100];
	char *paren;
	char * name;
	pid = getpid();
	char fileLine[100];
	char * dataHolder;
	FILE * fptr;

	for (i = 0; i < MAZE_LENGTH; i++) {
		//Get filepath to open correct file.
		paren = "/";
		c = "./stinero.rooms.";
		name = fileNames[i];
		sprintf(s, "%d", pid);
		strcpy(new, c);
		strcat(new, s);
		strcat(new, paren);
		strcat(new, name);
		layout[i].roomsConnected = 0; // Initialize roomsConnected to 0.
		fptr = fopen(new, "r");	//open file as read.

		while (fgets(fileLine, 100, fptr) != NULL)	//While fgets reads a line
		{

			/*
			 * Because file Structure is
			 * ROOM NAME:
			 * CONNECT x:
			 * ROOM TYPE:
			 *
			 * I have broken the first part of the string using ":".
			 *
			 * I then use an if statement to correctly parse data.
			 * */
			dataHolder = strtok(fileLine, ":");
			if (dataHolder != NULL) {
				if (strcmp(dataHolder, "ROOM NAME") == 0) {
					//Get the next string using STRTOK delimiter " ".
					dataHolder = strtok(NULL, " ");
					dataHolder[strlen(dataHolder) - 1] = '\0'; //cut off \n.
					layout[i].roomNumber = room_number(dataHolder); //assign integer number for room name.

				}
				//Set the type of room using room_type_start_or_end to figure out
				//if it is a MID_ROOM, START_ROOM or END_ROOM.
				//NOTE: Because it is EOF no cutting a \n is required.
				else if (strcmp(dataHolder, "ROOM TYPE") == 0) {

					dataHolder = strtok(NULL, " ");
					if (room_type_start_or_end(dataHolder) == 1) {
						assert(room_type_start_or_end(dataHolder) == 1);
						layout[i].startRoom = 1;
						layout[i].endRoom = 0;
					} else if (room_type_start_or_end(dataHolder) == -1) {
						assert(room_type_start_or_end(dataHolder) == -1);
						layout[i].startRoom = 0;
						layout[i].endRoom = 1;
					} else {
						assert(room_type_start_or_end(dataHolder) == 0);
						layout[i].startRoom = 0;
						layout[i].endRoom = 0;
					}
				}
				/*
				 * If it is not the Room Name or the Room Type it must be a connected room.
				 * Assign accordingly, incrementing rooms Connected variable.
				 * */
				else {
					dataHolder = strtok('\0', " ");
					dataHolder[strlen(dataHolder) - 1] = '\0';

					layout[i].connectedRooms[layout[i].roomsConnected] =
							room_number(dataHolder);
					assert(
							layout[i].connectedRooms[layout[i].roomsConnected]
									== room_number(dataHolder));
					layout[i].roomsConnected++;

				}
			}
		}
		fclose(fptr); //file is opened and closed in for loop.
	}

	return;
}

/*
 * Helper function to 0 for mid room, 1 for start room and -1 for end_room. Return -2 on error.
 * */
int room_type_start_or_end(char * room) {

	if (strcmp(room, "MID_ROOM") == 0) {
		return 0;
	} else if (strcmp(room, "START_ROOM") == 0) {
		return 1;
	} else if (strcmp(room, "END_ROOM") == 0) {
		return -1;
	}

	return -2;

}

/* //http://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html#Simple-Directory-Lister
 * This function primarily list files from a directory using the DIR and DIRENT.
 * This will open a directory and cycle through rooms.
 * It will then copy the rooms into a character array of character arrays
 * if the string is a valid file name. This is done to avoid "." and ".."
 * */
void read_file_names_from_directory(char ** array) {
	int pid;
	char *c;
	char s[50];
	char new[100];
	char *paren;
	int fileCounter; //Used as index for array.
	DIR *directory;
	struct dirent *ep;

	//Set Directory
	fileCounter = 0;
	pid = getpid();
	paren = "/";
	c = "./stinero.rooms.";
	sprintf(s, "%d", pid);
	strcpy(new, c);
	strcat(new, s);
	strcat(new, paren);

	//Open Directory and read contents.
	directory = opendir(new);
	if (directory != NULL) {
		while ((ep = readdir(directory))) //While the data is being read in.
		{
			//Test to see if the name is a valid file name.
			if (room_number(ep->d_name) != -1) {
				//If valid file name copy it into a character array at fileCounter(index).
				strcpy(array[fileCounter], ep->d_name);
				fileCounter++;
				;
			}
		}
		closedir(directory); //close the directory.
	} else {
		perror(
				"Directory is not available. Critical Error. Has program made directory?\n");
	}
	return;
}

/*
 * This will generate a set of rooms based on an array of
 * integers of MAZE_LENGTH valued at 0-9(to represent
 * the maze rooms.
 * First it will initialize the layout. It will set the index 0 to start_room and
 * last index to end_room.
 * */
void generate_rooms(int * room_order, room * layout) {
	int i;
	int roomsToConect;
	int randomRoomNumber;
	int alreadyConnected;
	int j;

	//Initialization Loop.
	for (i = 0; i < MAZE_LENGTH; i++) {
		layout[i].acceptingConnections = 1;
		layout[i].roomNumber = room_order[i];
		layout[i].roomsConnected = 0;
		layout[i].endRoom = 0;
		layout[i].startRoom = 0;
	}

	//Set first and last room to start and end room.
	layout[0].startRoom = 1;
	layout[MAZE_LENGTH - 1].endRoom = 1;

	//For each room in the array, generate random connections and try to connect them.
	for (i = 0; i < MAZE_LENGTH; i++) {

		roomsToConect = (random() % 3) + 2; //This will set connected rooms between 2-5.

		//Loop until connected to enough rooms.
		while (roomsToConect > layout[i].roomsConnected) {
			alreadyConnected = 0;
			randomRoomNumber = (random() % (MAZE_LENGTH)); //Pick a random room to connect to.

			/*If the room is not the same room, AND is accepting connections
			 * Cycle through the current connections to make sure it isn't
			 * already connected. If it ISNT already connected then connect each
			 * room to each other. If this connection increments the connections
			 * 5(i.e. 6) then have the room (randomRoomNumber) turn off accepting new connections.
			 * */
			if (layout[i].roomNumber != layout[randomRoomNumber].roomNumber) {
				if (layout[randomRoomNumber].acceptingConnections == 1) {

					for (j = 0; j < layout[i].roomsConnected; j++) {
						if (layout[randomRoomNumber].roomNumber
								== layout[i].connectedRooms[j]) {
							alreadyConnected = 1;
						}
					}
					/*
					 * The following (alreadyConnected != 1) activates IF
					 * the room is not already connected
					 * AND is accepting connections AND is
					 * not the same room
					 * **/
					if (alreadyConnected != 1) {

						layout[i].connectedRooms[layout[i].roomsConnected] =
								layout[randomRoomNumber].roomNumber;
						layout[i].roomsConnected++;

						layout[randomRoomNumber].connectedRooms[layout[randomRoomNumber].roomsConnected] =
								layout[i].roomNumber;
						layout[randomRoomNumber].roomsConnected++;

						if (layout[randomRoomNumber].roomsConnected > 4) {
							layout[randomRoomNumber].acceptingConnections = 0;
						}
					}
				}

			}
		}
		//If the room has 6 connections then turn off accepting connections.
		if (layout[i].roomsConnected > 4) {
			layout[i].acceptingConnections = 0;
		}

	}

	return;
}

/*
 *	This will write the contents of the LAYOUT room array to
 *	MAZE_LENGTH files, each named for the room they represent.
 * */
void write_maze_to_files(room * layout) {
	int pid;
	char *c;
	char s[20];
	char new[30];
	FILE *fptr;
	char *paren;
	char *room;
	int i;
	int j;

	pid = getpid();

	for (i = 0; i < MAZE_LENGTH; i++) {
		paren = "/";
		c = "./stinero.rooms.";
		sprintf(s, "%d", pid); //convert pid to string
		strcpy(new, c);	 //put"./stinero.rooms." into a new string
		strcat(new, s); //append pid to stinero.rooms.
		strcat(new, paren); // Add a "/" to make it ./stinero.rooms.pid/"(Now it's ready for the file name)
		room = room_name(layout[i].roomNumber);	//put room string into array.
		strcat(new, room);

		//Open filepath ./stinero.rooms/${PID}/${ROOMNAME} as
		//Write, creating file.
		fptr = fopen(new, "w");
		/*
		 * fprintf used to write to the file.
		 * Will use a for loop to write connection files.
		 * */
		fprintf(fptr, "ROOM NAME: %s\n", room_name(layout[i].roomNumber));
		for (j = 0; j < layout[i].roomsConnected; j++) {
			fprintf(fptr, "CONNECTION %d: %s\n", j + 1,
					room_name(layout[i].connectedRooms[j]));
		}
		fprintf(fptr, "ROOM TYPE: %s", room_type(layout, layout[i].roomNumber));
		fclose(fptr);
	}
	return;
}

//Return type of room by ROOM number (**not** array position!) in Layout.
char *room_type(room * layout, int number) {
	room returnNumber;
	int i;

	//Obtain correct index to get ROOM information.
	for (i = 0; i < MAZE_LENGTH; i++) {
		if (layout[i].roomNumber == number) {
			returnNumber = layout[i];
			break;
		}
	}

	assert(!(returnNumber.startRoom == 1 && returnNumber.endRoom == 1));

	//Return room information.
	if (returnNumber.startRoom == 1) {
		return "START_ROOM";
	} else if (returnNumber.endRoom == 1) {
		return "END_ROOM";
	} else if (returnNumber.startRoom == 0 && returnNumber.endRoom == 0) {
		return "MID_ROOM";
	}

	return "ERROR";

}

/*
 * Helper function.
 *
 * Returns a number based on the string name. Return -1 on no room found.
 * */
int room_number(char *name) {
	if (strcmp(name, "Wally-Room") == 0) {
		return 0;
	} else if (strcmp(name, "Sun-Room") == 0) {
		return 1;
	} else if (strcmp(name, "Study") == 0) {
		return 2;
	} else if (strcmp(name, "Living-Room") == 0) {
		return 3;
	} else if (strcmp(name, "Dining-Room") == 0) {
		return 4;
	} else if (strcmp(name, "Server-Room") == 0) {
		return 5;
	} else if (strcmp(name, "Lounge") == 0) {
		return 6;
	} else if (strcmp(name, "Storage") == 0) {
		return 7;
	} else if (strcmp(name, "Laundry-Room") == 0) {
		return 8;
	} else if (strcmp(name, "Pillow-Fort") == 0) {
		return 9;
	} else {
		return -1;
	}
	return -1;
}
/*
 * Helper function.
 *
 * Return character array based on integer number input.
 *
 * Return "NO DATA" on unknown room type.
 * */
char * room_name(int number) {
	switch (number) {
	case 0:
		return "Wally-Room";
	case 1:
		return "Sun-Room";
	case 2:
		return "Study";
	case 3:
		return "Living-Room";
	case 4:
		return "Dining-Room";
	case 5:
		return "Server-Room";
	case 6:
		return "Lounge";
	case 7:
		return "Storage";
	case 8:
		return "Laundry-Room";
	case 9:
		return "Pillow-Fort";
	default:
		return "NO DATA";
	}

	return "NO DATA";

}

/*Return a random array containing 7 numbers between 0-9*/
void game_rooms_used(int * random_rooms) {
	int room_options[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len_options = 9;
	int used_rooms = 0;
	int room_choice;
	int i;

	//This will pick a random room out of the room_options array. (using index len_options).
	//It will then erase the number from the array.
	//It will continue until MAZE_LENGTH values are stored.
	while (used_rooms < MAZE_LENGTH) {
		room_choice = random() % len_options+1;
		random_rooms[used_rooms] = room_options[room_choice];

		len_options--;
		used_rooms++;

		for (i = room_choice; i < len_options+1; i++) {
			room_options[i] = room_options[i + 1];
		}
	}

	return;
}

/*
 * This will make the game directory.
 * It will be stinero.rooms/PID/
 * */
void make_game_directory() {

	int pid;
	char *c = "stinero.rooms."; //set base.
	char i[20];
	char new[20];

	pid = getpid(); //get the PID

	sprintf(i, "%d", pid); //convert PID to string
	strcpy(new, c);	//add stinero.rooms/ to string
	strcat(new, i); //add PID to string
	mkdir(new, 0755); //make a new directory with the string.
	assert(mkdir(new, 0755) == -1); //assert that the directory has been made.
	return;
}

