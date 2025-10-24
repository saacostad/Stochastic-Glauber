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
#include "headers/createNucleus.h"


const double sigma_nn = 10.0;
double nucleon_radius = std::sqrt(sigma_nn / M_PI);



int main(void){
    
    using namespace std;
    

    // CREATE NUCLEUS PARAMETERS
    double SH_positive_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};
    double SH_negative_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};

    // Rad_0, rho_0, a, w
    double rad_density_params[4] = {{0.1}, {1.0}, {1.0}, {1.0}};
    
    std::string path = "systems/nucleusTest.csv";
    create_file_nucleus(path, 2, 5, nucleon_radius, SH_positive_coef, SH_negative_coef, rad_density_params);
    cerr << "FINISHED WRITTING TO FILE" << endl; 
    

    vector<Nucleus> data = read_file_nucleus(path, 5); 

    // WRITE NUCLEUS DATA TO A FILE 
    ofstream file("systems/nucleus1.csv");
    
    for (Nucleon nucleon : data[0]){
        file << nucleon[0] << "," << nucleon[1] << "," << nucleon[2] << endl;
    }

    file.close();

    // WRITE THE OTHER NUCLEUS DATA TO A FILE 
    ofstream file2("systems/nucleus2.csv");

    for (Nucleon nucleon : data[1]){
        double coorChange = nucleon[2];
        file2 << nucleon[0] << "," << nucleon[1] << "," << coorChange << endl;
    }

    file2.close();

    return 0;
}


