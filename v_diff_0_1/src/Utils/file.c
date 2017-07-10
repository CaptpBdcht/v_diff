#include "../v_includes.h"

/*
 * Creates & initializes a new File structure.
 * * * * * * * * * * * * * * * * * * */
File* file_new(char* name)
{
	File* file = (File*)malloc(sizeof(File));

	if(file != NULL)
	{
		file->name = name;
		file->modtime = NULL;

		file->data = array_new();
		file->hashtable = array_new();

		file->trailing_line = FALSE;
	}

	return file;
}

/*
 * Initialize structure content from the given FILE* stream.
 * * * * * * * * * * * * * * * * * * */
void file_init(v_Config* config, File* file, FILE* stream)
{
	int c;
	STAT buf;
	String* str = string_new(NULL);

	// Init modification time
	if(!stat(file->name, &buf))
	{
		if((file->modtime = ctime(&buf.st_mtime)) != NULL)
		{
			// Remove trailing \n
			file->modtime[strlen(file->modtime) - 1] = '\0';
		}
	}

	// Init data & hastable
	while((c = fgetc(stream)) != EOF)
	{
		// Save line data
		if(c == '\n' && (!config->b_ignore_blank_lines || (config->b_ignore_blank_lines && str->length > 0)))
		{
			file_append(config, file, str);
			str->clear(str);
		}
		// Append char to tmp String
		else if(c != '\r')
			str->add(str, c);
	}

	// If there is still some content in our buffer, then there's no new line at the end of the file
	if(str->length > 0)
	{
		file_append(config, file, str);
		file->trailing_line = FALSE;
	}
	else
		file->trailing_line = TRUE;

	str->free(str);
}

/*
 * Append the String buffer to the File content.
 * * * * * * * * * * * * * * * * * * */
void file_append(v_Config* config, File* file, String* str)
{
	char* raw = str->toc(str);		// Raw line, unmodified to be stored in data array
	char* line = str->toc(str);		// Line that can be modified before hash algorithm
	ullint hash;

	// Process line content based on passed arguments
	if(config->b_ignore_case)
		toLowerCase(line);
	if(config->b_ignore_tab_expansion)
		delTabExpansion(line);
	if(config->b_ignore_trailing_space)
		delWhiteSpaceEnd(line);
	if(config->b_ignore_space_change)
		delWhiteSpaceNotEnd(line);
	if(config->b_ignore_white_space)
		delWhiteSpacesAll(line);

	// Hash the processed line content
	switch(config->hash)
	{
		case DJB2A: hash = djb2a((const unsigned char*)line); break;
		case SDBM:	hash = sdbm((const unsigned char*)line);	break;
		case DJB2:
		default:	hash = djb2((const unsigned char*)line);
	}

	file->data->add(file->data, raw, str->length+1, TRUE);
	file->hashtable->add(file->hashtable, &hash, sizeof(ullint), FALSE);

	free(line); // Free hashable line
}


/*
 * Free the File data structure and its content.
 * * * * * * * * * * * * * * * * * * */
void file_free(File** file)
{
	(*file)->data->free((*file)->data);
	(*file)->hashtable->free((*file)->hashtable);
	free(*file);
}

/*
 * Returns a new File structure based on given path content.
 * * * * * * * * * * * * * * * * * * */
File* path_to_file(v_Config* config, char* path)
{
	FILE* src;

	if((src = fopen(path, "rb")) != NULL)
	{
		File* file = file_new(path);
		file_init(config, file, src);

		fclose(src);
		return file;
	}
	// If absent files must be trated as empty ones, do not print any error message
	else if(!config->b_absent_empty_files)
		fprintf(stderr, "Unable to open %s\n", path);

	return NULL;
}

/*
 * Returns TRUE if the given path is a directory.
 * Otherwise, returns FALSE.
 * * * * * * * * * * * * * * * * * * */
Bool is_dir(const char* path)
{
	DIR* dir;
	Bool flag = FALSE;

	if((dir = opendir(path)) != NULL)
	{
		flag = TRUE;
		closedir(dir);
	}

	return flag;
}

/*
 * Returns TRUE if the given path is a regular file.
 * Otherwise, returns FALSE.
 * * * * * * * * * * * * * * * * * * */
Bool file_exists(const char* path)
{
	FILE* file;
	Bool flag = FALSE;

	if((file = fopen(path, "rb")) != NULL)
	{
		flag = TRUE;
		fclose(file);
	}

	return flag;
}

/*
 * Returns the size of the file at given path.
 * * * * * * * * * * * * * * * * * * */
uint file_size(const char* path)
{
    uint size = 0;

    // If we have a real file that exists
    if (!is_dir(path) && file_exists(path))
    {
        FILE * file;

        if ((file = fopen(path, "rb")) != NULL)
        {
            fseek(file, 0L, SEEK_END);
            size = ftell(file);
            rewind(file);

            fclose(file);
        }
    }

    return size;
}

/*
 * Returns a path forged with the two arguments.
 * * * * * * * * * * * * * * * * * * */
char* create_path(const char* base, const char* name)
{
	uint base_len = strlen(base);
	uint name_len = strlen(name);
	usint trailing = base[base_len - 1] == '/' ? 1 : 0;
	char* path = malloc((base_len - trailing + name_len + 2) * sizeof(char));

	sprintf(path, (trailing ? "%s%s" : "%s/%s"), base, name);

	return path;
}
