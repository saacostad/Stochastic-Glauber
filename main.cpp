#include <iostream>
#include "headers/geometryHeader.h"
#include <cmath>

double PI = 3.141592653589793;


int main(void){
    
    using namespace std; 


    for (double theta = 0; theta < PI; theta += (PI / 100.0)){ 
        for (double phi = -PI; phi < PI; phi += (PI / 100.0)){
             
            double x = sin(theta) * cos(phi);
            double y = sin(theta) * sin(phi);
            double z = cos(theta);

           cout << x << "," << y << "," << z << "," << Ylm(5, 3, theta, phi) << endl;            
        }
    }

    return 0;
}


