#include "v_includes.h"

/*
 * v_diff
 * C Project @ ESGI, 11/2015
 * 
 * Custom implementation of Linux/Unix diff core command.
 * * * * * * * * * * * * * * * * * * */
int main(int argc, char** argv)
{
	v_Config *config = NULL;
	usint exit_code;

	// Initialize the config
	config = config_create();

	if(!config_init(&config, argc, argv))
		return EXIT_TROUBLE;

	// Processing
	exit_code = process_diff(config);

	// Free & exit
	free(config);
	return exit_code;
}
