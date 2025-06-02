#include "structure.h"
#include <stdio.h>
double stack[MAXVAL]; /* value stack */
int sp = 0; /* next free stack position */
/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
    stack[sp++] = f;
    else
    printf("error: stack full, can't push %g\n", f);
}
/* pop: pop and return top value from stack */
double pop(void)
{
    if (sp > 0)
    return stack[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}