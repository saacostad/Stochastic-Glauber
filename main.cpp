#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <omp.h>

#include <string>
#include <fstream>


// INCLUDES OF HANDMADE HEADERS 
#include "headers/densityFunctions.h"
#include "headers/geometryHeader.h"
#include "headers/nucleonSampling.h"
#include "headers/checkCollision.h"
#include "headers/createNucleus.h"

#include "headers/API.h"




int main(void){
    
    using namespace std;

    
    // We'll read the nucleon-nucleon Cross Section data we have
    string path_NN_CrossSection = "systems/NNCrossSection.csv"; 
    unordered_map<double, double> NN_CrossSection = read_NN_CrossSection(path_NN_CrossSection);


    /*_______________________________________________________________________________________________________ 
         NN_CrossSection contains the data of some of the nucleon-nucleon CrossSections at different energies 
    -------------------------------------------------------------------------------------------------------*/
 
    string sysPath = "systems/test";
    writeSystem(sysPath, 200.0, NN_CrossSection[200.0], 50, 197, 50, 197);

    return 0;
    // omp_set_nested(1);
    // std::string tar_path = "systems/Au79-Au79/targets.csv";
    // std::string proj_path = "systems/Au79-Au79/projectiles.csv";
    //
    // // CREATE NUCLEUS PARAMETERS
    // double SH_positive_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};
    // double SH_negative_coef[3][3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}};
    //
    // // Rad_0, rho_0, a, w
    // double rad_density_params[4] = {{6.38}, {1.0}, {0.535}, {0.0}};
    //
    //
    // create_file_nucleus(tar_path, 50, 197, nucleon_radius, SH_positive_coef, SH_negative_coef, rad_density_params);
    // cerr << "FINISHED WRITTING TO FILE" << endl; 
    // create_file_nucleus(proj_path, 50, 197, nucleon_radius, SH_positive_coef, SH_negative_coef, rad_density_params);
    // cerr << "FINISHED WRITTING TO FILE" << endl;
    //
    //
    //
    // vector<Nucleus> tarjets = read_file_nucleus(tar_path, 197); 
    // vector<Nucleus> projectiles = read_file_nucleus(proj_path, 197);
    //
    //
    //
    //
    // for (double b = 0.0; b <= 2 * rad_density_params[0] + 15.0; b += 0.05){
    //
    //     long int collision_counter = 1;
    //     long long int NO_collissions = 0;
    //     long long int NO_participants_1 = 0;
    //     long long int NO_participants_2 = 0;
    //
    //     #pragma omp parallel for reduction(+:NO_collissions, NO_participants_1, NO_participants_2, collision_counter)
    //     for (size_t i = 0; i < tarjets.size(); ++i) {
    //         for (size_t j = 0; j < projectiles.size(); ++j) {
    //
    //             Nucleus nuc_tarjet = tarjets[i];
    //             Nucleus nuc_projectile = projectiles[j];
    //
    //             array<int, 3> ret = checkCollisions(nuc_projectile, nuc_tarjet, b, nucleon_radius);
    //             NO_collissions += ret[0];
    //             // NO_participants_1 += ret[1];
    //             // NO_participants_2 += ret[2];
    //
    //             collision_counter++;
    //         }
    //     }  
    //
    //     cout << b << "," << double(NO_collissions) / double(collision_counter) << "\n";
    //
    // }
    //
    //
    //
    return 0;
}


