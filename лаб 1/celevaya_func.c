#include <stdio.h>
float main () {
    float tochnost;
    printf("введите точность ""\n");
    scanf("%f", &tochnost);
    float e,d,i,ost;
    e=1;
    d=1;
    i=1;
    while ((1/d)>=tochnost){
        ost=1/d;
        e+=ost;
        i++;
        d*=i;

    }
    printf("%f",e);
     
     return 0;
}
