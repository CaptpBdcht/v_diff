#include "../v_includes.h"

#define intMax(a, b) (a < b) ? b : a

/*
 * Main v_diff processing method.
 * Dispatch the execution based on detected file association during argument
 * parsing.
 * * * * * * * * * * * * * * */
usint process_diff(v_Config *config)
{
	switch(config->file_assoc)
	{
		case FILE_FILE:
			return process_files(config, config->targets[0], config->targets[1]);
		case FILE_DIR:
			return process_dir_file(config, TRUE);
		case DIR_FILE:
			return process_dir_file(config, FALSE);
		case DIR_DIR:
			return process_dirs(config, config->targets[0], config->targets[1]);

		default:
			display_help_error();
			return EXIT_TROUBLE;
	}
}

/*
 *  Processing two files.
 * * * * * * * * * * * * * * */
usint process_files(v_Config *config, char *srcpath, char *dstpath)
{
	File *src = NULL,
		 *dst = NULL;

	if((src = path_to_file(config, srcpath)) != NULL
	|| (config->b_absent_empty_files && (src = file_new(srcpath)) != NULL))
	{
		if((dst = path_to_file(config, dstpath)) != NULL
		|| (config->b_absent_empty_files && (dst = file_new(dstpath)) != NULL))
		{
			// If the 2 files are reached & initialized properly, we run
			// our diff algorithm on them before returning the diff status code
			Bool same = get_LCS(config, src, dst);

			file_free(&src);
			file_free(&dst);

			return (same ? EXIT_SAME : EXIT_DIFFERENT);
		}
		else
			file_free(&src);
	}

	return EXIT_TROUBLE;
}

/*
 *  Processing a DIR and a FILE.
 * * * * * * * * * * * * * * */
usint process_dir_file(v_Config *config, Bool swap)
{
	usint status = EXIT_TROUBLE;

	char* dir = config->targets[(swap ? 1 : 0)];
	char* file = config->targets[(swap ? 0 : 1)];
	char* remote = create_path(dir, file);

	//If 'file' doesn't exists in 'dir'
	if(!file_exists(remote))
		display_file_error(remote);
	//Process diff on files
	else
		status = (swap ? process_files(config, file, remote) : process_files(config, remote, file));

	free(remote);
	return status;
}

/*
 *  Processing two DIRS content.
 * * * * * * * * * * * * * * */
usint process_dirs(v_Config *config, char *srcpath, char *dstpath)
{
	usint status = EXIT_SAME;
	DIR *srcdir = NULL,
		*dstdir = NULL;

	if(!strcmp(srcpath, dstpath))
		return EXIT_SAME;

	if((srcdir = opendir(srcpath)) != NULL)
	{
		if((dstdir = opendir(dstpath)) != NULL)
		{
			uint i, j;
			Bool swap = TRUE;
			Array* entries = array_new();
			struct dirent* entry = NULL;

			//Read through DIR streams
			while((entry = readdir(srcdir)) != NULL)
			{
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
					continue;
				array_add(entries, entry_new(srcpath, entry->d_name), sizeof(Entry*), TRUE);
			}

			while((entry = readdir(dstdir)) != NULL)
			{
				if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
					continue;
				array_add(entries, entry_new(dstpath, entry->d_name), sizeof(Entry*), TRUE);
			}

			closedir(srcdir);
			closedir(dstdir);

			//Sort src & dst entries before processing it
			for(i = entries->size ; i > 0 && swap ; i--)
			{
				swap = FALSE;

				for(j = 1 ; j < i - 1 ; j++)
				{
					Entry* src_ent = (Entry*)array_get(entries, j);
					Entry* dst_ent = (Entry*)array_get(entries, j + 1);

					if(strcasecmp(src_ent->value, dst_ent->value) > 0)
					{
						array_swap(entries, j, j + 1);
						swap = TRUE;
					}
				}
			}

			//Loop over entries & diff
			j = entries->size - 1;
			for(i = 0 ; i < j ; i++)
			{
				Entry* src_ent = (Entry*)array_get(entries, i);
				Entry* dst_ent = (Entry*)array_get(entries, i + 1);
				int state = (config->b_case_file_names) ? strcmp(src_ent->value, dst_ent->value) : strcasecmp(src_ent->value, dst_ent->value);

				//Process...
				if(!state)
				{
					char* src_path = create_path(src_ent->dir, src_ent->value);
					Bool src_flags[2] = {is_dir(src_path), file_exists(src_path)};
					char* dst_path = create_path(dst_ent->dir, dst_ent->value);
					Bool dst_flags[2] = {is_dir(dst_path), file_exists(dst_path)};

					//If 2 dirs
					if(src_flags[0] && dst_flags[0])
					{
						//Recursive call to process_dirs
						if(config->b_recursive)
						{
							if(process_dirs(config, src_path, dst_path) == EXIT_DIFFERENT && status != EXIT_DIFFERENT)
								status = EXIT_DIFFERENT;
						}
						//Who cares anyway?
						else
							printf("Sub-directories %s and %s are identical\n", src_path, dst_path);
					}
					//If 1 dir / 1 file (or the other way round)
					else if((src_flags[0] && dst_flags[1]) || (src_flags[1] && dst_flags[0]))
						printf("File %s is a %s while file %s is a %s\n", src_path, (src_flags[0] ? "directory" : "regular file"), dst_path, (dst_flags[0] ? "directory" : "regular file"));
					//If 2 files
					else
					{
						printf("diff %s %s\n", src_path, dst_path);

						if(process_files(config, src_path, dst_path) == EXIT_DIFFERENT && status != EXIT_DIFFERENT)
							status = EXIT_DIFFERENT;
					}

					free(src_path);
					free(dst_path);

					//We ignore the second entry
					i++;
				}
				else
					printf("Only in %s: %s\n", src_ent->dir, src_ent->value);
			}

			// If there is a remaining entry, then it is only located in one DIR.
			if(i < entries->size)
			{
				Entry* e = (Entry*)array_get(entries, i);
				printf("Only in %s: %s\n", e->dir, e->value);
			}

			//Free entries
			for(i = 0 ; i < entries->size ; i++)
			{
				Entry* e = (Entry*)array_get(entries, i);
				entry_free(&e);
			}
			free(entries);
		}
		else
			closedir(srcdir);
	}

	return status;
}



/*
 *          get_LCS
 *
 * @config : gives options
 * @fsrc   : source file
 * @fdst   : destination file
 * @return : boolean fsrc equals fdst
 * * * * * * * * * * * * * * */
Bool get_LCS(v_Config * config, File * fsrc, File * fdst)
{
	Bool br = FALSE;
    int height = fsrc->hashtable->size + 1;     // First line will be 0s
    int length = fdst->hashtable->size + 1;     // First column too

    int** lcsVertex = malloc(height * sizeof(int *));
    int i, j;

    // Init left column with 0s during instanciation of tab
    for (i = 0; i < height; i++)
    {
        lcsVertex[i] = malloc(length * sizeof(int));
        lcsVertex[i][0] = 0;
    }

    // Then init first line with 0s
    for (j = 0; j < length; j++)
        lcsVertex[0][j] = 0;

    // Vertex creation
    // No steps needed on 1st line & column
    for (i = 1; i < height; i++)
    {
        for (j = 1; j < length; j++)
        {
            // Same line : value = 1 + upper-left value
            if (*(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
                lcsVertex[i][j] = lcsVertex[i-1][j-1] + 1;
            // Otherwise we take max of left and top
            else
                lcsVertex[i][j] = intMax(lcsVertex[i][j-1], lcsVertex[i-1][j]);
        }
    }

    // Check if same files
    if (lcsVertex[height-1][length-1] == height - 1 &&
        lcsVertex[height-1][length-1] == length - 1)
        br = TRUE;

	/* Display Output following the asked options */
	if(config->b_brief)
	{
		if(!br)
			printf("Files %s and %s differ\n", fsrc->name, fdst->name);
	}
	else if(config->b_identical_files && br)
		printf("Files %s and %s are identical\n", fsrc->name, fdst->name);
	else
		v_display(config, lcsVertex, fsrc, fdst, height-1, length-1);

    for (i = 0; i < height; i++)
        free(lcsVertex[i]);
    free(lcsVertex);

    return br;
}
