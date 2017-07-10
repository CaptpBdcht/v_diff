#include "../v_includes.h"

/** HELP/VERSION **/
void display_help()
{
    printf("Usage: diff [OPTION]... FILES Compare FILES line by line.\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("      --normal                    output a normal diff (the default)\n");
    printf("  -q, --brief                     report only when files differ\n");
    printf("  -s, --report-identical-files    report when two files are the same\n");
    printf("  -u, -U NUM, --unified[=NUM]     output NUM (default 3) lines of unified context\n");
    printf("  -e, --ed                        output an ed script\n");
    printf("  -y, --side-by-side              output in two columns\n");
    printf("  -r, --recursive                 recursively compare any subdirectories found\n");
    printf("  -N, --new-file                  treat absent files as empty\n");
    printf("      --ignore-file-name-case     ignore case when comparing file names\n");
    printf("      --no-ignore-file-name-case  consider case when comparing file names\n");
    printf("  -i, --ignore-case               ignore case differences in file contents\n");
    printf("  -E, --ignore-tab-expansion      ignore changes due to tab expansion\n");
    printf("  -Z, --ignore-trailing-space     ignore white space at line end\n");
    printf("  -b, --ignore-space-change       ignore changes in the amount of white space\n");
    printf("  -w, --ignore-all-space          ignore all white space\n");
    printf("  -B, --ignore-blank-lines        ignore changes whose lines are all blank\n");
    printf("      --hash=<str>                hash files using <str> hash (default djb2)\n");
    printf("                                    (options : djb2, djb2a, sdbm)\n");
    printf("      --help                      display this help and exit\n");
    printf("  -v, --version                   output version information and exit\n\n");
    printf("FILES are 'FILE1 FILE2' or 'DIR1 DIR2' or 'DIR FILE...' or 'FILE... DIR'.\n");
    printf("If --from-file or --to-file is given, there are no restrictions on FILE(s).\n");
    printf("If a FILE is '-', read standard input.\n");
    printf("Exit status is 0 if inputs are the same, 1 if different, 2 if trouble\n\n");
    printf("Report bugs to: bug-diffutils@gnu.org \n");
    printf("GNU diffutils home page: <http://www.gnu.org/software/diffutils/>\n");
    printf("General help using GNU software: <http://www.gnu.org/gethelp/>\n");
}

void display_version()
{
    printf("v_diff Version 0.1\n");
    printf("Written by Vincent Milano, Valentin Fries.\n\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("You might find it at https://bitbucket.org/Captp/v_diff when we'll release it ;)\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
/** END H/V **/


/** ARGUMENTS **/
void display_short_arg_error(char arg)
{
	fprintf(stderr, "v_diff: invalid option -- '%c'\n", arg);
	display_help_error();
}

void display_long_arg_error(char* arg)
{
	fprintf(stderr, "v_diff: unknown option -- '%s'\n", arg);
	display_help_error();
}

void display_extra_operand_error(char* arg)
{
	fprintf(stderr, "v_diff: extra operand '%s'\n", arg);
	display_help_error();
}

void display_missing_operand_error(char* arg)
{
	fprintf(stderr, "v_diff: missing operand after '%s'\n", arg);
	display_help_error();
}

void display_ambiguous_param(char* arg)
{
	fprintf(stderr, "v_diff: ambiguous option %s\n", arg);
	display_help_error();
}

void display_file_error(char* path)
{
	fprintf(stderr, "v_diff: %s: No such file or directory\n", path);
}

void display_help_error(void)
{
	fprintf(stderr, "v_diff: Use 'v_diff --help' to display available options.\n");
}
/** END ARGS **/


/** OUTPUT STYLE **/
/*
 *  v_display - Display Dispatcher
 *
 * @v_config  : options flags
 * @vertexLCS : LCS vertex generated
 * @fsrc      : source file
 * @fdst      : destination file
 * @i         : last line
 * @j         : last column
 *
 * Note that following the
 * asked input, different
 * types of stacks are created.
 *  (see Tools/string_utils.c)
 * * * * * * * * * * * * * * */
void v_display(v_Config* v_config, int** vertexLCS, File* fsrc, File* fdst, int i, int j)
{
    // Find number of different display(s) asked
    usint printFlags = v_config->b_output_ed_script +
                       v_config->b_output_two_columns +
                       v_config->b_unified_context;

    // If one flag, choose right display
    if (printFlags == 1)
    {
        if (v_config->b_output_ed_script)
        {
            ed_stack* eStack = NULL;
            display_ed_script(vertexLCS, fsrc, fdst, i, j, 0, FALSE, 0, FALSE, eStack);
        }
        else if (v_config->b_output_two_columns)
            display_side_side(vertexLCS, fsrc, fdst, i, j);
        else if (v_config->b_unified_context)
        {
            u_stack* uStack = NULL;
            if(!v_config->lines_unified_context)
                v_config->lines_unified_context = 3;

            display_unified(vertexLCS, fsrc, fdst, i, j, v_config->lines_unified_context, uStack);
        }
    }
    // If two, display error message
    else if (printFlags >= 2)
    {
        printf("v_diff: conflicting output style options\n");
        printf("v_diff: Try 'v_diff --help' for more information.\n");
        exit(EXIT_TROUBLE);
    }
    // Otherwise print normal v_diff
    else
    {
        // Stack used for the output
        v_stack* vStack = NULL;

        // Initialize files flags
        int* srcLines = malloc(2 * sizeof(int));
        srcLines[0] = -1;
        srcLines[1] = -1;
        int* dstLines = malloc(2 * sizeof(int));
        dstLines[0] = -1;
        dstLines[1] = -1;

        display_usual(vertexLCS, fsrc, fdst, i, j, FALSE, FALSE, FALSE, srcLines, dstLines, vStack);
    }
}


/*
 *  display_usual
 *
 * @vertexLCS : LCS vertex
 * @fsrc      : source file
 * @fdst      : destination file
 * @inSequence : in a difference or not
 * @beginPlus  : sequence begins by '+'
 * @changing   : sequence has changed (+ to -)
 * @srcLines   : source file line flags
 * @dstLines   : dest. file line flags
 * * * * * * * * * * * * * * */
void display_usual(int** vertexLCS, File* fsrc, File* fdst, int i, int j,
                   Bool inSequence, Bool beginPlus, Bool changing,
                   int* srcLines, int* dstLines, v_stack* vStack)
{
    // Got to go upper-left
    if (i > 0 && j > 0
        && *(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
    {
        if (inSequence) // We have stuff to print
        {
            if (beginPlus)  // Addition or Changing
            {
                if (changing)   // Changing
                    v_stack_push(&vStack, srcLines, 'c', dstLines);
                else    // Addition
                    v_stack_push(&vStack, srcLines, 'a', dstLines);
            }
            else    // Deletion
                v_stack_push(&vStack, srcLines, 'd', dstLines);

            srcLines[0] = -1;
            srcLines[1] = -1;
            dstLines[0] = -1;
            dstLines[1] = -1;
        }

        display_usual(vertexLCS, fsrc, fdst, i-1, j-1,
                      FALSE, FALSE, FALSE,
                      srcLines, dstLines, vStack);
    }
    // Got to go left
    else if (j > 0 && (!i || vertexLCS[i][j-1] >= vertexLCS[i-1][j]))
    {
        // Set sequence to now, get lines
        if (!inSequence)
        {
            beginPlus = TRUE;
            inSequence = TRUE;
            srcLines[0] = i;
            srcLines[1] = i;
            dstLines[0] = j;
            dstLines[1] = j;
        }
        // New addition, get line
        else
        {
            dstLines[0] = j;
        }

        display_usual(vertexLCS, fsrc, fdst, i, j-1,
                      inSequence, beginPlus, changing,
                      srcLines, dstLines, vStack);
    }
    // Got to go up
    else if (i > 0 && (!j || vertexLCS[i][j-1] < vertexLCS[i-1][j]))
    {
        // Deletion
        if (!inSequence)
        {
            beginPlus = FALSE;
            inSequence = TRUE;
            srcLines[0] = i;
            srcLines[1] = i;
            dstLines[0] = j;
            dstLines[1] = j;
        }
        // Changing OR deletion
        else
        {
            if (!changing && beginPlus)
                changing = TRUE;

            srcLines[0] = i;
        }

        display_usual(vertexLCS, fsrc, fdst, i-1, j,
                      inSequence, beginPlus, changing,
                      srcLines, dstLines, vStack);
    }
    // Got to output
    else
    {
        if (inSequence) // We have stuff to print
        {
            if (beginPlus)  // Addition or Changing
            {
                if (changing)   // Changing
                    v_stack_push(&vStack, srcLines, 'c', dstLines);
                else    // Addition
                    v_stack_push(&vStack, srcLines, 'a', dstLines);
            }
            else    // Deletion
                v_stack_push(&vStack, srcLines, 'd', dstLines);
        }

        // Output
        v_stack_empty(&vStack, fsrc, fdst);
    }
}


/*
 *  display_side_side
 *
 * @vertexLCS : LCS vertex
 * @fsrc      : source file
 * @fdst      : destination file
 * * * * * * * * * * * * * * */
void display_side_side(int** vertexLCS, File* fsrc, File* fdst, int i, int j)
{
    /*
     * A problem remains on the changes.
     * We only show additions and deletions,
     * not changes line-by-line
     */
    if (i > 0 && j > 0
        && *(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
    {
        display_side_side(vertexLCS, fsrc, fdst, i-1, j-1);

		printf("%s", (char *)fsrc->data->get(fsrc->data, i-1));
        printSpaces(60 - 1 - strlen((char *)fsrc->data->get(fsrc->data, i-1)));
        printf("   %s\n",(char *)fsrc->data->get(fsrc->data, i-1));
    }
    else if (j > 0 && (!i || vertexLCS[i][j-1] >= vertexLCS[i-1][j]))
    {
        display_side_side(vertexLCS, fsrc, fdst, i, j-1);

        printSpaces(60);
        printf("> %s\n", (char *)fdst->data->get(fdst->data, j-1));
    }
    else if (i > 0 && (!j || vertexLCS[i][j-1] < vertexLCS[i-1][j]))
    {
        display_side_side(vertexLCS, fsrc, fdst, i-1, j);

        printf("%s", (char *)fsrc->data->get(fsrc->data, i-1));
        printSpaces(60 - strlen((char *)fsrc->data->get(fsrc->data, i-1)));
        printf("<\n");
    }
}


/*
 *  display_unified
 *
 * @vertexLCS   : LCS vertex
 * @fsrc        : source file
 * @fdst        : destination file
 * @lines_asked : param lines asked by user
 *
 * Call print_unified_diff()
 * with same params. This functions
 * only helps to print right headers
 * * * * * * * * * * * * * * */
void display_unified(int** vertexLCS, File* fsrc, File* fdst, int i, int j, int lines_asked, u_stack* uStack)
{
    // Print headers, with filename & last file modif.
    printf("--- %s %s\n", fsrc->name, fsrc->modtime);
    printf("+++ %s %s\n", fdst->name, fdst->modtime);

    // unified_get_last_line(vertexLCS, fsrc, fdst, i, j, lines_asked));
    // Has a recursive state problem, which ensures you cannot return the
    // right flag whether you are ascending or descending the recursive call..

    printf("@@ -1,%d +1,%d @@\n", lines_asked, lines_asked);
    print_unified_diff(vertexLCS, fsrc, fdst, i, j, lines_asked, uStack);
}

void print_unified_diff(int** vertexLCS, File* fsrc, File* fdst, int i, int j, int lines_asked, u_stack* uStack)
{
    if (i > 0 && j > 0
        && *(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
    {
        // Everytime we prepare to push, we
        // just check the line is in the right
        // range (asked or not -> default 3)
        if (i <= lines_asked)
            u_stack_push(&uStack, ' ', (char *)fsrc->data->get(fsrc->data, i-1));

        // Go upper-left
        print_unified_diff(vertexLCS, fsrc, fdst, i-1, j-1, lines_asked, uStack);
    }
    else if (j > 0 && (!i || vertexLCS[i][j-1] >= vertexLCS[i-1][j]))
    {
        if (i <= lines_asked)
            u_stack_push(&uStack, '+', (char *)fdst->data->get(fdst->data, j-1));

        // Go left
        print_unified_diff(vertexLCS, fsrc, fdst, i, j-1, lines_asked, uStack);
    }
    else if (i > 0 && (!j || vertexLCS[i][j-1] < vertexLCS[i-1][j]))
    {
        if (i <= lines_asked)
            u_stack_push(&uStack, '-', (char *)fsrc->data->get(fsrc->data, i-1));

        // Go up
        print_unified_diff(vertexLCS, fsrc, fdst, i-1, j, lines_asked, uStack);
    }
    else
        u_stack_empty(&uStack);     // Output
}

/*int unified_get_last_line(int** vertexLCS, File* fsrc, File* fdst, int i, int j, int lines_asked)
{
    if (i > 0 && j > 0
        && *(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
    {
        //printf("   i+1=%d <= lines_asked=%d ... j-1=%d\n", i+1, lines_asked, j-1);
        if (i <= lines_asked)
        {
            return j-1;
        }
        unified_get_last_line(vertexLCS, fsrc, fdst, i-1, j-1, lines_asked);
    }
    else if (j > 0 && (!i || vertexLCS[i][j-1] >= vertexLCS[i-1][j]))
    {
        //printf("++ i+1=%d <= lines_asked=%d ... j-1=%d\n", i+1, lines_asked, j-1);
        if (i <= lines_asked)
        {
            return j-1;
        }
        unified_get_last_line(vertexLCS, fsrc, fdst, i-1, j-1, lines_asked);
    }
    else if (i > 0 && (!j || vertexLCS[i][j-1] < vertexLCS[i-1][j]))
    {

        //printf("-- i+1=%d <= lines_asked=%d ... j-1=%d\n", i+1, lines_asked, j-1);
        if (i <= lines_asked)
        {
            return j-1;
        }
        unified_get_last_line(vertexLCS, fsrc, fdst, i-1, j, lines_asked);
    }

    return 0;
}*/


/*
 *  display_ed_script
 *
 * @vertexLCS   : LCS vertex
 * @fsrc        : source file
 * @fdst        : destination file
 * @currentSeq  : current sequence state
 *                  3 - Add
 *                  2 - Delete or Modif.
 *                  0 - Nothing to do
 * @changing    : Modification flag
 * @gap         : Line number for Modif.
 * * * * * * * * * * * * * * */
void display_ed_script(int** vertexLCS, File* fsrc, File* fdst, int i, int j,
                 usint currentSeq, Bool changing, int endLine, Bool gap, ed_stack* eStack)
{
    if (i > 0 && j > 0
        && *(ullint*)fsrc->hashtable->get(fsrc->hashtable, i-1) == *(ullint*)fdst->hashtable->get(fdst->hashtable, j-1))
    {
        if (currentSeq == 3)    // Content must be Added
        {
            printf("%da\n", i);
            stack_empty(&eStack);
            printf(".\n");
        }

        if (currentSeq == 2)
        {
            if (changing)       // Modification
            {
                if (gap)
                {
                    printf("%d,%dc\n", i+1, endLine);
                    stack_empty(&eStack);
                    printf(".\n");
                }
                else
                {
                    printf("%dc\n", endLine);
                    stack_empty(&eStack);
                    printf(".\n");
                }
            }
            else                // Deletion
            {
                if (gap)
                    printf("%d,%dd\n", j, endLine);
                else
                    printf("%dd\n", endLine);
            }
        }

        display_ed_script(vertexLCS, fsrc, fdst, i-1, j-1,
                    0, FALSE, 0, FALSE, eStack);        // Reinit recursive params
    }
    else if (j > 0 && (!i || vertexLCS[i][j-1] >= vertexLCS[i-1][j]))
    {
        stack_push(&eStack, (char *)fdst->data->get(fdst->data, j-1));

        display_ed_script(vertexLCS, fsrc, fdst, i, j-1,
                    3, changing, endLine, gap, eStack);       // Adding
    }
    else if (i > 0 && (!j || vertexLCS[i][j-1] < vertexLCS[i-1][j]))
    {
        // + to - => Change spotted
        if (currentSeq == 3)
            changing = TRUE;

        // Keep end line if we don't have any
        if (0 == endLine)
        {
            endLine = i;
        }
        // Otherwise, if we haven't noticed yet
        // that several line has changed, notice it
        else if (!gap)
        {
            gap = TRUE;
        }

        display_ed_script(vertexLCS, fsrc, fdst, i-1, j,
                    2, changing, endLine, gap, eStack);    // Delete or Modif
    }
    else
    {
        if (currentSeq == 3)    // Content must be Added
        {
            printf("%da\n", i);
            stack_empty(&eStack);
            printf(".\n");
        }

        if (currentSeq == 2)
        {
            if (changing)       // Modification
            {
                if (gap)
                {
                    printf("%d,%dc\n", i+1, endLine);
                    stack_empty(&eStack);
                    printf(".\n");
                }
                else
                {
                    printf("%dc\n", i+1);
                    stack_empty(&eStack);
                    printf(".\n");
                }
            }
            else                // Deletion
            {
                if (gap)
                    printf("%d,%dd\n", j+1, endLine);
                else
                    printf("%dd\n", j+1);
            }
        }
    }
}
/***** END O_S *****/


void printSpaces(int n)
{
    int i;

    for (i = 0; i < n; i++)
        printf(" ");
}
