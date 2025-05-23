#include <ctype.h> 
#include <stdio.h>
 int getch(void); 
 void ungetch(int); 
 /* getop: get next character or numeric operand */ 
 #define MAXVAL 100 /* maximum depth of val stack */ 
 int sp = 0; /* next free stack position */ 
 double val[MAXVAL]; /* value stack */ 
 /* push: push f onto value stack */ 
 void push(double f) 
 { 
 if (sp < MAXVAL) 
 val[sp++] = f; 
 else 
 printf("error: stack full, can't push %g\n", f); 
 } 
 /* pop: pop and return top value from stack */ 
 double pop(void) 
 { 
 if (sp > 0) 
 return val[--sp]; 
 else { 
 printf("error: stack empty\n"); 
 return 0.0; 
 } 
 }

 int getop(char s[]) 
 { 
    int i, c; 
    while ((s[0] = c = getch()) == ' ' || c == '\t') 
    ; 
    s[1] = '\0'; 
    if (!isdigit(c) && c != '.') 
    return c; /* not a number */ 
    i = 0; 
    if (isdigit(c)) /* collect integer part */ 
    while (isdigit(s[++i] = c = getch())) 
    ; 
    if (c == '.') /* collect fraction part */ 
    while (isdigit(s[++i] = c = getch())) 
    ; 
    s[i] = '\0'; 
    if (c != EOF) 
    ungetch(c); 
    return NUMBER;
 }