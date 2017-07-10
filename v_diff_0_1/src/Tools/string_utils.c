#include "../v_includes.h"

/* -i */
void toLowerCase(char* str)
{
    uint i;
	
    for(i = 0 ; str[i] != '\0' ; i++)
    {
        if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
    }
}

/* -E */
void delTabExpansion(char* str)
{
    uint i, flag;
	
    for(i = 0, flag = 0 ; str[i] != '\0' ; i++)
    {
		if(flag)
			str[i] = str[i + flag];
		
		while(str[i] == '\t')
		{
			char c = str[i + 1 + flag++];
			str[i] = c;
		}
		
		if(str[i] == '\0')
			break;
    }
}

/* -Z */
void delWhiteSpaceEnd(char* str)
{
	uint i;
	
    while(str[(i = strlen(str) - 1)] == ' ')
		str[i] = '\0';
}

/* -b */
void delWhiteSpaceNotEnd(char* str)
{
    uint i, flag;
	
    for(i = 0, flag = 0 ; str[i] != '\0' ; i++)
    {
		if(flag)
			str[i] = str[i + flag];
		
		if(i > 0)
		{
			while(str[i] == ' ' && str[i - 1] == ' ')
			{
				char c = str[i + 1 + flag++];
				str[i] = c;
			}
		}
		
		if(str[i] == '\0')
			break;
    }
}

/* -w */
void delWhiteSpacesAll(char* str)
{
    uint i, flag;
	
    for(i = 0, flag = 0 ; str[i] != '\0' ; i++)
    {
		if(flag)
			str[i] = str[i + flag];
		
		while(str[i] == ' ')
		{
			char c = str[i + 1 + flag++];
			str[i] = c;
		}
		
		if(str[i] == '\0')
			break;
    }
}
