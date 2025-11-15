#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <omp.h>

#include <string>
#include <fstream>


#include "headers/cxxopts.hpp"


// INCLUDES OF HANDMADE HEADERS 
#include "headers/densityFunctions.h"
#include "headers/geometryHeader.h"
#include "headers/nucleonSampling.h"
#include "headers/checkCollision.h"
#include "headers/createNucleus.h"

#include "headers/API.h"
#include "headers/rotations.h"




int main(int argc, char* argv[]){
    
    using namespace std;

    
    // We'll read the nucleon-nucleon Cross Section data we have
    string path_NN_CrossSection = "systems/NNCrossSection.csv"; 
    unordered_map<double, double> NN_CrossSection = read_NN_CrossSection(path_NN_CrossSection);
    double nucleon_radius = NucleonRadius(NN_CrossSection[200.0]); 

    /*_______________________________________________________________________________________________________ 
         NN_CrossSection contains the data of some of the nucleon-nucleon CrossSections at different energies 
    -------------------------------------------------------------------------------------------------------*/
 
    omp_set_nested(1);

    cxxopts::Options options("Stochastic Glauber", "Software to simulate Stochastic Glauber Nuclear Collisions for general shaped nuclei for different impact parameters");
    

    options.add_options()
        ("m, mode", "Select the mode to run the script: write/read", cxxopts::value<string>()->default_value("read"))
        ("i, input_path", "Path to the general system (Without the simulation energy)", cxxopts::value<string>()->default_value("systems/test"))
        ("o, output_path", "Path to save the result of the software", cxxopts::value<string>()->default_value("data.csv"))
        ("E, energy", "Energy of the collision (given in GeV, must be a multiple of 10)", cxxopts::value<double>()->default_value("200.0"))
        ("T, target_nuclei", "How many tarjet nuclei to simulate (only for write mode)", cxxopts::value<int>()->default_value("50"))
        ("P, projectile_nuclei", "How many projectile nuclei to simulate (only for write mode)", cxxopts::value<int>()->default_value("50"))
        ("t, target_nucleons", "How many nucleons are in tarjet nucleus (needed for both modes)", cxxopts::value<int>()->default_value("197"))
        ("p, projectile_nucleons", "How many projectile nuclei to simulate (needed for both modes)", cxxopts::value<int>()->default_value("197"))
        ("b, max_impact_parameter", "Maximum impact parameter b to simulate", cxxopts::value<double>()->default_value("20.0"))
        ("d, delta_impact_parameter", "Delta step to simulate the impact parameter", cxxopts::value<double>()->default_value("0.1"))
        ("r, rotations", "Number of rotations to apply per nucleus", cxxopts::value<int>()->default_value("1"))
        ("h, help", "Print help")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }


    string mode = result["mode"].as<string>();
    string sysPath = "systems/" + result["input_path"].as<string>();
    double energy = result["energy"].as<double>();
    int tarNucleons = result["target_nucleons"].as<int>();
    int projNucleons = result["projectile_nucleons"].as<int>();


    if (mode == "write"){
        int tarNuclei = result["target_nuclei"].as<int>();
        int projNuclei = result["projectile_nuclei"].as<int>();


        writeSystem(sysPath, energy, NN_CrossSection[energy], tarNuclei, tarNucleons, projNuclei, projNucleons);

    } else if (mode == "read"){
        
        double bMax = result["max_impact_parameter"].as<double>();
        double bDelta = result["delta_impact_parameter"].as<double>();
        string outPath = result["output_path"].as<string>();

        std::vector<Nucleus> tarjets, projectiles;
        readSystem(sysPath, energy, tarjets, projectiles, tarNucleons, projNucleons);

        ofstream file(outPath);

        if (!file){
            cerr << "ERROR WHEN OUTPUTING FILE \n";
            return 1;

        }

        vector<Quaternion> quaternions_tarjet;
        vector<Quaternion> quaternions_projectile;

        int no_rot = result["rotations"].as<int>();
        
        for (int i = 1; i <= no_rot; i++){
            vector<double> angs = angularDistribution(eng);
            double theta = angs[0];
            double phi = angs[1];


            // double theta = 0.0;
            // double phi = 0.0;
            quaternions_tarjet.push_back(createQuaternion(theta, phi));
        }

        for (int i = 1; i <= no_rot; i++){
            vector<double> angs = angularDistribution(eng);
            double theta = angs[0];
            double phi = angs[1];
                
            // double theta = 0.0;
            // double phi = 0.0;
            quaternions_projectile.push_back(createQuaternion(theta, phi));
        }

        // for the progress bar
        int totalSteps = static_cast<int>((bMax) / bDelta) + 1;
        int currentStep = 0;

        for (double b = 0.0; b <= bMax; b += bDelta){

            long int collision_counter = 0;
            long long int NO_collissions = 0;
            long long int NO_participants_1 = 0;

            #pragma omp parallel for reduction(+:NO_collissions, NO_participants_1, collision_counter) schedule(dynamic) collapse(4)
            for (size_t i = 0; i < tarjets.size(); ++i) {
                for (size_t j = 0; j < projectiles.size(); ++j) {

                    const Nucleus& nuc_tarjet = tarjets[i];
                    const Nucleus& nuc_projectile = projectiles[j];
                    
                    for (const auto& qt : quaternions_tarjet){
                        for (const auto& qp : quaternions_projectile){
                            // array<int, 3> ret = checkCollisions(nuc_projectile, nuc_tarjet, b, nucleon_radius);
                            const array<int, 3> ret = checkCollisions(
                                    rotateNucleus(nuc_projectile, qp), 
                                    rotateNucleus(nuc_tarjet, qt), 
                                    b, nucleon_radius);
                            NO_collissions += ret[0];
                            NO_participants_1 += ret[1] + ret[2];
                            
                            collision_counter++;

                        }
                    }
                }
            }  
            // Progress bar AFTER the parallel region (safe!)
            currentStep++;
            float progress = static_cast<float>(currentStep) / totalSteps;
            int barWidth = 50;
            int pos = static_cast<int>(barWidth * progress);
            
            std::cout << "[";
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << "%\r";
            std::cout.flush();

            file << b << "," << double(NO_collissions) / double(collision_counter) << "," << double(NO_participants_1) / double(collision_counter) << "\n";

        }

        cout << "\n";
        file.close();

    } else{

        cerr << "NO VALID USAGE OF PROGRAM \n";
        return 1;

    }


    return 0;
}


