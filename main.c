/*
 * main.c
 *
 *  Created on: Apr 27, 2016
 *      Author: rob
 */
#include "adventure.h"

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

	//Makes a directory following file specifications of
	//./stinro.rooms.$(process_id)
	make_game_directory();

	//Generates a maze of rooms, held in the layout from room_order.
	generate_rooms(room_order, layout);
	free(room_order); //no longer needed.

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
	for (i = 0; i < MAZE_LENGTH; i++) {
		free(fileNames[i]);
	}
	free(fileNames);

	playGame(layout);
	/*
	 * Free's the last memory and returns with 0.
	 * */
	for (i = 0; i < MAZE_LENGTH; i++) {
		free(layout[i].connectedRooms);
	}
	free(layout);
	return 0;

}
