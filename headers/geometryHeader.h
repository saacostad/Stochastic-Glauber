#pragma once 
#include <iostream>
#include <gsl/gsl_sf_legendre.h>
#include <random> 
#include <cmath>
#include <vector> 


#include "densityFunctions.h"

// SPHERICAL HARMONICS 
double Ylm(int l, int m, double theta, double phi) {
    // Function that return the real part of the lm spherical harmonic evaluated in theta and phi 

    double Plm = gsl_sf_legendre_sphPlm(l, std::abs(m), std::cos(theta)) * std::cos(m * phi);

    if (m < 0)
        return std::pow(-1.0, m) * Ylm(l, -m, theta, phi);

    return Plm;
}


// ------------------------------
// NUCLEON SAMPLING 
// ------------------------------

std::vector<double> angularDistribution(std::mt19937& gen){
    // Returns two doubles: the position in theta and the position in phi sampled
    // uniformly in a unitary sphere
    using namespace std; 

    uniform_real_distribution<double> dist_phi(0.0, 2.0 * M_PI);
    uniform_real_distribution<double> dist_z(-1.0, 1.0); 

    double phi = dist_phi(gen);
    double theta = acos( dist_z(gen) );

    vector ret = {theta, phi};

    return ret;
}   



double radialSampling(double theta, double phi){
    // This function samples a nucleon radially located in the angular coordinates theta and phi
    // according to the radial density function 
    
    

    return 0.0;
}



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
