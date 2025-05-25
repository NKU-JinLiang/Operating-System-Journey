#include <stdio.h>
#define DATASIZE 3000 /*how many students we can record*/
#define MAXLENGTH 200 /*how long one student's information*/

struct studentNode;
unsigned hash(char *s);
struct studentNode *lookup(char *s);
struct studentNode *install(char *name, char *number, int Chinese, int Math, int English, int line);
static struct studentNode *hashtab[DATASIZE]; /* pointer table */

int loadData(FILE *studentData);
void printStudent(const struct studentNode *s);
void printAllStudents();