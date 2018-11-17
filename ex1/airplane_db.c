/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - ex1. Pilots, Planes & Plans
Description - In this project we assign for each destination a plane with a set of pilots (captain and first officer), with minimum hours, qualified to fly the plane.
*/
#include "airplane_db.h"

// Const Plane Model Data
#define PLANE_737_DATA  {"737",{"Larnaca","Athens","Budapest","Zurich","London","Paris","Rome"}}
#define PLANE_747_DATA  {"747",{"London","New York","Bangkok"}}
#define PLANE_787_DATA  {"787",{"London","New York","Los Angeles","Hong Kong","Miami"}}
#define AP_MODEL_DB_SIZE 3										// Size of plane model db

const plane_model plane_737 = PLANE_737_DATA;
const plane_model plane_747 = PLANE_747_DATA;
const plane_model plane_787 = PLANE_787_DATA;
const plane_model *plane_model_db[AP_MODEL_DB_SIZE] = { &plane_737, &plane_747,&plane_787 };
const int model_destinations[AP_MODEL_DB_SIZE] = { 7, 3, 5 };	// Holds number of destinations per model
int model_select[AP_MODEL_DB_SIZE] = { 0, 0, 0 };				// This a global score board used by GetAirplaneType function to return all optional db entries per destination

// Const Plane Fleet Data
const plane plane_db[AP_DB_SIZE] = {
	{ "Beait-Shean", "737", 5 , NULL },
	{ "Ashkelon", "737", 10.25, NULL },
	{ "Hadera", "737", 3, NULL },
	{ "Kineret", "737", 7.5, NULL },
	{ "Nahariya", "737", 1, NULL },
	{ "Tel-Aviv", "747", 20, NULL },
	{ "Haifa", "747", 15, NULL },
	{ "Jerusalem", "747", 17, NULL },
	{ "Ashdod", "787", 1, NULL },
	{ "Bat Yam", "787", 1.5, NULL },
	{ "Rehovot", "787", 0.5, NULL },
};

/*
Function GetAirplaneType
------------------------
Description – The function points to an etry in plane model db that contains the designated destination 
Parameters	– *destination points to the destination string, **plane_db_ptr is pointer to a pointer to the db entry (This is also the output),
				plane_model_db constant global db is accessed , model_select global scoreboard is checked in order to allow access to all model entries
				this is managed by calling function.
Returns		– 0 for success, -1 for failure
*/
int GetAirplaneType(char *destination, plane_model ** plane_db_ptr) {	
	int i; int j; int db_destination_length;
	if (*destination == NULL) {

		return -1;
	}
	for (i = 0; i < AP_MODEL_DB_SIZE; i++) {
		if (model_select[i] == 1) { // If plane model alread checked, continue to other models. Calling Function should reset the score board
			continue;
		}
		for (j = 0; j < model_destinations[i]; j++) {
			if (strcmp(destination, plane_model_db[i]->destinations[j]) == 0) {
				*plane_db_ptr = plane_model_db[i];					// Output the pointer to the db entry
				model_select[i] = 1;
				return 0;											// Found a model with input destination
			}
		}
	}
	
	return -1;	//Destination not found in plane db
}

/*
Function CreateAirplaneList
------------------------
Description – The function creates a linked list of available fleet planes. 
				The constant data is taken from plane_db array.
Parameters	– **plane_head_ptr is pointer to a pointer to the head of planes linked list (This is also the output) 
Returns		–  0 for success, -1 for failure
*/
int CreateAirplaneList(plane **plane_head_ptr) {
	// Handle null pointer input
	if (plane_head_ptr == NULL) {
		return (-1);
	}

	// Set first entry
	plane *new_plane,*prev_plane;
	new_plane = (plane*)malloc(sizeof(plane));
	if (NULL == new_plane)
	{
		return -1;
	}
	*new_plane = plane_db[0];
	*plane_head_ptr = new_plane;
	prev_plane = new_plane;
	// Set next entries in linked list
	int i;
	for (i = 1; i < AP_DB_SIZE; i++) {
		new_plane = (plane*)malloc(sizeof(plane));
		if (NULL == new_plane)
		{
			return -1;
		}
		*new_plane = plane_db[i];
		prev_plane->next_plane = new_plane;
		prev_plane = new_plane;
	}
	return 0;
}

/*
Function GetAirplane
------------------------
Description – The function outputs a pointer to the youngest plane in the fleet (in plane linked list)
				of the same model specified by the input model.				
Parameters	–  *model is the input model string, *plane_head_ptr is a pointer to the head of planes linked list, 
				**out_plane_ptr is a pointer to a pointer to an entry in plane linked list
Returns		– 0 for success, -1 for failure
*/
int GetAirplane(char *model, plane *plane_head_ptr, plane **out_plane_ptr) {
	plane *p_lst = plane_head_ptr;
	if (p_lst == NULL || model == NULL) // if list is empty or model name is empty
	{
		return -1;
	}
	int min_age = -1;	//For first itteration
	while (p_lst != NULL)	// Itterate on plane list
	{
		if (strcmp(model, p_lst->model) == 0) {
			// Handle first
			if (min_age == -1){
				*out_plane_ptr = p_lst;
				min_age = p_lst->age;
			}
			else {
				if (min_age > p_lst->age) {
					*out_plane_ptr = p_lst;
					min_age = p_lst->age;
				}

			}
		}
		p_lst = p_lst->next_plane;
	}
	if (min_age == -1) {
		// Input model name was wrong
		return -1;
	}
	else
		return 0;
}

/*
Function DeleteAirplane
------------------------
Description – The function deletes the entry in linked list (plane_lst) that is pointed by *del_plane.
Parameters	– **plane_lst is pointer to a pointer to the head of planes linked list (Input), 
				*del_plane is a pointer to the entry in linked list to be deleted (Input)
Returns		– Nothing
*/
void DeleteAirplane(plane **plane_lst, plane *del_plane) {
	plane *next_plane = NULL,*cur_plane = *plane_lst;
	if (cur_plane == NULL || del_plane == NULL) {//Empty list or no ptr to delete
		return;
	}
	if (cur_plane == del_plane) {	 //Entry to remove is at the head
		*plane_lst = cur_plane->next_plane;
		free(del_plane);
		return;
	}
	next_plane = cur_plane->next_plane;		//Entry to remove is maybe in the middel
	while (next_plane != NULL) {
		if (next_plane == del_plane) {	//Found entry to remove	
			cur_plane->next_plane = next_plane->next_plane;
			free(del_plane);
			return;
		}
		cur_plane = next_plane;
		next_plane = next_plane->next_plane;
	}
	return;
}

/*
Function ClearAirplanceList
------------------------
Description – The function free's the allocated memory used to create the airplane list
Parameters	– **plane_lst is pointer to a pointer to the head of planes linked list (Input) 
Returns		– Nothing
*/
void ClearAirplanceList(plane *plane_lst)
{
	plane *tmp_p;
	while (plane_lst != NULL)
	{
		tmp_p = plane_lst;
		plane_lst = plane_lst->next_plane;
		free(tmp_p);
	}
}

/*
Function getFreshPlane
------------------------
Description – The function gets the destination and finds the youngest plane that is qualified to fly to there.
Parameters	– *dest is destinations string (Input), *fresh_plane_name plane name string (Output), *fresh_plane_model plane model string (Output),
				**plane_list pointer to a pointer to the head of planes linked list (Input), the model_select scoreboard is used in order to access all
				data base model enries.
Returns		– 0 for success, -1 for failure
*/
int getFreshPlane(char *dest, char *fresh_plane_name, char *fresh_plane_model,plane **plane_list) {
	plane_model *model_ptr = NULL;
	plane *fresh_plane = NULL, *min_ptr=NULL;
	int i,plane_age = 10000;

	// Reset model scoreboard
	model_select[0] = 0; model_select[1] = 0; model_select[2] = 0;

	for (i = 0; i < AP_MODEL_DB_SIZE; i++) {
		model_ptr = NULL;
		if (GetAirplaneType(dest, &model_ptr) == 0) {
			// New model found in db
			// Check score board
			if (model_ptr == &plane_737)
				model_select[0] = 1;
			if (model_ptr == &plane_747)
				model_select[1] = 1;
			if (model_ptr == &plane_787)
				model_select[2] = 1;
			// Get youngest model from plane list
			if (GetAirplane(model_ptr->model, *plane_list, &fresh_plane) == 0) {// Plane model found in list
				if (fresh_plane->age < plane_age) {// Plan is youngest found
					plane_age = fresh_plane->age;
					strcpy(fresh_plane_name, fresh_plane->name);
					strcpy(fresh_plane_model, fresh_plane->model);
					min_ptr = fresh_plane;
				}

			}
		}
	}
	if (min_ptr != NULL) { // Youngest plane found and is now selected thus removed from list
		DeleteAirplane (plane_list, min_ptr);
		return 0;
	}
	else
		return -1;
}