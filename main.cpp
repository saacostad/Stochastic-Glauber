#include <iostream>
#include <cmath>
#include <vector>
#include <array>


#include <string>
#include <fstream>


// INCLUDES OF HANDMADE HEADERS 
#include "headers/densityFunctions.h"
#include "headers/geometryHeader.h"
#include "headers/nucleonSampling.h"
#include "headers/checkCollision.h"



const double sigma_nn = 10.0;
double nucleon_radius = std::sqrt(sigma_nn / M_PI);



int main(void){
    
    using namespace std;
    
    double radius = 1.0; 

    double SH_positive_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};
    double SH_negative_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};

    // Rad_0, rho_0, a, w
    double rad_density_params[4] = {{0.1}, {1.0}, {1.0}, {1.0}};

    Nucleus nucleus = createNucleus(20,  nucleon_radius, SH_positive_coef, SH_negative_coef, rad_density_params);  
  


    double SH_positive_coef2[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{100.0, 0.0, 0.0}};
    Nucleus nucleus2 = createNucleus(20,  1.0, SH_positive_coef2, SH_negative_coef, rad_density_params);  
    
    ofstream file("nucleus1.csv");
    
    for (Nucleon nucleon : nucleus){
        file << nucleon[0] << "," << nucleon[1] << "," << nucleon[2] << endl;
    }

    file.close();

    ofstream file2("nucleus2.csv");

    for (Nucleon nucleon : nucleus2){
        double coorChange = nucleon[2];
        file2 << nucleon[0] << "," << nucleon[1] << "," << coorChange << endl;
    }

    file2.close();
    
    array<int, 4> col_params = checkCollisions(nucleus, nucleus2, 0.0, 4*pow(nucleon_radius,2));

    cerr << "N. Collision 1: " << col_params[0] << endl;
    cerr << "N. Participants 1: " << col_params[2] << endl;
    cerr << "N. Collision 2: " << col_params[1] << endl;
    cerr << "N. Participants 2: " << col_params[3] << endl;
    return 0;
}


