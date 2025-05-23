#include <stdio.h>
#include <string.h>
 #define MAXLENGTH 200 
int loadData(char *filename){
    FILE *studentData;
    studentData = fopen(filename, "r");
    char student[200];
    while (fgets(student, MAXLENGTH, studentData)) {
        for(int i=0;i< strlen(student);i++)
    
    }

    }