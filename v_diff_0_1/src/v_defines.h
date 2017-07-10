#ifndef V_DEFINES_H_INCLUDED
#define V_DEFINES_H_INCLUDED


/*
 * Shortcut for types
 * * * * * * * * * * * * * * */
typedef unsigned long long int ullint;
typedef unsigned long int ulint;
typedef unsigned int uint;
typedef unsigned short int usint;


#ifdef WIN32
	#define strcasecmp(s1, s2) _stricmp(s1, s2)
	#define stat(filename, dest) _stat(filename, dest)
	typedef struct _stat STAT;
#elif defined (linux)
	typedef struct stat STAT;
#endif


/*
 * Bool def
 * * * * * * * * * * * * * * */
typedef unsigned char Bool;
#define TRUE 1
#define FALSE 0


/*
 * Exit Status
 * * * * * * * * * * * * * * */
#define EXIT_SAME 0
#define EXIT_DIFFERENT 1
#define EXIT_TROUBLE 2


#endif // V_DEFINES_H_INCLUDED

/*
 *  Type your comment here o/
 * * * * * * * * * * * * * * */
