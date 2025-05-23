#include <string.h>
#include <stdlib.h>

#define DATASIZE 3000 /*how many students we can record*/
static struct studentNode *hashtab[DATASIZE]; /* pointer table */

struct studentNode { /* table entry: */
    struct studentNode *next; /* next entry in chain */
    char *name; /* student name */
    int Chinese; /* Chinese grade */
    int Math; /* Math grade */
    int English; /* English grade */
};

/* hash: form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31 * hashval;
    return hashval % DATASIZE;
}

/* lookup: look for s in hashtab */
struct studentNode *lookup(char *s)
{
    struct studentNode *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* install: put (name, Chinese, Math, English) in hashtab */
struct studentNode *install(char *name, int Chinese, int Math, int English)
{
    struct studentNode *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct studentNode *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL){
            return NULL;
        }
        hashval = hash(name);
        np->next = hashtab[hashval];   // insert
        hashtab[hashval] = np;
    }
    np->Chinese=Chinese;
    np->Math=Math;
    np->English=English;
    return np;
}