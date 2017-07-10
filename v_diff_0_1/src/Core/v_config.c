#include "../v_includes.h"

/*
 * Allocate a new v_Config structure
 * and initializes its members with default values.
 * * * * * * * * * * * * * * * * * * */
v_Config* config_create(void)
{
    v_Config* conf = (v_Config*)malloc(sizeof(v_Config));

	if(conf)
	{
		/** File assoc **/
		conf->file_assoc = ERR;

		/** Targets **/
		conf->targets_count = 0;
		conf->targets[0] = NULL;
		conf->targets[1] = NULL;

		/** Diff common uses **/
		conf->b_normal            = FALSE;
		conf->b_brief             = FALSE;
		conf->b_identical_files   = FALSE;

		/** INLINE **/
		conf->b_ignore_case           = FALSE;
		conf->b_ignore_tab_expansion  = FALSE;
		conf->b_ignore_trailing_space = FALSE;
		conf->b_ignore_space_change   = FALSE;
		conf->b_ignore_white_space    = FALSE;
		conf->b_ignore_blank_lines    = FALSE;
		/** END INLINE **/

		/** FILE/DIR **/
		conf->b_absent_empty_files    = FALSE;
		conf->b_case_file_names       = TRUE;
		conf->b_recursive             = FALSE;
		/** END FILE/DIR **/

		/** OUTPUT **/
		conf->b_copied_context       = FALSE;
		conf->lines_copied_context   = 3;

		conf->b_unified_context      = FALSE;
		conf->lines_unified_context  = 3;

		conf->b_output_two_columns   = FALSE;
		conf->b_output_ed_script     = FALSE;
		/** END OUTPUT **/

		/** HASH **/
		conf->hash = DJB2;
	}

    return conf;
}

/*
 * Initialize the config with the command line
 * content.
 * We first look for version or help flag, and then
 * parse the arguments and check the file association type.
 * * * * * * * * * * * * * * * * * * */
Bool config_init(v_Config** conf, int argc, char** argv)
{
	Bool flag;

	find_version(conf, argc, argv);
	find_help(conf, argc, argv);

	if((flag = parse_args(conf, argc, argv)))
	{
		if((*conf)->targets_count < 2)
		{
			display_missing_operand_error((*conf)->targets[0] != NULL ? (*conf)->targets[0] : argv[argc-1]);
			return FALSE;
		}
		else
			flag = get_file_assoc(conf);
	}

	return flag;
}

/*
 * Check for the version flag.
 * If it is present, display the version information and exit.
 * * * * * * * * * * * * * * * * * * */
void find_version(v_Config** conf, int argc, char** argv)
{
	int i;

	for(i = 1 ; i < argc ; i++)
	{
		if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
		{
			display_version();
			free(*conf);
			exit(EXIT_SUCCESS);
		}
	}
}

/*
 * Check for the help flag.
 * If it is present, display the help and exit.
 * * * * * * * * * * * * * * * * * * */
void find_help(v_Config** conf, int argc, char** argv)
{
	int i;

	for(i = 1 ; i < argc ; i++)
	{
		if(!strcmp(argv[i], "--help"))
		{
			display_help();
			free(*conf);
			exit(EXIT_SUCCESS);
		}
	}
}

/*
 * Parse the command line content to update the config content.
 * * * * * * * * * * * * * * * * * * */
Bool parse_args(v_Config** conf, int argc, char** argv)
{
	int i, j;

	for(i = 1 ; i < argc ; i++)
	{
		int length = strlen(argv[i]);

		if(length >= 2 && argv[i][0] == '-')
		{
			//If long arg: --...
			if(argv[i][1] == '-' && length > 2)
			{
				if(!check_long_arg(conf, argv[i]))
					return FALSE;
			}
			//If short arg
			else
			{
				//For each char...
				for(j = 1 ; j < length ; j++)
				{
					if(!check_short_arg(conf, &i, argc, argv, argv[i][j]))
						return FALSE;
				}
			}
		}
		//Potential file/dir
		else
		{
			if((*conf)->targets_count < 2)
				(*conf)->targets[(*conf)->targets_count++] = argv[i];
			else
			{
				display_extra_operand_error(argv[i]);
				return FALSE;
			}
		}
	}

	return TRUE;
}

/*
 * Check for short arg correspondance.
 * * * * * * * * * * * * * * * * * * */
Bool check_short_arg(v_Config** conf, int* i, int argc, char** argv, char c)
{
	switch(c)
	{
		case 'v':
			//Ignore version flag
			break;
		case 'q':
			(*conf)->b_brief = TRUE;
			break;
		case 's':
			(*conf)->b_identical_files = TRUE;
			break;
		case 'i':
			(*conf)->b_ignore_case = TRUE;
			break;
		case 'E':
			(*conf)->b_ignore_tab_expansion = TRUE;
			break;
		case 'Z':
			(*conf)->b_ignore_trailing_space = TRUE;
			break;
		case 'b':
			(*conf)->b_ignore_space_change = TRUE;
			break;
		case 'w':
			(*conf)->b_ignore_white_space = TRUE;
			break;
		case 'B':
			(*conf)->b_ignore_blank_lines = TRUE;
			break;
		case 'N':
			(*conf)->b_absent_empty_files = TRUE;
			break;
		case 'r':
			(*conf)->b_recursive = TRUE;
			break;
		case 'c':
		case 'C':
			if(*i < argc - 1)
			{
				(*conf)->b_copied_context = TRUE;
				(*conf)->lines_copied_context = atoi(argv[++(*i)]);
			}
			break;
		case 'u':
		case 'U':
			if(*i < argc - 1)
			{
				(*conf)->b_unified_context = TRUE;
				(*conf)->lines_unified_context = atoi(argv[++(*i)]);
			}
			break;
		case 'y':
			(*conf)->b_output_two_columns = TRUE;
			break;
		case 'e':
			(*conf)->b_output_ed_script = TRUE;
			break;

		default:
			display_short_arg_error(c);
			return FALSE;
	}

	return TRUE;
}

/*
 * Check for long arg correspondance.
 * * * * * * * * * * * * * * * * * * */
Bool check_long_arg(v_Config** conf, char* arg)
{
	Bool flag = TRUE;
	String* str = string_new(arg);
	
	// Initialize the possible ambiguous arguments.
	String* ambig[] = {
		string_new("--re"),
		string_new("--ignore-")
	};

	// If the given input is ambiguous, display an error.
	if(string_exists(ambig[0], str) != -1 || string_exists(ambig[1], str) != -1)
	{
		display_ambiguous_param(arg);
		flag = FALSE;
	}
	else
	{
		if(strcmp(arg, "--help") == 0 || strcmp(arg, "--version") == 0)
		{
			//Ignore version & help
		}
		else if(strcmp(arg, "--normal") == 0)
		{
			(*conf)->b_normal = TRUE;
		}
		else if(strcmp(arg, "--brief") == 0)
		{
			(*conf)->b_brief = TRUE;
		}
		else if(strcmp(arg, "--ignore-case") == 0)
		{
			(*conf)->b_ignore_case = TRUE;
		}
		else if(strcmp(arg, "--ignore-tab-expansion") == 0)
		{
			(*conf)->b_ignore_tab_expansion = TRUE;
		}
		else if(strcmp(arg, "--ignore-trailing-space") == 0)
		{
			(*conf)->b_ignore_trailing_space = TRUE;
		}
		else if(strcmp(arg, "--ignore-space-change") == 0)
		{
			(*conf)->b_ignore_space_change = TRUE;
		}
		else if(strcmp(arg, "--ignore-all-space") == 0)
		{
			(*conf)->b_ignore_white_space = TRUE;
		}
		else if(strcmp(arg, "--ignore-blank-lines") == 0)
		{
			(*conf)->b_ignore_blank_lines = TRUE;
		}
		else if(strcmp(arg, "--new-file") == 0)
		{
			(*conf)->b_absent_empty_files = TRUE;
		}
		else if(strcmp(arg, "--ignore-file-name-case") == 0)
		{
			(*conf)->b_case_file_names = FALSE;
		}
		else if(strcmp(arg, "--no-ignore-file-name-case") == 0)
		{
			(*conf)->b_case_file_names = TRUE;
		}
		else if(strcmp(arg, "--report-identical-files") == 0)
		{
			(*conf)->b_identical_files = TRUE;
		}
		else if(strcmp(arg, "--recursive") == 0)
		{
			(*conf)->b_recursive = TRUE;
		}
		//Context or Unified
		else if(strcmp(arg, "--context") == 0)
		{
			(*conf)->b_copied_context = TRUE;
			(*conf)->lines_copied_context = 3;
		}
		else if(strcmp(arg, "--unified") == 0)
		{
			(*conf)->b_unified_context = TRUE;
			(*conf)->lines_unified_context = 3;
		}
		else if(arg[9] == '=')
		{
			// Explode on the '=' char to separate argument name and value
			String* name = str->sub(str, 0, 8);
			String* value = str->sub(str, 10, str->length - 1);
			char* val = value->toc(value);

			if(name->ccompare(name, "--context"))
			{
				(*conf)->b_copied_context = TRUE;
				(*conf)->lines_copied_context = atoi(val);
			}
			else if(name->ccompare(name, "--unified"))
			{
				(*conf)->b_unified_context = TRUE;
				(*conf)->lines_unified_context = atoi(val);
			}
			else
				flag = FALSE;

			free(val);
			value->free(value);
			name->free(name);
		}
		else if(strcmp(arg, "--side-by-side") == 0)
		{
			(*conf)->b_output_two_columns = TRUE;
		}
		else if(strcmp(arg, "--ed") == 0)
		{
			(*conf)->b_output_ed_script = TRUE;
		}
		//Hash
		else if(strcmp(arg, "--hash") == 0)
		{
			(*conf)->hash = DJB2;
		}
		else if(arg[6] == '=')
		{
			String* name = str->sub(str, 0, 5);
			String* value = str->sub(str, 7, str->length - 1);

			if(name->ccompare(name, "--hash"))
			{
				if(value->ccompare(value, "djb2"))
					(*conf)->hash = DJB2;
				else if(value->ccompare(value, "djb2a"))
					(*conf)->hash = DJB2A;
				else if(value->ccompare(value, "sdbm"))
					(*conf)->hash = SDBM;
			}
			else
				flag = FALSE;

			value->free(value);
			name->free(name);
		}
		else
			flag = FALSE;

		if(!flag)
			display_long_arg_error(arg);
	}

	ambig[0]->free(ambig[0]);
	ambig[1]->free(ambig[1]);
	str->free(str);
	return flag;
}

/*
 * Returns the detected file association
 * between our two targets.
 * * * * * * * * * * * * * * * * * * */
Bool get_file_assoc(v_Config **conf)
{
    if(is_dir((*conf)->targets[0]))
	{
		if(is_dir((*conf)->targets[1]))
			(*conf)->file_assoc = DIR_DIR;
		else if(file_exists((*conf)->targets[1]) || (*conf)->b_absent_empty_files)
			(*conf)->file_assoc = DIR_FILE;
		else
			display_file_error((*conf)->targets[1]);
	}
	else if(file_exists((*conf)->targets[0]))
	{
		if(is_dir((*conf)->targets[1]))
			(*conf)->file_assoc = FILE_DIR;
		else if(file_exists((*conf)->targets[1]) || (*conf)->b_absent_empty_files)
			(*conf)->file_assoc = FILE_FILE;
		else
			display_file_error((*conf)->targets[1]);
	}
	else
        display_file_error((*conf)->targets[0]);

    return ((*conf)->file_assoc == ERR) ? FALSE : TRUE;
}
