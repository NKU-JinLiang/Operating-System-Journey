#include <string.h>
#include <stdio.h>
/* getline: read a line into s, return length */
int getline(char s[],int lim)
{
int c, i;
for (i=0; i < lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
    s[i] = c;
if (c == '\n') {
    s[i] = c;
    ++i;
    }
s[i] = '\0';
return i;
}
/* reverse: reverse string s in place */
void reverse(char s[])
{
int c, i, j;
for (i = 0, j = strlen(s)-2; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
    }
    s[strlen(s)-1] = '\0';
}
int main(){
    char s[1000];
    getline(s, 1000);
    reverse(s);
    int i;
    for (i=0; i < 1000-1 &&  s[i]!='\0'; ++i)
        printf("%c", s[i]);
    return 0;
}
