#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED


/*
 *   struct v_stack
 *
 * Used for 'normal' output style
 * @srcLines int* : source line flags
 * @flag     char : 'c'/'d'/'a'
 * @dstLines int* : dest. line flags
 * * * * * * * * * * * * * * */
typedef struct v_stack {
    struct v_stack* next;
    int* srcLines;
    char flag;
    int* dstLines;
} v_stack;

void v_stack_push(v_stack**, int*, char, int*);
void v_stack_pop(v_stack**, File*, File*);
void v_stack_empty(v_stack**, File*, File*);


/*
 *   struct ed_stack
 *
 * Used for ed script output style
 * @content char* : a printable line
 * * * * * * * * * * * * * * */
typedef struct ed_stack {
    struct ed_stack* next;
    char* content;
} ed_stack;

void stack_push(ed_stack**, char*);
void stack_pop(ed_stack**);
void stack_empty(ed_stack**);


/*
 *   struct u_stack
 *
 * Used for unified v_diff output style
 * @flag    char  : ' '/'+'/'-'
 * @content char* : a printable line
 * * * * * * * * * * * * * * */
typedef struct u_stack {
    struct u_stack* next;
    char flag;
    char* content;
} u_stack;

void u_stack_push(u_stack**, char, char*);
void u_stack_pop(u_stack**);
void u_stack_empty(u_stack**);

#endif // UTILS_H_INCLUDED
