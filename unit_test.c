/*
 * unit_test.c
 *
 *  Created on: Apr 29, 2016
 *      Author: rob
 */

#include "adventure.h"
/*
 * Test functions - test above functions.
 * */
void test_room_string_function();
void test_room_integer_function();
void test_room_name_integer_functions();
void test_maze(room * layout);
void test_valid_file_names(char ** array);
void test_game_rooms_used(int *roomArray);
void test_validate_maze_legitimate_rooms(room * layout);
void test_array();
void test_Pid();

int main() {

	int * room_order;	//For randomization of rooms
	room * layout; 		//Layout will hold the maze
	time_t t;			//For randomization
	srand(time(&t));		//""
	char ** fileNames;	//Will hold filesnames read from disk
	int i;				//Used for for-loops for malloc

	layout = malloc(7 * sizeof(room));
	for (i = 0; i < MAZE_LENGTH; i++) {
		layout[i].connectedRooms = malloc(MAZE_LENGTH * sizeof(int));
	}
	room_order = malloc(7 * sizeof(int));

	//Game_rooms_used will return a random array of 7 integers.
	//Integers correspond to game room names
	//(see room_number or room_name function)
	game_rooms_used(room_order);
	test_game_rooms_used(room_order);

	//Makes a directory following file specifications of
	//./stinro.rooms.$(process_id)
	make_game_directory();
	test_room_string_function();
	test_room_integer_function();
	test_room_name_integer_functions();

	//Generates a maze of rooms, held in the layout from room_order.
	generate_rooms(room_order, layout);
	free(room_order); //no longer needed.
	test_validate_maze_legitimate_rooms(layout);
	test_maze(layout);

	//Will write rooms to disk. filename == room name
	write_maze_to_files(layout); //Manually tested.

	/*
	 * Free the connected rooms as well as the layout.
	 * This is done because the program will read files in from
	 * the disk.
	 *
	 * */
	for (i = 0; i < MAZE_LENGTH; i++) {
		free(layout[i].connectedRooms);
	}
	free(layout);

	/*
	 * This will allocate space for an array of array of characters - this is where
	 * the filenames will be housed.
	 * */
	fileNames = malloc(7 * sizeof(char*));
	for (i = 0; i < MAZE_LENGTH; i++) {
		fileNames[i] = malloc(sizeof(char) * 30);

	}
	read_file_names_from_directory(fileNames);
	test_valid_file_names(fileNames);

	/*
	 * Allocate space for the maze.
	 * Double allocation required for integers holding room connections.
	 * */
	layout = malloc(7 * sizeof(room)); //Made to house files from disk.
	for (i = 0; i < MAZE_LENGTH; i++) {
		layout[i].connectedRooms = malloc(MAZE_LENGTH * sizeof(int));
	}

	/*
	 * This will generate a maze
	 * from the files listed.
	 *
	 * */
	make_maze_from_files(layout, fileNames);
	test_validate_maze_legitimate_rooms(layout);
	test_maze(layout);
	for (i = 0; i < MAZE_LENGTH; i++) {
		free(fileNames[i]);
	}
	free(fileNames);

	/*
	 * Test Array will test the dynamic array implemented
	 * to hold the amount of rooms visited.
	 *
	 * */
	test_array();

	/*
	 * Free's the last memory and returns with 0.
	 * */
	for (i = 0; i < MAZE_LENGTH; i++) {
		free(layout[i].connectedRooms);
	}
	free(layout);
	return 0;

}

//Simple test function to see if filenames are valid choices.
void test_valid_file_names(char ** fileNames) {

	int i;
	for (i = 0; i < MAZE_LENGTH; i++) {
		assert(room_number(fileNames[i]) != -1);
	}

	return;

}

/*
 * This test will test the layout to ensure a valid path to the finish
 * is available. THIS TEST WILL HANG THE PROGRAM IF THERE IS NO VALID WAY
 * */
void test_maze(room * layout) {
	int currentRoom;
	int rooms[MAZE_LENGTH - 1];
	int totalRooms;
	int door;
	int i;
	int newRoom;
	int arrayLocation;

	/*
	 * Figure out indices for start room.
	 * input start room to rooms array.
	 * */
	for (i = 0; i < MAZE_LENGTH; i++) {
		if (layout[i].startRoom == 1) {
			currentRoom = layout[i].roomNumber;
			totalRooms = 1;
			rooms[0] = layout[i].roomNumber;
		}
	}

	while (totalRooms < MAZE_LENGTH) {

		newRoom = 0; //This is a toggle to see if the random choice is a new room or not.

		//Index for current room is found from room array.
		for (i = 0; i < MAZE_LENGTH; i++) {
			if (layout[i].roomNumber == currentRoom) {
				arrayLocation = i;
				break;
			}
		}

		//Attempt to take a random connection.
		//If it is a previously taken connection toggle newRoom to 1.
		door = random() % layout[arrayLocation].roomsConnected;
		for (i = 0; i < totalRooms; i++) {
			if (layout[arrayLocation].connectedRooms[door] == rooms[i]) {

				newRoom = 1;
			}
		}
		//If a room is NEW (i.e. newRoom ==0) then add it to the room array,
		// increment total rooms.
		if (newRoom == 0) {
			rooms[totalRooms] = layout[arrayLocation].connectedRooms[door];
			totalRooms++;
		}
		//Regardless of outcome, set current room to the random path taken.
		currentRoom = layout[arrayLocation].connectedRooms[door];
	}
}

/*
 * Tester function for room_name and room_number
 * */
void test_room_name_integer_functions() {
	int i;
	int returnedNumber;
	char *roomName;

	for (i = 0; i < 10; i++) {
		roomName = room_name(i);
		returnedNumber = room_number(roomName);
		assert(returnedNumber == i);
	}
}

/*
 * Tester function for room_number.
 *
 * Test "Lounge" Expects 6 as result back.
 *
 * */
void test_room_integer_function() {
	int roomNum;
	int roomCompare;
	char * roomString;
	roomNum = 6;
	roomString = "Lounge";

	roomCompare = room_number(roomString);

	assert(roomCompare == roomNum);
}

/*
 *	Small test of test_room function.
 * */
void test_room_string_function()

{
	char *Wally_room = room_name(0);
	char *Wally_compare = "Wally-Room";
	assert(strcmp(Wally_room, Wally_compare) == 0);
}
/*This test will make sure that the array only has unique
 * integer values. This is important because rooms are represented by
 * integers in parts of the program.*/
void test_game_rooms_used(int * test_array) {
	int compareArray[MAZE_LENGTH - 1];
	int i;
	int j;
	int compareArrayLength;
	int counter;

	compareArrayLength = 0;

	for (i = 0; i < MAZE_LENGTH; i++) {
		compareArray[i] = test_array[i];
		compareArrayLength++;
		counter = 0;
		for (j = 0; j < compareArrayLength; j++) {
			if (compareArray[j] == test_array[i]) {
				counter++;
			}
			assert(counter < 2);
		}
	}
	return;
}

/*
 * Test to see PID always returns the same value.
 *
 * This is because the process ID does not change.
 * */
void test_Pid() {
	int i;
	int j;

	i = getpid();
	j = getpid();
	assert(i == j);

	return;
}

/*
 * Test Function. Will take in room array and test each value to make sure it is legitimate.
 * Will make sure array has only 1 start and end room.
 * Will check all connected rooms.
 * */
void test_validate_maze_legitimate_rooms(room * layout) {
	int i;
	int start_Counter;
	int end_Counter;
	int j;
	start_Counter = 0;
	end_Counter = 0;
	for (i = 0; i < MAZE_LENGTH; i++) {
		assert(layout[i].roomNumber < 11);
		assert(strcmp(room_name(layout[i].roomNumber), "NO DATA") != 0);
		for (j = 0; j < layout[i].roomsConnected; j++) {
			assert(
					strcmp(room_name(layout[i].connectedRooms[j]), "NO DATA")
							!= 0);
		}

		if (layout[i].startRoom == 1) {
			start_Counter++;
		}
		if (layout[i].endRoom == 1) {
			end_Counter++;
		}
		assert(!(layout[i].startRoom == 1 && layout[i].endRoom == 1));

		assert(layout[i].roomsConnected >= 0);
	}
	assert(start_Counter == 1);
	assert(end_Counter == 1);

	return;
}

/*I did not error test all functions in dynArray because data structure is relatively simple.
 * Function creates an array, forces it to resize dynamically, and then frees array.*/
void test_array() {
	dynArray * testArray;
	testArray = createDynArray(5);
	int i;
	for (i = 0; i < 13; i++) {
		addArray(testArray, i);
	}

	for (i = 0; i < 13; i++) {
		getArray(testArray, i);
	}

	freeArray(testArray);
}
