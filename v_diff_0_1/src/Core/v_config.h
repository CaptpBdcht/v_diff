#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/*
 * File association enumeration.
 * * * * * * * * * * * * * * * * * * */
typedef enum {
    FILE_FILE,
    FILE_DIR,
    DIR_FILE,
    DIR_DIR,
    ERR
} FILE_ASSOC;

/*
 * Supported hash types.
 * * * * * * * * * * * * * * * * * * */
typedef enum {
    DJB2,
    DJB2A,
    SDBM
} HASH_TYPE;

/*
 * Main v_diff Config structure.
 * It encapsulates the different argument flags and values
 * passed through the argument line that called the program.
 * * * * * * * * * * * * * * * * * * */
typedef struct v_config {

    /*
    *  File Association for diff
    *      0 - File/File
    *      1 - File/Folder
    *      2 - Folder/File
    *      3 - Folder/Folder
    * * * * * * * * * * * * * * * * * * * */
    FILE_ASSOC file_assoc;

    /*
     * Raw targets path
     * * * * * * * * * * * * * * * * * * */
    usint targets_count;
    char* targets[2];


    /********* BEGIN HELP/VERSION *********/
    /*
    *   --help
    *  Display this help and exit
    *
    *   -v, --version
    *  Output version information and exit
    */
    /********** END HELP/VERSION **********/


    /*
    *   --normal
    *  Output normal v_diff
    * * * * * * * * * * * * * * * * * * * */
    Bool b_normal;

    /*
    *   -q, --brief
    *  Report when files differ
    * * * * * * * * * * * * * * * * * * * */
    Bool b_brief;

    /*
    *   -s, --report-identical-files
    *  Report when two files are the same
    * * * * * * * * * * * * * * * * * * * */
    Bool b_identical_files;

    /************ BEGIN INLINE ************/
    /*
    *   -i, --ignore-case
    *  Ignore case differences in
    *  file contents
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_case;

    /*
    *   -E, --ignore-tab-expansion
    *  Ignore changes due to tab expansion
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_tab_expansion;

    /*
    *   -Z, --ignore-trailing-space
    *  Ignore white space at line end
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_trailing_space;

    /*
    *   -b, --ignore-space-change
    *  Ignore changes in the amount of
    *  white space
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_space_change;

    /*
    *   -w, --ignore-all-space
    *  Ignore all white space
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_white_space;

    /*
    *   -B, --ignore-blank-lines
    *  Ignore changes whose lines are all
    *  blank
    * * * * * * * * * * * * * * * * * * * */
    Bool b_ignore_blank_lines;
    /************* END INLINE *************/


    /*********** BEGIN FILE/DIR ***********/
    /*
    *   -N, --new-file
    *  Treat absent files as empty
    * * * * * * * * * * * * * * * * * * * */
    Bool b_absent_empty_files;

    /*
    *   --ignore-file-name-case
    *  Ignore case when comparing file names
    *
    *   --no-ignore-file-name-case
    *  Consider case when comparing file
    *  names
    * * * * * * * * * * * * * * * * * * * */
    Bool b_case_file_names;

    /*
    *   -r, --recursive
    *  Recursively compare any
    *  subdirectories found
    * * * * * * * * * * * * * * * * * * * */
    Bool b_recursive;
    /************ END FILE/DIR ************/


    /************ BEGIN OUTPUT ************/
    /*
    *   -c, -C NUM, --context[=NUM]
    *  Output NUM (default 3) lines of
    *  copied context
    * * * * * * * * * * * * * * * * * * * */
    Bool b_copied_context;
    int lines_copied_context;

    /*
    *   -u, -U NUM, --unified[=NUM]
    *  Output NUM (default 3) lines of
    *  unified context
    * * * * * * * * * * * * * * * * * * * */
    Bool b_unified_context;
    int lines_unified_context;

    /*
    *   -y, --side-by-side
    *  Output in two columns
    * * * * * * * * * * * * * * * * * * * */
    Bool b_output_two_columns;

    /*
    *   -e, --ed
    *  Output an ed script
    * * * * * * * * * * * * * * * * * * * */
    Bool b_output_ed_script;
    /************* END OUTPUT *************/

	/*
    *   --hash=<str>
    *  Hash type
    * * * * * * * * * * * * * * * * * * * */
	HASH_TYPE hash;

} v_Config;


/****************************************
    Initializing the Configuration
   directly from the arguments received
   from the user.
****************************************/
v_Config* config_create(void);
Bool config_init(v_Config**, int, char**);

void find_version(v_Config**, int, char**);
void find_help(v_Config**, int, char**);

Bool parse_args(v_Config**, int, char**);
Bool check_short_arg(v_Config**, int*, int, char**, char);
Bool check_long_arg(v_Config**, char*);

Bool get_file_assoc(v_Config**);

#endif // CONFIG_H_INCLUDED
