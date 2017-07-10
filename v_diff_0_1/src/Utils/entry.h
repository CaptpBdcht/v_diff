#ifndef ENTRY_H
#define	ENTRY_H

typedef struct Entry {
    char* dir;
    char* value;
} Entry;

Entry* entry_new(char*, char*);
void entry_free(Entry**);

#endif	/* ENTRY_H */

