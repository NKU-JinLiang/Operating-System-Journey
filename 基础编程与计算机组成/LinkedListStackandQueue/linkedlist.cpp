#include "structure.h"
#include <stdlib.h>
#include <stdio.h>
struct linkedlist { /* linkedlist */
    struct linkedlist *next; /* next entry in chain */
    double value;
};

int list_add_after(struct linkedlist *a, struct linkedlist *next){
    if(a->next ==NULL){
        a->next = next;
    }
    else{
        next->next = a->next;
        a->next = next;
    }
    return 0;
}

int list_remove(struct linkedlist *beging, struct linkedlist *target){//内存释放需要外部实现
    if (beging == NULL || target == NULL) {
        return -1;
    }
    struct linkedlist *search = beging;
    while(search->next != target || search->next != NULL){
        search = search->next;
    }

    if (search->next == target) {
        search->next = target->next;
        return 0;
    } else {
        printf("target not found in list\n");
        return -1;
    }
    return 0;
}