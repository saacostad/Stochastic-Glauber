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



struct Stats {
    long long n = 0;
    double mean = 0.0;
    double M2 = 0.0;

    void update(double x) {
        n++;
        double delta = x - mean;
        mean += delta / n;
        double delta2 = x - mean;
        M2 += delta * delta2;
    }

    void merge(const Stats& other) {
        if (other.n == 0) return;
        long long n_total = n + other.n;
        double delta = other.mean - mean;

        mean += delta * other.n / n_total;
        M2 += other.M2 + delta * delta * n * other.n / n_total;

        n = n_total;
    }
};



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

        ostringstream path;

        path << sysPath << "/" << energy << "/" << outPath;
        
        std::cout << path.str() << std::endl;
        ofstream file(path.str());

        if (!file){
            cerr << "ERROR WHEN OUTPUTING FILE \n";
            return 1;

        }


        int no_rot = result["rotations"].as<int>();

        const size_t total_combinations = no_rot * no_rot;
        const size_t total_iterations = tarjets.size() * projectiles.size() * total_combinations; 
        const double tot_systems = tarjets.size()*projectiles.size();


        vector<Nucleus> rotated_nucleus_proj;
        vector<Nucleus> rotated_nucleus_tarj;
       
        rotated_nucleus_tarj.reserve(tarjets.size() * no_rot);
        rotated_nucleus_proj.reserve(projectiles.size() * no_rot);

        for (int i = 1; i <= no_rot; i++){
            for (const auto &nuc : tarjets){
                vector<double> angs = angularDistribution(eng);
                double theta = angs[0];
                double phi = angs[1];

                rotated_nucleus_tarj.push_back(rotateNucleus( nuc , createQuaternion(theta, phi)));
            }
        }

        for (int i = 1; i <= no_rot; i++){
            for (const auto &nuc : projectiles){
                vector<double> angs = angularDistribution(eng);
                double theta = angs[0];
                double phi = angs[1];

                rotated_nucleus_proj.push_back(rotateNucleus( nuc , createQuaternion(theta, phi)));
            }
        }
        
        cout << "Finished rotating nuclei \n ---------------------" << endl;

        // for the progress bar
        int totalSteps = static_cast<int>((bMax) / bDelta) + 1;
        int currentStep = 0;

        for (double b = 0.0; b <= bMax; b += bDelta){

            long int collision_counter = 0;
            long long int NO_collissions = 0;
            long long int NO_participants_1 = 0;
            
            Stats g_cols, g_part;

            #pragma omp parallel
            {
            
                Stats l_cols, l_parts;

                #pragma omp for reduction(+:NO_collissions, NO_participants_1, collision_counter) schedule(dynamic) 
                for (size_t idx = 0; idx < tarjets.size() * projectiles.size() * total_combinations; ++idx) {
                    // Compute indices from flattened index
                    size_t quat_combo = idx % total_combinations;
                    size_t temp = idx / total_combinations;
                    size_t j = temp % projectiles.size();
                    size_t i = temp / projectiles.size();
                    
                    size_t qp_idx = quat_combo % no_rot;
                    size_t qt_idx = quat_combo / no_rot;
                    
                    size_t rot_tarjet_idx = i * no_rot + qt_idx;
                    size_t rot_proj_idx = j * no_rot + qp_idx;
                    
                    const array<int, 3> ret = checkCollisions(
                        rotated_nucleus_proj[rot_proj_idx], 
                        rotated_nucleus_tarj[rot_tarjet_idx], 
                        b, nucleon_radius);   
                    
                    l_cols.update(ret[0]);
                    l_parts.update(ret[1] + ret[2]);


                    // NO_collissions += ret[0];
                    // NO_participants_1 += ret[1] + ret[2];
                    // collision_counter++;
                } 
                
                #pragma omp critical
                {
                    g_cols.merge(l_cols);
                    g_part.merge(l_parts);
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

            double std_dev_cols   = std::sqrt(g_cols.M2 / (g_cols.n - 1));
            double mean_cols      = g_cols.mean;
            double std_dev_parts   = std::sqrt(g_part.M2 / (g_part.n - 1));
            double mean_parts      = g_part.mean;
            
            double error_col = 1.96 / std::sqrt(tot_systems) * std_dev_cols;
            double error_part = 1.96 / std::sqrt(tot_systems) * std_dev_parts;

            file << b << "," << mean_cols << "," << error_col << "," << mean_parts << "," << error_part << "\n";

        }

        cout << "\n";
        file.close();

    } else{

        cerr << "NO VALID USAGE OF PROGRAM \n";
        return 1;

    }


    return 0;
}


