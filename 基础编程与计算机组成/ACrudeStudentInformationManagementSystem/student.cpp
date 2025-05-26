#include <string.h>
#include <stdlib.h>
#include "studentbase.h"

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
        if (strcmp(s, np->number) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* install: put (name, number, Chinese, Math, English) in hashtab */
struct studentNode *install(char *name, char *number, int Chinese, int Math, int English, int line)
{
    struct studentNode *np;
    unsigned hashval;
    if ((np = lookup(number)) == NULL) { /* not found */
        np = (struct studentNode *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL|| (np->number = strdup(number)) == NULL){
            return NULL;
        }
        hashval = hash(number);
        np->next = hashtab[hashval];   // insert
        hashtab[hashval] = np;
    }
    np->Chinese=Chinese;
    np->Math=Math;
    np->English=English;
    np->line=line;
    return np;
}

void printStudent(const struct studentNode *s) {
    printf("name %s\n", s->name);
    printf("number %s\n", s->number);
    printf("Chinese %d\n", s->Chinese);
    printf("Math %d\n", s->Math);
    printf("English %d\n", s->English);
    printf("line %d\n", s->line);
    printf("---------------------------\n");
}

void printAllStudents() {
    for (int i = 0; i < DATASIZE; i++) {
        struct studentNode *p = hashtab[i];
        while (p != NULL) {
            printStudent(p);
            p = p->next;
        }
    }
}