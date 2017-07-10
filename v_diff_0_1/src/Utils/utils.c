#include "../v_includes.h"

void v_stack_push(v_stack** myStack, int* srcLines, char flag, int* dstLines)
{
    v_stack* newNode = malloc(sizeof(v_stack));

    newNode->srcLines = malloc(2 * sizeof(int));
    newNode->srcLines[0] = srcLines[0];
    newNode->srcLines[1] = srcLines[1];
    newNode->flag        = flag;
    newNode->dstLines = malloc(2 * sizeof(int));
    newNode->dstLines[0] = dstLines[0];
    newNode->dstLines[1] = dstLines[1];

    newNode->next = (*myStack);

    (*myStack) = newNode;
}

void v_stack_pop(v_stack** myStack, File* fsrc, File* fdst)
{
    if ((*myStack) == NULL)
        exit(EXIT_TROUBLE);

    // Get all that brick stuff
    // Source flags, Dest. flags
    // && state flag (+/ /-)
    int* srcLines = NULL;
    srcLines  = (*myStack)->srcLines;
    int* dstLines = NULL;
    dstLines  = (*myStack)->dstLines;
    char flag = (*myStack)->flag;
    v_stack* tmp_node = (*myStack);

    (*myStack) = (*myStack)->next;
    free(tmp_node->srcLines);
    free(tmp_node->dstLines);
    free(tmp_node);

    switch (flag)
    {
        case 'a':
            // One line modif
            if (dstLines[0] == dstLines[1])
            {
                printf("%da%d\n", srcLines[0], dstLines[0]);
                printf("> %s\n", (char *)fdst->data->get(fdst->data, dstLines[0]-1));
            }
            // Multiple modifs
            else
            {
                printf("%da%d,%d\n", srcLines[0], dstLines[0], dstLines[1]);
                int j;
                for (j = dstLines[0]; j <= dstLines[1]; j++)
                    printf("> %s\n", (char *)fdst->data->get(fdst->data, j-1));
            }
            break;

        case 'c':
        {
            // One line modif
            if (srcLines[0] == srcLines[1] &&
                dstLines[0] == dstLines[1])
            {
                printf("%dc%d\n", srcLines[0], dstLines[0]);
                printf("< %s\n", (char *)fsrc->data->get(fsrc->data, srcLines[0]-1));
                printf("---\n");
                printf("> %s\n", (char *)fdst->data->get(fdst->data, dstLines[0]-1));
            }
            // Multiple/Multiple modif
            else if (srcLines[0] != srcLines[1] &&
                     dstLines[0] != dstLines[1])
            {
                printf("%d,%dc%d,%d\n", srcLines[0], srcLines[1], dstLines[0], dstLines[1]);
                int i, j;
                for (i = srcLines[0]-1; i <= srcLines[1]-1; i++)
                    printf("< %s\n", (char *)fsrc->data->get(fsrc->data, i));
                printf("---\n");
                for (j = dstLines[0]-1; j <= dstLines[1]-1; j++)
                    printf("> %s\n", (char *)fdst->data->get(fdst->data, j));
            }
            else
            {
                // 1-n modif
                if (srcLines[0] != srcLines[1])
                {
                    int i;
                    printf("%d,%dc%d\n", srcLines[0], srcLines[1], dstLines[0]);
                    for (i = srcLines[0]-1; i <= srcLines[1]-1; i++)
                        printf("< %s\n", (char *)fsrc->data->get(fsrc->data, i));
                    printf("---\n");
                    printf("> %s\n", (char *)fdst->data->get(fdst->data, dstLines[0]-1));
                }
                // n-1 modif
                else
                {
                    int j;
                    printf("%dc%d,%d\n", srcLines[0], dstLines[0], dstLines[1]);
                    printf("< %s\n", (char *)fsrc->data->get(fsrc->data, srcLines[0]-1));
                    printf("---\n");
                    for (j = dstLines[0]-1; j <= dstLines[1]-1; j++)
                        printf("> %s\n", (char *)fdst->data->get(fdst->data, j));
                }
            }
            break;
        }

        case 'd':
            // One line modif
            if (srcLines[0] == srcLines[1])
            {
                printf("%dd%d\n", srcLines[0], dstLines[0]);
                printf("< %s\n", (char *)fsrc->data->get(fsrc->data, srcLines[0]-1));
            }
            // Multiple modifs
            else
            {
                printf("%d,%dd%d\n", srcLines[0], srcLines[1], dstLines[0]);
                int i;
                for (i = srcLines[0]; i <= srcLines[1]; i++)
                    printf("< %s\n", (char *)fsrc->data->get(fsrc->data, i-1));
            }
            break;
    }
}

void v_stack_empty(v_stack** myStack, File* fsrc, File* fdst)
{
	if(myStack && *myStack)
	{
		while ((*myStack)->next != NULL)
			v_stack_pop(myStack, fsrc, fdst);
		v_stack_pop(myStack, fsrc, fdst);

		free(myStack);
	}
}



void stack_push(ed_stack** myStack, char* newContent)
{
    ed_stack* newNode = malloc(sizeof(ed_stack));

    newNode->content = newContent;
    newNode->next = (*myStack);

    (*myStack) = newNode;
}

void stack_pop(ed_stack** myStack)
{
    if ((*myStack) == NULL)
        exit(EXIT_TROUBLE);

    char* value = (*myStack)->content;
    ed_stack* tmp_node = (*myStack);

    (*myStack) = (*myStack)->next;
    free(tmp_node->content);
    free(tmp_node);

    printf("%s\n", value);
}

void stack_empty(ed_stack** myStack)
{
	if(myStack && *myStack)
	{
		while ((*myStack)->next != NULL)
			stack_pop(myStack);
		stack_pop(myStack);

		free(myStack);
	}
}



void u_stack_push(u_stack** myStack, char newFlag, char* newContent)
{
    u_stack* newNode = malloc(sizeof(u_stack));

    newNode->flag       = newFlag;
    newNode->content    = newContent;
    newNode->next = (*myStack);

    (*myStack) = newNode;
}

void u_stack_pop(u_stack** myStack)
{
    if ((*myStack) == NULL)
        exit(EXIT_TROUBLE);

    char flag  = (*myStack)->flag;
    char* value = (*myStack)->content;
    u_stack* tmp_node = (*myStack);

    (*myStack) = (*myStack)->next;
    // ??.. free(tmp_node->content);
    free(tmp_node);

    printf("%c%s\n", flag, value);
}

void u_stack_empty(u_stack** myStack)
{
	if(myStack && *myStack)
	{
		while ((*myStack)->next != NULL)
			u_stack_pop(myStack);
		u_stack_pop(myStack);

		free(myStack);
	}
}
