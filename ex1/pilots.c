/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - ex1. Pilots, Planes & Plans
Description - In this project we assign for each destination a plane with a set of pilots (captain and first officer), with minimum hours, qualified to fly the plane.
*/

#include "pilots.h"


/*
ClearPilotList
------------------------
Description – The function receive pointer to the head of pilots list and free's allocated memory
Parameters	– *pilots_head - pointer to the head of the pilots list.
Returns		– Nothing
*/
void ClearPilotList(pilot *pilot_lst)
{

	pilot *tmp_p;
	while (pilot_lst != NULL) // while list is not empty
	{
		tmp_p = pilot_lst;
		pilot_lst = pilot_lst->nxt_pilot;
		free(tmp_p);
	}
}

/*
DeletePilots
------------------------
Description – The function deletes specific pilot from the pilots list
Parameters	– **head is a pointer to the head of the pilots list (allso the output). *p_pilot is a pointer to the pilot that needs to be deleted.
Returns		– nothing
*/
void DeletePilots(pilot **head, pilot *p_pilot){

	pilot *prev_p = NULL, *tmp_p = (*head);

	if (tmp_p == NULL || p_pilot == NULL)
		return;										// pilot list is empty
	if (tmp_p == p_pilot)							// if the pilot that needs to be deleted is the head of the list
	{
		*head = (*head)->nxt_pilot;
		free(p_pilot);
		return;
	}
	while (tmp_p != NULL)		//while list is not empty
	{
		if (tmp_p == p_pilot)   // if found the pilot
		{
			prev_p->nxt_pilot = tmp_p->nxt_pilot;
			free(p_pilot);
			break;
		}
		prev_p = tmp_p;
		tmp_p = tmp_p->nxt_pilot;
	}
}


/*
Function GetPilots
------------------------
Description – the function read lines from pilots.txt file and create a linked list of pilots.
Parameters	– *file_name is the name of the pilots.txt file. **pilot_lst is pointer to a pointer to the head of the pilot list that the function creates (allso the output).
Returns		– 0 for success, -1 for failure
*/
int GetPilots(char *file_name, pilot **pilot_lst)
{
	FILE *fp_pilots=NULL;
	pilot *tmp_p=NULL;
	char line[MAX_LINE_LEN];
	fp_pilots = fopen(file_name, "r");
	if (fp_pilots == NULL)			// if failed to open file
	{
		return -1;
	}
	while (fgets(line, MAX_LINE_LEN, fp_pilots)) // read line from file
	{
		tmp_p = MakePilotFromLine(line);        // create a pilot struct from the line
		if (tmp_p == NULL)
			return -1;
		AddPilotToList(pilot_lst, tmp_p);		//add pilot to the pilots list
	}
	fclose(fp_pilots);
	return 0;
}


/*
Function MakePilotFromLine
------------------------
Description – the function gets a string containing pilot data and creates a pilots strcut.
Parameters	– *line is a string which includes pilot data.
Returns		– Nothing
*/
pilot *MakePilotFromLine(char *line)
{
	char c, *tmp_p = line, *location;
	pilot *new_pilot;
	new_pilot = (pilot*)malloc(sizeof(pilot));
	if (NULL ==new_pilot)
	{
		
		return new_pilot;
	}
	location = strchr(tmp_p, ',');
	*location = '\0';
	strcpy(new_pilot->name, tmp_p);		//copy pilots name from line to the pilot strcut;

	tmp_p = location + 2;
	location = strchr(tmp_p, ',');
	*location = '\0';
	strcpy(new_pilot->qual_plane, tmp_p);

	tmp_p = location + 2;
	location = strchr(tmp_p, ',');
	*location = '\0';
	new_pilot->hours = atoi(tmp_p);

	tmp_p = location + 2;
	if (*tmp_p == 'C' || *tmp_p == 'c')		// if rank is captain
		*(tmp_p + 7) = '\0';
	else									// first officer
		*(tmp_p + 13) = '\0';
	strcpy(new_pilot->rank, tmp_p);

	new_pilot->nxt_pilot = NULL;
	return new_pilot;
}

/*
Function AddPilotToList
------------------------
Description – The function adds a new pilot to the pilots linked list
Parameters	– **pilot_lst is pointer to a pointer to the head of the pilot list (Is output if list is empty and just created),
				*new_pilot is a pointer to the new pilot node that is to be added to the linked list
Returns		– Nothing
*/
void AddPilotToList(pilot **pilot_lst, pilot *new_pilot)
{
	pilot *temp_p = *pilot_lst;
	if (temp_p == NULL) // if list is empty
	{
		*pilot_lst = new_pilot;
		return;
	}
	while (temp_p->nxt_pilot != NULL)
	{
		temp_p = temp_p->nxt_pilot; //move pointer to the last pilot in the list
	}
	temp_p->nxt_pilot = new_pilot;
}

/*
Function getFreshPilots
------------------------
Description – the function gets a model of a plane and finds a 1st officer and captain with minum hour qualified to fly the plane.
Parameters	– *model is a string of the plane model needs to be blow. *captain_name and *first_officer are strings that are filled with the name of the chosen pilots.
				they serve as the outputs. **pilots_list is the list of pilots. it is also the output as the list is being updated (pilots deleted).
Returns		– 0 for success , -1 for failure
*/
int getFreshPilots(char *model, char *captain_name, char *first_officer_name, pilot **pilots_list) {
	int got_cptn = 0, got_fo = 0, min_h_cptn = 1400000, min_h_fo = 1400000;
	pilot *cptn, *fo, *tmp_list;
	tmp_list = *pilots_list;
	while (tmp_list!=NULL) // while there are still piltos in the list
	{
		if (0 == strcmp(tmp_list->rank, "Captain"))		// if the current pilot in list is captain
		{
			if ((tmp_list->hours < min_h_cptn) && (0 == strcmp(model, tmp_list->qual_plane))) // check if the pilots is qualified and with minum hours.
			{
				min_h_cptn = tmp_list->hours;
				cptn = tmp_list;
				got_cptn = 1;		// used as flag. if 1, found a captain.

			}
		}
		if (0 == strcmp(tmp_list->rank, "First Officer"))	// if the current pilot in list is first officer
		{
			if ((tmp_list->hours < min_h_fo) && (0 == strcmp(model, tmp_list->qual_plane))) // check if the pilots is qualified and with minum hours.
			{
				min_h_fo = tmp_list->hours;
				fo = tmp_list;
				got_fo = 1;			// used as flag. if 1, found a first officer.

			}
		}
		tmp_list = tmp_list->nxt_pilot;	// itirate over the pilots in list.
	}
	if (got_cptn && got_fo) // if found both captain and fist officer
	{
		strcpy(captain_name, cptn->name);		// update the output string with pilots name
		strcpy(first_officer_name, fo->name);
		DeletePilots(pilots_list, cptn);		// delete pilots from list.
		DeletePilots(pilots_list, fo);
		return 0;
	}
	else
		return -1;
}

