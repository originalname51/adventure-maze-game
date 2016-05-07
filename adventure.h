/*
 * adventure.h
 *
 *  Created on: Apr 24, 2016
 *      Author: rob
 */

#ifndef ADVENTURE_H_
#define ADVENTURE_H_

#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/stat.h>

//Sets length of maze. Makes it a bit more expendable.
#define MAZE_LENGTH 7

/*
 * Accepting connections used for generating a maze
 * Rooms Connected is index for connected rooms
 * room number is integer number for the room
 * Connected Rooms is array of connected rooms.
 * End room is a toggle to set room as "end room"
 * Start room is toggle to set room as "start room"
 * */
typedef struct room {
	int acceptingConnections;
	int roomsConnected;
	int roomNumber;
	int * connectedRooms;
	int endRoom;
	int startRoom;
} room;

/*
 * Implementation of dynamic array. size is total size, cap is capacity, data is array.
 * */
typedef struct dynArray {
	int size;
	int cap;
	int * data;
} dynArray;

//Holds game logic.
void playGame(room * layout);
//Return integers for room type. (-1 for end room,0 for mid room,1 for start_room) -2 on error.
int room_type_start_or_end(char * room);

//**takes filenames to read and makes a maze from them**
void make_maze_from_files(room * layout, char ** fileNames);

//Opens directory and writes a maze to file.
void write_maze_to_files(room * layout);

//Scans directory, returns all applicable filenames in character array, array.(must be preinitialized)
void read_file_names_from_directory(char ** array);

//returns room number from room_name string
int room_number(char *name);

//returns the type of room. e.g. "Start_Room"
char *room_type(room * layout, int number);

//Creates dynamic array
dynArray * createDynArray(int cap);

//Delete dynamic array, freeing memory
void freeArray(dynArray *arr);

//returns arraySize
int arraySize(dynArray *arr);

//Gets value at index index from array
int getArray(dynArray * array, int index);

//Adds value to dynamc array
void addArray(dynArray * array, int room);

//internal function for dynamic array
void initDynArr(dynArray *arr, int cap);

//returns process ID
int getpid();

//Makes the game directory folder
void make_game_directory();

//Sets random integer values of rooms to roomArray
void game_rooms_used(int *roomArray);

//Generates rooms based on room_order
void generate_rooms(int * room_order, room * layout);

//Helper function - returns character array of room name.
char * room_name(int number);

#endif /* ADVENTURE_H_ */
