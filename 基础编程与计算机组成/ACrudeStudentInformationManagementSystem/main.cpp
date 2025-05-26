#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "studentbase.h"
int main(){
    printf("Welcome to the student database.\n");
    printf("Initializing database.....\n");
    char lines[DATASIZE][MAXLENGTH] = {0};
    int total_lines = 0;
    // open file
    FILE* studentData = fopen("record", "r");
    if (!studentData) {
        perror("failed to read record file.");
        return 0;
    }
    // read all line
    while (fgets(lines[total_lines], MAXLENGTH, studentData)) {
        total_lines++;
    }
    // construct hash table
    if(loadData(lines, total_lines) == 0)
        printf("load data failed.\n");
    // close file
    fclose(studentData);
    printAllStudents();
    printf("Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");
    char c;
    while((c = getchar()) != '4'){
        while ((getchar()) != '\n'); 
        struct studentNode *lookupresult;
        int i = 0;
        char newName[32], newNumber[16];
        int newChinese, newMath, newEnglish;        
        switch (c) {
            case '1':
                while (1) {
                    printf("Query existing student information. Please enter the 6-digit student ID (or q to quit):\n");

                    char ID[8];  // allow for 6 digits + '\0' + 1 extra
                    i = 0;
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

                    lookupresult = lookup(ID);
                    if (lookupresult == NULL) {
                        printf("Can't find the student information. Try again.\n");
                        continue;
                    }

                    printStudent(lookupresult);
                    break;
                }
                break;
            case '2':
                while (1) {
                    printf("Add new student information. Please enter new student name, ID, Chinese, Math, English grade:\n");
                    scanf("%s %s %d %d %d", newName, newNumber, &newChinese, &newMath, &newEnglish);
                    lookupresult = lookup(newNumber);
                    if(lookupresult != NULL){
                        printf("The student information is already exist. Try again.\n");
                        continue;
                    }  
                    snprintf(lines[total_lines++], MAXLENGTH, "\n%s/%s/%d/%d/%d", newName, newNumber, newChinese, newMath, newEnglish);
                    studentData = fopen("record", "w");
                    if (!studentData) {
                        perror("update record file failed.");
                        return 0;
                    }
                    printf("%s", lines[total_lines - 1]);
                    for (int i = 0; i < total_lines; i++) {
                        fputs(lines[i], studentData);
                    }
                    fclose(studentData);
                    printf("Add new student successfullly. Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");             
                    break;
                }
                break;
            case '3':
                printf("Modify existing student information. Please enter the 6-digit student ID (or q to quit):\n");
                while (1) {
                    char ID[8];  // allow for 6 digits + '\0' + 1 extra

                    char ch;
                    i = 0;
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
                    lookupresult = lookup(ID);
                    if(lookupresult == NULL){
                        printf("Can't find the student information. Try again.\n");
                        continue;
                    }   
                    if (lookupresult->line >= total_lines) {
                        fprintf(stderr, "error, student record out range of record line.\n");
                        return 0;
                    }


                    printf("Please enter student name, ID, Chinese, Math, English grade:\n");
                    scanf("%s %s %d %d %d", newName, newNumber, &newChinese, &newMath, &newEnglish);
                    snprintf(lines[lookupresult->line], MAXLENGTH, "%s/%s/%d/%d/%d\n",
                            newName, newNumber, newChinese, newMath, newEnglish);

                    studentData = fopen("record", "w");
                    if (!studentData) {
                        perror("update record file failed.");
                        return 0;
                    }

                    for (int i = 0; i < total_lines; i++) {
                        fputs(lines[i], studentData);
                    }
                    fclose(studentData);
                    // update hash table 
                    free(lookupresult->name);
                    free(lookupresult->number);
                    lookupresult->name = strdup(newName);
                    lookupresult->number = strdup(newNumber);
                    lookupresult->Chinese = newChinese;
                    lookupresult->Math = newMath;
                    lookupresult->English = newEnglish;
                    printf("Modify successfullly. Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");
                    break;
                }
                break;
            case '4':
                return 0;
            default:
                printf("Unrecognizable characters. Enter 1 to query existing student information; enter 2 to add new student information; enter 3 to modify existing student information; enter 4 to end use.\n");
        }
    }
    return 0;
}