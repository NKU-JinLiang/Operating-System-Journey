#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "studentbase.h"

int loadData(char lines[][MAXLENGTH], int total_lines){
    char c;
    char *name;
    char *number;
    char *Chinese;
    char *Math;
    char *English;
    int line = 0;
    while (line < total_lines) {
        char name[32] = {0};
        char number[16] = {0};
        char Chinese[8] = {0}, Math[8] = {0}, English[8] = {0};

        int field = 0;
        int pos[5] = {0};

        for (int i = 0; lines[line][i] != '\0' && lines[line][i] != '\n'; i++) {
            if (lines[line][i] == '/') {
                field++;
                continue;
            }

            switch (field) {
                case 0: name[pos[0]++] = lines[line][i]; break;
                case 1: number[pos[1]++] = lines[line][i]; break;
                case 2: Chinese[pos[2]++] = lines[line][i]; break;
                case 3: Math[pos[3]++] = lines[line][i]; break;
                case 4: English[pos[4]++] = lines[line][i]; break;
            }
        }
        name[pos[0]] = '\0';
        number[pos[1]] = '\0';
        Chinese[pos[2]] = '\0';
        Math[pos[3]] = '\0';
        English[pos[4]] = '\0';
        memset(hashtab, 0, sizeof(hashtab));
        struct studentNode *test;
        test = install(name, number, atoi(Chinese), atoi(Math), atoi(English), line++);
        if(test = NULL) {printf("install student information failed."); return 0;}
    }
    return 1;
}

int addStudent(){
    char *name;
    char *number;
    char *Chinese;
    char *Math;
    char *English;
    char c;
    int i = 0;
    int exist = 0;
    struct studentNode *student;
    /*input name*/
    printf("姓名\n");
    while((c=getchar())!=EOF && c!='\n'){
        *(name+(i++)) = c;
    }
    i = 0;
    /*input student number*/
    printf("\n请输入学号\n");
    while((c=getchar())!=EOF && c!='\n'){
        *(number+(i++)) = c;
    }
    i = 0;
    /*check if the student already exist*/
    if ((student = lookup(name)) != NULL){
        exist = 1;
        printf("\n该学生信息已存在！键入y将修改学生信息，键入n将退出。\n");
        while((c=getchar())!='y' && c!='n'){
            printf("\n该学生信息已存在！键入y将修改学生信息，键入n将退出。\n");
        }
        if(c == 'n'){
            return 0;
        }
    }
    /*input Chinese grade*/
    printf("\n请输入学号\n");
    while((c=getchar())!=EOF && c!='\n'){
        *(Chinese+(i++)) = c;
    }
    i = 0;
    /*input Math grade*/
    printf("\n请输入学号\n");
    while((c=getchar())!=EOF && c!='\n'){
        *(Math+(i++)) = c;
    }
    i = 0;
    /*input Engilish grade*/
    printf("\n请输入学号\n");
    while((c=getchar())!=EOF && c!='\n'){
        *(English+(i++)) = c;
    }
    i = 0;
    return 0;
}