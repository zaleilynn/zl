#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv) {
    float randpr, intrval, r = 1.0;
    while(true){
        randpr = (float)rand()/RAND_MAX; 
        intrval = -(1/r) * (float)log(1-randpr);
        printf("%f\n", intrval);
        sleep(1);
    }
    return 0;
}
