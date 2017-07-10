#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


/** HELP/VERSION **/
void display_help();
void display_version();
/** END H/V **/


/** ARGUMENTS **/
void display_long_arg_error(char*);
void display_short_arg_error(char);
void display_extra_operand_error(char*);
void display_ambiguous_param(char*);
void display_missing_operand_error(char *arg);
void display_file_error(char*);
void display_help_error(void);
/** END ARGS **/


/** OUTPUT STYLE **/
void v_display(v_Config*, int**, File*, File*, int, int);

// Classic style display v_diff
void display_usual(int**, File*, File*, int, int, Bool, Bool, Bool, int*, int*, v_stack*);

// Columns display v_diff
void display_side_side(int**, File*, File*, int, int);

// Unified display v_diff
void display_unified(int**, File*, File*, int, int, int, u_stack*);
int unified_get_last_line(int**, File*, File*, int, int, int);
void print_unified_diff(int**, File*, File*, int, int, int, u_stack*);

// Editing script display v_diff
void display_ed_script(int**, File*, File*, int, int, usint, Bool, int, Bool, ed_stack*);
/** END O_S **/


void printSpaces(int);

#endif // DISPLAY_H_INCLUDED
