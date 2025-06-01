#include <stdio.h>
int main(){
    double c = 1e20;
    double a = (3.14+c)-(c);
    printf("%lf\n", a);
    printf("%lf\n", 3.14 + (c-c));
    printf("%lf\n", (500 * 400) * (300 * 200));
    return 0;
}