#include "../v_includes.h"

/*
 * Originals (and full) results of the array presented below can be found at
 * http://programmers.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed.
 *
 * These results are presented in order to offer a preview of the
 * use of these hash, their low-collisions levels and the ease and
 * speed of hashing.
 *
 * Hash are used in v_diff in order to allow memory time to proceed
 * LCS, which is faster than stream processing, especially when you
 * have huge files.
 *
 * * * With a list of 216,553 English words * * *
 * Hash      Lowercase
 * ========  =============
 * DBJ2a     158 ns
 *             5 collis
 * DJB2      156 ns
 *             7 collis
 * SDBM      148 ns
 *             4 collis
 */

ullint djb2(const unsigned char* str)
{
	ullint hash = 5381;
    int c;

    while((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

ullint djb2a(const unsigned char* str)
{
	ullint hash = 5381;
    int c;

    while((c = *str++) != '\0')
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */

    return hash;
}

ullint sdbm(const unsigned char* str)
{
    ullint hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}
