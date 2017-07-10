#ifndef STRING_UTILS_H_INCLUDED
#define STRING_UTILS_H_INCLUDED

// Get a String in lower case
// -i
void toLowerCase(char*);

// Delete tab expansions from a String
// -E
void delTabExpansion(char*);

// Delete trailing spaces
// -Z
void delWhiteSpaceEnd(char*);

// Delete spaces except trailing ones
// -b
void delWhiteSpaceNotEnd(char*);

// Delete all spaces
// -w
void delWhiteSpacesAll(char*);

#endif // STRING_UTILS_H_INCLUDED
