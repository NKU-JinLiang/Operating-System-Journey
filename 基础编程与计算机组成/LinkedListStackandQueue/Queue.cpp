#include "structure.h"
#include <stdio.h>
double queue[MAXVAL]; /* value stack */
int begin = 0; /* next free stack position */
int end = 0;
/* push: push f onto value stack */
void enqueue(double f)
{
    if (((end +1) % MAXVAL) != begin){
        queue[end] = f;
        end = (end + 1) % MAXVAL;
    }
    else
    printf("error: queue full, can't push %g\n", f);
}
/* pop: pop and return top value from stack */
double dequeue(void)
{
    if (begin != end){
        double val = queue[begin];
        begin = (begin + 1) % MAXVAL;
        return val;}
    else {
        printf("error: queue empty\n");
        return 0.0;
    }
}

double front(){
    if (begin != end)
    return queue[begin];
    else {
        printf("error: queue empty\n");
        return 0.0;
    }
}