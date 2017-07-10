#ifndef ARRAY_H
#define	ARRAY_H

typedef struct Array {
    void** element;
    size_t size;

    Bool (*add)(struct Array*, void*, size_t, Bool);
    Bool (*remove)(struct Array*, size_t);
    Bool (*swap)(struct Array*, size_t, size_t);
    void *(*get)(struct Array*, size_t);

    void (*clear)(struct Array*);
    void (*free)(struct Array*);
} Array;

Array* array_new(void);
void array_init(Array*);
void array_free(Array*);
void array_clear(Array*);

Bool array_add(Array*, void*, size_t, Bool);
Bool array_remove(Array*, size_t);
Bool array_swap(Array*, size_t, size_t);
void* array_get(Array*, size_t);


#endif	/* ARRAY_H */

