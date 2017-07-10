#ifndef DIFF_CORE_H_INCLUDED
#define DIFF_CORE_H_INCLUDED

/*
 * Main v_diff processing methods.
 * * * * * * * * * * * * * * */
usint process_diff(v_Config*);
usint process_files(v_Config*, char*, char*);
usint process_dir_file(v_Config*, Bool);
usint process_dirs(v_Config*, char*, char*);

/*
 *                        get_LCS
 *
 *    Main algorithm for diff process. The LCS returns the
 * 'Longest Common Subsequence' between two sequences.
 * Here, each sequence is considered as a group of lines.
 * These lines represent both lines from source file (fsrc)
 * and destination file (fdst).
 *
 *    For the general case of an arbitrary number of input
 * sequences, this problem is known to be NP-hard (O(NP)
 * with N = lines in fsrc and P = lines in fdst).
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Bool get_LCS(v_Config*, File*, File*);

#endif // DIFF_CORE_H_INCLUDED
