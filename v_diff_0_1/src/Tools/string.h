#ifndef STRING_H
#define	STRING_H

typedef struct String {
    char *value;
    size_t length;
    
    Bool (*add)(struct String*, const char);
    Bool (*remove)(struct String*, size_t);
    
    struct String* (*set)(struct String*, const char*);
    char *(*get)(struct String*, size_t);
    struct String* (*in)(struct String*);
    struct String* (*out)(struct String*);
    
    int (*pos)(struct String*, const char);
    int (*exists)(struct String*, struct String*);
    int (*cexists)(struct String*, const char*);
    struct String *(*sub)(struct String*, size_t, size_t);
    struct String* (*copy)(struct String*, struct String*);
    struct String* (*ncopy)(struct String*, struct String*, size_t);
    Bool (*compare)(struct String*, struct String*);
    Bool (*ccompare)(struct String*, char*);
    Bool (*ncompare)(struct String*, struct String*, size_t);
    struct String* (*append)(struct String*, struct String*);
    struct String* (*cappend)(struct String*, const char*);
    struct String* (*trunc)(struct String*, size_t, size_t);
    char *(*toc)(struct String*);
    
    Bool (*startsWith)(struct String*, struct String*);
    Bool (*cstartsWith)(struct String*, const char*);
    Bool (*endsWith)(struct String*, struct String*);
    Bool (*cendsWith)(struct String*, const char*);
    
    struct String* (*clear)(struct String*);
    void (*free)(struct String*);
} String;

String* string_new(const char*);
String* string_init(String*, const char*);
void string_free(String*);

Bool string_add(String*, const char);
Bool string_remove(String*, size_t);
String* string_clear(String*);

String* string_set(String*, const char*);
char* string_get(String*, size_t);

String* string_stdin(String*);
String* string_stdout(String*);

int string_pos(String*, const char);
int string_exists(String*, String*);
int string_cexists(String*, const char*);
String* string_sub(String*, size_t, size_t);
String* string_copy(String*, String*);
String* string_ncopy(String*, String*, size_t);
Bool string_compare(String*, String*);
Bool string_ccompare(String*, char*);
Bool string_ncompare(String*, String*, size_t);
String* string_append(String*, String*);
String* string_cappend(String*, const char*);
String* string_trunc(String*, size_t, size_t);
char* string_toc(String*);

Bool string_starts_with(String*, String*);
Bool string_cstarts_with(String*, const char*);
Bool string_ends_with(String*, String*);
Bool string_cends_with(String*, const char*);


#endif	/* STRING_H */

