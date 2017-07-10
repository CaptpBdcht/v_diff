#include "../v_includes.h"

/*
 * Creates & initializes a new Entry structure.
 * * * * * * * * * * * * * * * * * * */
Entry* entry_new(char* dir, char* value)
{
	Entry* entry = (Entry*)malloc(sizeof(Entry));

	if(entry)
	{
		char* tmp = malloc((strlen(value) + 1) * sizeof(char));
		strcpy(tmp, value);

		entry->dir = dir;
		entry->value = tmp;
	}

	return entry;
}

/*
 * Free of the given Entry content and pointer.
 * * * * * * * * * * * * * * * * * * */
void entry_free(Entry** entry)
{
	free((*entry)->value);
	free(*entry);
	*entry = NULL;
}
