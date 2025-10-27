#pragma once
#include <random>
#include <cmath>
#include <vector>
#include <array>

#include "densityFunctions.h"


typedef std::array<double, 3> Nucleon;
typedef std::vector<Nucleon> Nucleus;

// ------------------------------
// NUCLEON SAMPLING 
// ------------------------------


// ANGULAR SAMPLING: uniform angularly
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



// RADIAL SAMPLING: depends on the density function and it is scaled according to the spherical harmonic
template <size_t N, size_t M>
double radialSampling(  std::mt19937 &gen, double theta, double phi,
                        double (&SH_positive_coef)[N][M],
                        double (&SH_negative_coef)[N][M],
                        double *rad_density_params){

    // This function samples a nucleon radially located in the angular coordinates theta and phi
    // according to the radial density function 
    
    
    // rad_density_params = { R_0,    rho_0 * \frac{ 1 + w * (r / (R_0 + Ylm)) }{ 1 + exp( (r - (R_0 + Ylm)) / a )}  }

    double general_Ylm = rad_density_params[0];

    for (int i = 0; i < N; i++){
        for(int j = 0; j <= i; j++){
            // std::cerr << i << ", " << j << ": " << SH_positive_coef[i][j] << "|" << Ylm(i, j, theta, phi) << "|" << std::endl;
            general_Ylm += SH_positive_coef[i][j] * Ylm(i, j, theta, phi);
            general_Ylm += SH_negative_coef[i][j] * Ylm(i, -j, theta, phi);
        }
    }

    return sample_from_pdf(gen, general_Ylm, rad_density_params);
}







// NUCLEON SAMPLING 
template <size_t N, size_t M>
std::array<double, 3> sampleNucleon(std::mt19937 &gen, Nucleus prevNucleus,
                                    double nuc_rad_4pow2,
                                    double (&SH_positive_coef)[N][M],
                                    double (&SH_negative_coef)[N][M],
                                    double *rad_density_params){

    while(true){
        // This function creates by its own a new nucleon randomly
        using namespace std; 

        vector angles = angularDistribution(gen);

        double theta = angles[0];
        double phi = angles[1];

        double rho = radialSampling(gen, theta, phi,
                                    SH_positive_coef, SH_negative_coef, 
                                    rad_density_params);

        // double rho = 1.0;

        // vector<double> ret = {rho, theta, phi};

        array<double, 3> ret = {   rho * sin(theta) * cos(phi),
                                   rho * sin(theta) * sin(phi),
                                   rho * cos(theta)};
        
        
            bool valid = true;
        for (const Nucleon &nuc : prevNucleus) {
            double dx = ret[0] - nuc[0];

            double dy = ret[1] - nuc[1];
            double dz = ret[2] - nuc[2];

            // double dy = ret[1] + nuc[1];
            // double dz = ret[2] + nuc[2];

            if (dx*dx + dy*dy + dz*dz < nuc_rad_4pow2) {
                valid = false;
                break;
            }
        }

        if (valid)
            return ret;  // ✅ valid nucleon found — done    }
    } 
}




//-----------------------------
//  NUCLEUS SAMPLING 
// SAMPLE THE WHOLE NUCLEUS
//-----------------------------
template <size_t N, size_t M>
std::vector<std::array<double, 3>> createNucleus(   int A, double nucleon_radius,
                                                    double (&SH_positive_coef)[N][M],
                                                    double (&SH_negative_coef)[N][M],
                                                    double *rad_density_params,
                                                    std::mt19937 &eng
                                                ){
                        
    std::vector<std::array<double, 3>> nucleus;

    for (int i = 1; i <= A; i++){
        std::array<double, 3> nucleon = sampleNucleon(
                eng, nucleus, 4 * pow(nucleon_radius, 2),
                SH_positive_coef, SH_negative_coef,
                rad_density_params);

        // TODO make a function to evaluate the proximity of each nucleon 

        nucleus.push_back(nucleon);
    }
    

    return nucleus;
}
