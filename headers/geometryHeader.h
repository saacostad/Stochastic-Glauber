#pragma once 
#include <iostream>
#include <gsl/gsl_sf_legendre.h>
#include <random> 
#include <cmath>
// void test(std::string text){
//     std::cout << text << std::endl;
//     return;
// }


double Ylm(int l, int m, double theta, double phi) {
    // Function that return the lm spherical harmonic evaluated in theta and phi 

    double Plm = gsl_sf_legendre_sphPlm(l, std::abs(m), std::cos(theta)) * std::cos(m * phi);

    if (m < 0)
        return std::pow(-1.0, m) * Ylm(l, -m, theta, phi);

    return Plm;
}


//
// double radialSample():
//     // Returns a double which is a sample from the density distribution \rho(r)
