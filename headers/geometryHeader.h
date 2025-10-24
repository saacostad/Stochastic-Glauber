#pragma once 
#include <gsl/gsl_sf_legendre.h>
#include <cmath>
#include <vector> 



//---------------------
// SPHERICAL HARMONICS
//---------------------
double Ylm(int l, int m, double theta, double phi) {
    // Function that return the real part of the lm spherical harmonic evaluated in theta and phi 

    double Plm = gsl_sf_legendre_sphPlm(l, std::abs(m), std::cos(theta)) * std::cos(m * phi);

    if (m < 0)
        return std::pow(-1.0, m) * Ylm(l, -m, theta, phi);

    return Plm;
}



//----------------------------
//  ROTATE THE WHOLE NUCLEUS
//----------------------------




//-----------------------------------------
// OTHER FUNCTIONS I'M NOT SURE ARE HELPFUL
//-----------------------------------------

void sampleSphereSurface(int N, int l, int m){
    // This function creates N^2 points over a unitary sphere and evaluates them on the 
    // spherical harmonics lm
    using namespace std;
    for (double theta = 0; theta < M_PI; theta += (M_PI / double(N))){ 
            for (double phi = -M_PI; phi < M_PI; phi += (M_PI / double(N))){
                 
                double x = sin(theta) * cos(phi);
                double y = sin(theta) * sin(phi);
                double z = cos(theta);

               cout << x << "," << y << "," << z << "," << Ylm(l, m, theta, phi) << endl;            
            }
        }
    return; 
}
