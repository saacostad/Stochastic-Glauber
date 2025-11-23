#pragma once

#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <sstream>

#include "nucleonSampling.h"
#include "createNucleus.h"


// Fast function to get the nucleon radius given a Cross Section
double NucleonRadius(double Snn){return std::sqrt(Snn / M_PI);}

std::unordered_map<double, double> read_NN_CrossSection(std::string path){
    /*  This function is in charge of executing and reading the data for the 
     *  nucleon-nucleon Cross Section found in the particle group dataset.
     *
     *  Returns a dictionary-like object for easy access to the data
     */ 
    std::unordered_map<double, double> data;
    std::ifstream file(path);

    if (!file.is_open()) {
        system("python outils/getNNCrossSection.py");
        file.close();

        file.open(path);

        if(!file.is_open()){
            std::cerr << "Problem when reading nucleon-nucleon Cross-Section data" << std::endl;
            return data;
        }
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        std::string value_str;
        
        if (std::getline(ss, key, ',') && std::getline(ss, value_str)) {
            try {
                double key_p = std::stod(key);
                double value = std::stod(value_str);
                data[key_p] = value;
            } catch (const std::invalid_argument&) {
                std::cerr << "Warning: invalid number for key " << key << "\n";
            }
        }
    }

    file.close();

    return data;
}




template<size_t N, size_t M>
void fillSphericalHarmonicsMatrix(std::string path, double (&matrix)[N][M]) 
{
    /* This function reads the Spherical Harmonics Expansion data for the nuclei.
     * his data is found in the SHE directory */

    std::ifstream file(path);
    std::string line;
    
    if (!file.is_open()){
        std::cerr << "Fuck my silly life" << std::endl;
    }

    int row_counter = 0;
    int column_counter = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        column_counter = 0; 
        while (std::getline(ss, value, ' ')){
            matrix[row_counter][column_counter] = std::stod(value);
            column_counter += 1;
        }

        row_counter++;
    }

    file.close();

}


void fillRadialDensityParams(std::string path, double (&vector)[4]){
    /* Similarly, this function fills up the radial density vector info
     * given the information found in the respective files trad.csv and prad.csv 
     * inside of the SHE folder */

    std::ifstream file(path);
    std::string line;
    
    if (!file.is_open()){
        std::cerr << "Fuck my silly life" << std::endl;
    }

    int row_counter = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ' ')){
            vector[row_counter] = std::stod(value);
            row_counter++;
        }
    }

    file.close();
}




void writeSystem(   std::string path, 
                    double Ecm, double Snn, 
                    int tarjetNucleus, int tarjetNucleons, 
                    int projectileNucleus, int projectileNucleons)
{
    /* This funtion creates a system given of nuclei given their collision energy, 
     * the number of Nuclei to create and the number of nucleons per nucleus. 
     */
   
    using namespace std;

    double nucleon_radius = NucleonRadius(Snn);
    
    // CREATION OF THE PARAMETERS FOR EACH NUCLEUS
    double tarjet_SH_positive_coef[7][7];
    double tarjet_SH_negative_coef[7][7];
    double tarjet_rad_density_params[4];
    //
    // string tarjet_path_SH_pos_coef = "SHE/tp.csv";
    // string tarjet_path_SH_neg_coef = "SHE/tn.csv";
    // string tarjet_path_rad_den = "SHE/trad.csv";

    string tarjet_path_SH_pos_coef = path + "/tp.csv";
    string tarjet_path_SH_neg_coef = path + "/tn.csv";
    string tarjet_path_rad_den = path + "/trad.csv";
    
    fillSphericalHarmonicsMatrix(tarjet_path_SH_pos_coef, tarjet_SH_positive_coef);
    fillSphericalHarmonicsMatrix(tarjet_path_SH_neg_coef, tarjet_SH_negative_coef);
    fillRadialDensityParams(tarjet_path_rad_den, tarjet_rad_density_params);

        
    double projectile_SH_positive_coef[7][7];
    double projectile_SH_negative_coef[7][7];
    double projectile_rad_density_params[4];

    // string projectile_path_SH_pos_coef = "SHE/pp.csv";
    // string projectile_path_SH_neg_coef = "SHE/pn.csv";
    // string projectile_path_rad_den = "SHE/prad.csv";

    string projectile_path_SH_pos_coef = path + "/pp.csv";
    string projectile_path_SH_neg_coef = path + "/pn.csv";
    string projectile_path_rad_den = path + "/prad.csv";
    
    fillSphericalHarmonicsMatrix(projectile_path_SH_pos_coef, projectile_SH_positive_coef);
    fillSphericalHarmonicsMatrix(projectile_path_SH_neg_coef, projectile_SH_negative_coef);
    fillRadialDensityParams(projectile_path_rad_den, projectile_rad_density_params);


    // SYSTEM'S CREATION

    ostringstream tarjetPath;
    ostringstream projPath;

    tarjetPath << path << "/" << Ecm << "/targets.csv";
    projPath << path << "/" << Ecm << "/projectiles.csv";

    create_file_nucleus(tarjetPath.str(), tarjetNucleus, tarjetNucleons, nucleon_radius, tarjet_SH_positive_coef, tarjet_SH_negative_coef, projectile_rad_density_params);
    cerr << "FINISHED WRITTING TO FILE " << tarjetPath.str() << endl; 
    create_file_nucleus(projPath.str(), projectileNucleus, projectileNucleons, nucleon_radius, projectile_SH_positive_coef, projectile_SH_negative_coef, projectile_rad_density_params);
    cerr << "FINISHED WRITTING TO FILE " << projPath.str() << endl;

}





void readSystem(std::string path, double E, std::vector<Nucleus> (&tarjets), std::vector<Nucleus> (&projectiles), int tarjet_nucleons, int projectile_nucleons){
    /* This functions reads a whole system and saves it's values inside of std::vector<Nucleus> */ 

    std::ostringstream tarPath, projPath;

    tarPath << path << "/" << E << "/" << "targets.csv";
    projPath << path << "/" << E << "/" << "projectiles.csv";

    tarjets = read_file_nucleus(tarPath.str(), tarjet_nucleons); 
    projectiles = read_file_nucleus(projPath.str(), projectile_nucleons);

    return;
}
