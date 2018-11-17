/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - ex1. Pilots, Planes & Plans
Description - In this project we assign for each destination a plane with a set of pilots (captain and first officer), with minimum hours, qualified to fly the plane.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef AIRPLANE_DB_H_INCLUDED
	#define AIRPLANE_DB_H_INCLUDED
	#define AP_DB_SIZE 11
	#define MAX_PLANE_NAME 15

	typedef struct {
		char model[4];			// String of size 3 + '/0'
		char destinations[8][15];	// Pointer to char arrays
	} plane_model;

	typedef struct {
		char name[MAX_PLANE_NAME];
		char model[4];			// String of size 3 + '/0'
		float age;
		struct plane *next_plane;
	}plane;

	int GetAirplaneType(char *destination, plane_model ** plane_db_ptr);
	int CreateAirplaneList(plane **plane_head_ptr);
	int GetAirplane(char *model, plane *plane_head_ptr, plane **out_plane_ptr);
	void DeleteAirplane(plane **plane_lst, plane *del_plane);
	void ClearAirplanceList(plane *plane_lst);

	// AUX FUNCTIONS BELOW
	int getFreshPlane(char *dest, char *fresh_plane_name, char *fresh_plane_model, plane **plane_list);
#endif // closes #ifndef