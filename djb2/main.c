#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long djb2(unsigned char *);
void printDjb2(unsigned char *, char *, unsigned char *, char *);

int main(int argc, char ** argv)
{
    unsigned char * str = malloc(255), * str2 = malloc(255);

    printDjb2(str, "a",
              str2, "A");

    printDjb2(str, "abc",
              str2, "ABC");

    printDjb2(str, "Lorem Ipsum Dolor Sit Amet",
              str2, "Lorem Ipsum Dolor Sit Ame");

    return 0;
}

unsigned long djb2(unsigned char * str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void printDjb2(unsigned char * str1, char * ph1, unsigned char * str2, char * ph2)
{
    unsigned long ul1, ul2;

    strcpy(str1, ph1);
    strcpy(str2, ph2);

    ul1 = djb2(str1);
    ul2 = djb2(str2);

    printf("/*************** TEST ***************/\n");
    printf("Str1     : %s\nHashVal  : %lu\n", str1, ul1);
    printf(" - - - - -                            \n");
    printf("Str2     : %s\nHashVal  : %lu\n", str2, ul2);
    printf("/************** END TS **************/\n\n");
}
