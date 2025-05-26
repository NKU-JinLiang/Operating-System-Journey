#include <stdio.h>
#define DATASIZE 3000 /*how many students we can record*/
#define MAXLENGTH 200 /*how long one student's information*/

struct studentNode { /* table entry: */
    struct studentNode *next; /* next entry in chain */
    char *name; /* student name */
    char *number; /* student number */
    int Chinese; /* Chinese grade */
    int Math; /* Math grade */
    int English; /* English grade */
    int line; /* record the data position in file */
};
unsigned hash(char *s);
struct studentNode *lookup(char *s);
struct studentNode *install(char *name, char *number, int Chinese, int Math, int English, int line);
static struct studentNode *hashtab[DATASIZE]; /* pointer table */

int loadData(char lines[][MAXLENGTH], int total_lines);
void printStudent(const struct studentNode *s);
void printAllStudents();