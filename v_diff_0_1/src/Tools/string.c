#include "../v_includes.h"

/*
 * Creates & initializes a new String data structure.
 * * * * * * * * * * * * * * * * * * */
String* string_new(const char *value)
{
    String *this = NULL;

    if((this = malloc(sizeof(String))) != NULL)
        string_init(this, value);

    return this;
}

/*
 * Initializes structure content.
 * * * * * * * * * * * * * * * * * * */
String* string_init(String *this, const char *value)
{
    if(this != NULL)
    {
        this->value = NULL;
        this->length = 0;

        this->add = string_add;
        this->remove = string_remove;

        this->set = string_set;
        this->get = string_get;
        this->in = string_stdin;
        this->out = string_stdout;

        this->pos = string_pos;
        this->exists = string_exists;
        this->cexists = string_cexists;
        this->sub = string_sub;
        this->copy = string_copy;
        this->ncopy = string_ncopy;
        this->compare = string_compare;
		this->ccompare = string_ccompare;
        this->ncompare = string_ncompare;
        this->append = string_append;
        this->cappend = string_cappend;
        this->trunc = string_trunc;
        this->toc = string_toc;

        this->startsWith = string_starts_with;
        this->cstartsWith = string_cstarts_with;
        this->endsWith = string_ends_with;
        this->cendsWith = string_cends_with;

        this->clear = string_clear;
        this->free = string_free;

        if(value != NULL)
            this->set(this, value);
    }

    return this;
}

/*
 * Free the given String structure.
 * * * * * * * * * * * * * * * * * * */
void string_free(String *this)
{
    if(this != NULL)
    {
        this->clear(this);
        free(this);
    }
}

/*
 * Append a new char at the end of the given String.
 * * * * * * * * * * * * * * * * * * */
Bool string_add(String *this, const char c)
{
    if(this != NULL && (this->value = realloc(this->value, ++this->length * sizeof(char))) != NULL)
    {
        this->value[this->length-1] = c;
        return TRUE;
    }

    return FALSE;
}

/*
 * Remove the char at the given index.
 * * * * * * * * * * * * * * * * * * */
Bool string_remove(String *this, size_t index)
{
    size_t i;
    Bool found = FALSE;
    char *temp = NULL;

    if(this != NULL && index >= 0 && index < this->length)
    {
        if((temp = (char*)malloc(--this->length * sizeof(char))) != NULL)
        {
            for(i = 0 ; i < this->length ; i++)
            {
                if(i == index)
                    found = TRUE;

                temp[i] = this->value[found ? i+1 : i];
            }

            free(this->value);
            this->value = temp;

            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Clear the String value (char*).
 * * * * * * * * * * * * * * * * * * */
String* string_clear(String *this)
{
    if(this != NULL && this->length > 0 && this->value != NULL)
    {
        free(this->value);
        this->value = NULL;
        this->length = 0;
    }

    return this;
}

/*
 * Set the String content based on the given char*.
 * * * * * * * * * * * * * * * * * * */
String* string_set(String *this, const char *value)
{
    if(this != NULL)
    {
        size_t i;

        this->clear(this);

        if(value != NULL)
        {
            for(i = 0 ; value[i] != '\0' ; i++)
                this->add(this, value[i]);
        }
    }

    return this;
}

/*
 * Returns a pointer to the char at the given index in the String.
 * * * * * * * * * * * * * * * * * * */
char* string_get(String *this, size_t index)
{
    if(this != NULL && index >= 0 && index < this->length)
        return &this->value[index];
    else
        return NULL;
}

/*
 * Input the String content trough STDIN.
 * * * * * * * * * * * * * * * * * * */
String* string_stdin(String *this)
{
    if(this != NULL)
    {
        char c;

        this->clear(this);

        while((c = getchar()) != '\n')
            this->add(this, c);
    }

    return this;
}

/*
 * Output the String content on STDOUT.
 * * * * * * * * * * * * * * * * * * */
String* string_stdout(String *this)
{
    if(this != NULL && this->length > 0 && this->value != NULL)
    {
        size_t i;

        for(i = 0 ; i < this->length ; i++)
            printf("%c", this->value[i]);
        printf("\n");
    }

    return this;
}

/*
 * Returns the index of the first occurence
 * of the given char in the String.
 * * * * * * * * * * * * * * * * * * */
int string_pos(String *this, const char search)
{
    if(this != NULL)
    {
        size_t i;

        for(i = 0 ; i < this->length ; i++)
        {
            if(this->value[i] == search)
                return i;
        }
    }

    return -1;
}

/*
 * Returns the index of the first occurence
 * of the given String in this String.
 * * * * * * * * * * * * * * * * * * */
int string_exists(String *this, String *search)
{
    if(this != NULL && search != NULL && this->length > search->length)
    {
        size_t i, j, k = this->length - search->length;

        for(i = 0 ; i < k ; i++)
        {
            Bool found = TRUE;

            for(j = 0 ; j < search->length ; j++)
            {
                if(this->value[i+j] != search->value[j])
                {
                    found = FALSE;
                    break;
                }
            }

            if(found)
                return i;
        }
    }

    return -1;
}

/*
 * Returns the index of the first occurence 
 * of the given char* in this String.
 * * * * * * * * * * * * * * * * * * */
int string_cexists(String *this, const char *search)
{
    if(this != NULL && search != NULL)
    {
        size_t search_length = 0;

        while(search[search_length] != '\0')
            search_length++;

        if(this->length > search_length)
        {
            size_t i, j, k = this->length - search_length;

            for(i = 0 ; i < k ; i++)
            {
                Bool found = TRUE;

                for(j = 0 ; j < search_length ; j++)
                {
                    if(this->value[i+j] != search[j])
                    {
                        found = FALSE;
                        break;
                    }
                }

                if(found)
                    return i;
            }
        }
    }

    return -1;
}

/*
 * Returns a substring using the start & the end indexes
 * in the String content.
 * * * * * * * * * * * * * * * * * * */
String* string_sub(String *this, size_t start, size_t end)
{
    if(this != NULL && this->length > 0 && start >= 0 && end < this->length && start <= end)
    {
        size_t i;
        String *str = string_new(NULL);

        for(i = start ; i <= end ; i++)
            str->add(str, this->value[i]);

        return str;
    }

    return NULL;
}

/*
 * Copy src content in the current String.
 * * * * * * * * * * * * * * * * * * */
String* string_copy(String *this, String *src)
{
    if(this != NULL && src != NULL)
        this->set(this, src->value);

    return this;
}

/*
 * Copy n chars of src in the current String.
 * * * * * * * * * * * * * * * * * * */
String* string_ncopy(String *this, String *src, size_t size)
{
    if(this != NULL && src != NULL && size >= 0 && size < src->length)
    {
        size_t i;

        this->clear(this);

        for(i = 0 ; i < size ; i++)
            this->add(this, src->value[i]);
    }

    return this;
}

/*
 * Compare two Strings and returns TRUE if they match ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_compare(String *this, String *str)
{
    if(this != NULL && str != NULL && this->length == str->length)
    {
        size_t i;
        Bool found = FALSE;

        for(i = 0 ; i < this->length ; i++)
        {
            if(this->value[i] != str->value[i])
            {
                found = TRUE;
                break;
            }
        }

        return !found;
    }

    return FALSE;
}

/*
 * Compare a String and a char*.
 * Returns TRUE if they match ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_ccompare(String *this, char *str)
{
	if(this != NULL && str != NULL && this->length == strlen(str))
    {
        size_t i;
        Bool found = FALSE;

        for(i = 0 ; i < this->length ; i++)
        {
            if(this->value[i] != str[i])
            {
                found = TRUE;
                break;
            }
        }

        return !found;
    }

    return FALSE;
}

/*
 * Compare n chars of the two Strings.
 * Returns TRUE if they match ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_ncompare(String *this, String *str, size_t size)
{
    if(this != NULL && str != NULL && size > 0 && this->length >= size && str->length >= size)
    {
        size_t i;
        Bool found = FALSE;

        for(i = 0 ; i < size ; i++)
        {
            if(this->value[i] != str->value[i])
            {
                found = TRUE;
                break;
            }
        }

        return !found;
    }

    return FALSE;
}

/*
 * Append a String at the end of the current one.
 * * * * * * * * * * * * * * * * * * */
String* string_append(String *this, String *str)
{
    size_t i;

    if(this != NULL && str != NULL)
    {
        for(i = 0 ; i < str->length ; i++)
            this->add(this, str->value[i]);
    }

    return this;
}

/*
 * Append a char* at the end of the current String.
 * * * * * * * * * * * * * * * * * * */
String* string_cappend(String *this, const char *str)
{
    size_t i;

    if(this != NULL && str != NULL)
    {
        for(i = 0 ; str[i] != '\0' ; i++)
            this->add(this, str[i]);
    }

    return this;
}

/*
 * Truncate current String between start and end indexes.
 * * * * * * * * * * * * * * * * * * */
String* string_trunc(String *this, size_t start, size_t end)
{
    if(this != NULL && start >= 0 && end < this->length && start < end)
    {
        size_t i, j = end - start;
        char temp[j];

        for(i = 0 ; i < j ; i++)
            temp[i] = this->value[i+start];
        temp[i] = '\0';

        this->set(this, temp);
    }

    return this;
}

/*
 * Returns a new standard C char* representation of this String.
 * * * * * * * * * * * * * * * * * * */
char* string_toc(String *this)
{
    if(this != NULL)
    {
        char *str = NULL;

        if((str = malloc((this->length + 1) * sizeof(char))) != NULL)
        {
            size_t i;

            for(i = 0 ; i < this->length ; i++)
                str[i] = this->value[i];
            str[i] = '\0';

            return str;
        }
    }

    return NULL;
}

/*
 * Returns TRUE if the String starts with str ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_starts_with(String *this, String *str)
{
    return (this != NULL && str != NULL) ? this->ncompare(this, str, str->length) : FALSE;
}

/*
 * Returns TRUE if the String starts with str ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_cstarts_with(String *this, const char *str)
{
    if(this != NULL && str != NULL)
    {
        String *tmp = string_new(str);
        Bool flag = this->startsWith(this, tmp);

        tmp->free(tmp);
        return flag;
    }

    return FALSE;
}

/*
 * Returns TRUE if the String ends with str ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_ends_with(String *this, String *str)
{
    if(this != NULL && str != NULL)
    {
        String *sub = this->sub(this, this->length - str->length, this->length - 1);
        Bool flag = sub->compare(sub, str);

        sub->free(sub);
        return flag;
    }

    return FALSE;
}

/*
 * Returns TRUE if the String ends with str ; FALSE otherwise.
 * * * * * * * * * * * * * * * * * * */
Bool string_cends_with(String *this, const char *str)
{
    if(this != NULL && str != NULL)
    {
        String *tmp = string_new(str);
        Bool flag = this->endsWith(this, tmp);

        tmp->free(tmp);
        return flag;
    }

    return FALSE;
}
