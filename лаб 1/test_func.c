#include <stdio.h>
#include <math.h>
float osn(float tochnost){
    
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
    e=round(e * 1000000.0) / 1000000.0;

    return e;
     
     
}
int main(){
int ek;
    float znach[]={2.000000, 2.666667,2.708333,2.718056,2.718254};
    float ac[]={1,0.1,0.01,0.001,0.0001};
    
    if ((znach[0]==osn(ac[0]))&&(znach[1]==osn(ac[1]))&&(znach[2]==osn(ac[2]))&&(znach[3]==osn(ac[3]))&&(znach[4]==osn(ac[4]))){
        return 0;
    }
    else{return 1;

    }

}