#ifndef FILE_STREAM_H
#define	FILE_STREAM_H

/*
 * File data structure
 * * * * * * * * * * * * * * */
typedef struct File {
	char* name;
	char* modtime;

	Array* data;
	Array* hashtable;

	Bool trailing_line;
} File;

File* file_new(char*);
void file_init(v_Config*, File*, FILE*);
void file_append(v_Config*, File*, String*);
void file_free(File**);
File* path_to_file(v_Config*, char*);

/*
 * File utils
 * * * * * * * * * * * * * * */
Bool is_dir(const char*);
Bool file_exists(const char*);
uint file_size(const char*);
char* create_path(const char*, const char*);

#endif	/* FILE_STREAM_H */

