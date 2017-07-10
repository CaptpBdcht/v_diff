#include "../v_includes.h"

/*
 * Creates & initializes a new array pointer.
 * * * * * * * * * * * * * * * * * * */
Array* array_new(void)
{
    Array *this = malloc(sizeof(Array));
    array_init(this);

    return this;
}

/*
 * Initializes structure members.
 * * * * * * * * * * * * * * * * * * */
void array_init(Array *this)
{
    if(this == NULL)
		return;

    this->element = NULL;
    this->size = 0;

    this->add = array_add;
    this->remove = array_remove;
    this->swap = array_swap;
    this->get = array_get;

    this->clear = array_clear;
    this->free = array_free;
}

/*
 * Free of an array elements & pointer.
 * * * * * * * * * * * * * * * * * * */
void array_free(Array *this)
{
    if(this == NULL)
		return;

    this->clear(this);
    free(this);
}

/*
 * Deletion of the entire collection.
 * * * * * * * * * * * * * * * * * * */
void array_clear(Array *this)
{
    if(this == NULL)
		return;

    size_t i;

    for(i = 0 ; i < this->size ; i++)
        free(this->element[i]);
    free(this->element);
    this->element = NULL;

    this->size = 0;
}

/*
 * Add a new Array element.
 * If the element is already allocated, the address is reused.
 * Else, a new memory space is allocated & the value is copied.
 * * * * * * * * * * * * * * * * * * */
Bool array_add(Array *this, void *value, size_t size, Bool allocated)
{
    if(this != NULL && value != NULL && (this->element = realloc(this->element, ++this->size * sizeof(void*))) != NULL)
    {
        if(allocated)
            this->element[this->size-1] = value;
        else
        {
            if((this->element[this->size-1] = malloc(size)) == NULL)
            {
                this->element = realloc(this->element, --this->size * sizeof(void*));
                return 0;
            }
            else
                memcpy(this->element[this->size-1], value, size);
        }

        return TRUE;
    }

    return FALSE;
}

/*
 * Deletion of the Array element at the given index.
 * * * * * * * * * * * * * * * * * * */
Bool array_remove(Array *this, size_t index)
{
    void **tmp = NULL;

    if(this != NULL && index >= 0 && index < this->size
    && (tmp = malloc(--this->size * sizeof(void*))) != NULL)
    {
        size_t i;
        unsigned char flag = 0;

        for(i = 0 ; i < this->size+1 ; i++)
        {
            if(i == index)
            {
                //Only the indexed element is liberated
                free(this->element[i]);

                flag = 1;
                continue;
            }

            //Other elements are still stored
            tmp[i - flag] = this->element[i];
        }
        free(this->element);

        this->element = tmp;
        return TRUE;
    }

    return FALSE;
}

/*
 * Swapping of 2 array elements.
 * * * * * * * * * * * * * * * * * * */
Bool array_swap(Array *this, size_t first, size_t second)
{
    if(this != NULL
	&& first >= 0 && first < this->size
    && second >= 0 && second < this->size)
    {
        void *tmp = this->element[first];
        this->element[first] = this->element[second];
        this->element[second] = tmp;

        return TRUE;
    }

    return FALSE;
}

/*
 * Returns the pointer stored at the given index.
 * * * * * * * * * * * * * * * * * * */
void* array_get(Array *this, size_t index)
{
    return (this != NULL && index >= 0 && index < this->size) ? this->element[index] : NULL;
}
