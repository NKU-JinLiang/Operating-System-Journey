#include <stdio.h>
#include <string.h>
#include "studentbase.h"
int main(){
    printf("Welcome to the student database.\n");
    printf("Initializing database.....\n");
    FILE *studentData  = fopen("record", "r");
    if (!studentData) {
        perror("Failed to open file.\n");
        return 1;
    }
    if(loadData(studentData) == 0)
        printf("load data failed.\n");
    printAllStudents();
    printf("Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");
    char c;
    while((c = getchar()) != '4'){
        while ((getchar()) != '\n'); 
        switch (c) {
            case '1':
                while (1) {
                    printf("Query existing student information. Please enter the 6-digit student ID (or q to quit):\n");

                    char ID[8];  // allow for 6 digits + '\0' + 1 extra
                    int i = 0;
                    char ch;

                    while (i < 6 && (ch = getchar()) != EOF && ch != '\n') {
                        if (ch == 'q') return 0; 
                        ID[i++] = ch;
                    }
                    ID[i] = '\0';
                    while (ch != '\n' && ch != EOF) ch = getchar();

                    if (strlen(ID) != 6) {
                        printf("Invalid ID length. Please enter exactly 6 digits.\n");
                        continue;
                    }

                    struct studentNode *lookupresult = lookup(ID);
                    if (lookupresult == NULL) {
                        printf("Can't find the student information. Try again.\n");
                        continue;
                    }

                    printStudent(lookupresult);
                    break;
                }
                break;
            case '2':
                printf("Add new student information\n");
                break;
            case '3':
                printf("Modify existing student information.\n");
                break;
            default:
                printf("Unrecognizable characters. Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");
        }
    }
    return 0;
}