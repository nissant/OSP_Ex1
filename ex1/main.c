/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - ex1. Pilots, Planes & Plans
Description - In this project we assign for each destination a plane with a set of pilots (captain and first officer), with minimum hours, qualified to fly the plane.
*/

#include "airplane_db.h"
#include "pilots.h"

#define _CRT_SECURE_NO_WARNINGS
#define ERROR_MESSAGE "An error occurred during execution, couldn't complete the task!\n"

void main(int argc, char *argv[])
{
	// Check that minimal cmd line args are present
	if (argc < 4) {
		printf(ERROR_MESSAGE);
		exit(1);
	}

	// Set flight plan output file for writing
	FILE *fp_output = fopen(argv[3], "w");
	if (fp_output == NULL)							// Handle errors
	{
		printf(ERROR_MESSAGE);						// At this point error can't be written to file...
		exit(1);
	}

	// Get Airplane list
	plane *plane_list = NULL;
	if (CreateAirplaneList(&plane_list) != 0) {		// Handle errors
		fprintf(fp_output, ERROR_MESSAGE);
		fclose(fp_output);
		ClearAirplanceList(plane_list);
		exit(1);
	}

	// Get Pilot list
	pilot *pilots_list = NULL;						// used to create linked list
	if (GetPilots(argv[1], &pilots_list) != 0) {	// Handle errors
		fprintf(fp_output, ERROR_MESSAGE);		
		ClearPilotList(pilots_list);				// Free allocated mem	
		ClearAirplanceList(plane_list);
		fclose(fp_output);
		exit(1);
	}

	// Set destination file for reading
	FILE *fp_destinations;
	fp_destinations = fopen(argv[2], "r");
	if (fp_destinations == NULL)					// Handle errors
	{
		fprintf(fp_output, ERROR_MESSAGE);
		fclose(fp_output);
		ClearPilotList(pilots_list);				// Free allocated mem	
		ClearAirplanceList(plane_list);
		exit(1);
	}

	// Itterate over destinations file lines
	char fresh_plane_name[MAX_PLANE_NAME];
	char fresh_plane_model[4];
	char dest[MAX_LINE_LEN];
	char captain_name[MAX_LINE_LEN];
	char first_officer_name[MAX_LINE_LEN];
	char *temp;
	while (fgets(dest, MAX_LINE_LEN, fp_destinations))
	{
		temp = strchr(dest, '\n');
		if (temp != NULL) // A line with '\n' was found so overwrite it
			*temp = '\0';

		if (getFreshPlane(dest, fresh_plane_name, fresh_plane_model,&plane_list) != 0) {	//Handle wrong or no destination error
			freopen(argv[3], "w", fp_output);				//Clear existing file lines that may be written already
			fprintf(fp_output, ERROR_MESSAGE);
			break;
		}
		
		if (getFreshPilots(fresh_plane_model, captain_name, first_officer_name, &pilots_list) != 0) {	// Handle no pilots error
			freopen(argv[3], "w", fp_output);				//Clear existing file lines that may be written already
			fprintf(fp_output, ERROR_MESSAGE);
			break;
		}
		// At this point, no errors with finding fresh plane and pilots to destination - Write to flight plan
		fprintf(fp_output, "%s, %s, %s, %s \n", dest, fresh_plane_name, captain_name, first_officer_name);
	}

	ClearPilotList(pilots_list);				// Free allocated mem	
	ClearAirplanceList(plane_list);
	fclose(fp_destinations);					// Close files
	fclose(fp_output);
}



