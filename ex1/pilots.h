/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - ex1. Pilots, Planes & Plans
Description - In this project we assign for each destination a plane with a set of pilots (captain and first officer), with minimum hours, qualified to fly the plane.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef PILOTS_H_INCLUDED
	#define PILOTS_H_INCLUDED
	#define MAX_LINE_LEN 100

	typedef struct pilot
	{
		char name[MAX_LINE_LEN]; //pilot's name
		char qual_plane[4]; //planes the pilot qualified to fly. 3 chars for plane code (747 for example) and another char for '\0'
		int hours; // pilot's flight hours
		char rank[14]; // "First Officer" or "Captain" - size 14 is to accomodate "First Officer" + '\0'
		struct pilot *nxt_pilot; // pointer to next pilot in the list. 
	}pilot;

	void ClearPilotList(pilot *pilot_lst);
	void DeletePilots(pilot **head, pilot *p_pilot);
	int GetPilots(char *file_name, pilot **pilot_lst);

	// AUX FUNCTIONS BELOW
	pilot *MakePilotFromLine(char *line);
	void AddPilotToList(pilot **pilot_lst, pilot *new_pilot);
	int getFreshPilots(char *model, char *captain_name, char *first_officer_name, pilot **pilots_list);

#endif // closes #ifndef