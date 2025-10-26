#pragma once 
#include <cmath>
#include <string> 
#include <fstream> 
#include <vector>
#include <sstream>
#include <omp.h> 
#include <iostream>
#include "nucleonSampling.h"
#include <filesystem>


    
template <size_t N, size_t M>
void create_file_nucleus(std::string path, 
                         int NO_nucleus, int NO_nucleons, double nucleon_radius, 
                         double (&SH_positive_coef)[N][M], double (&SH_negative_coef)[N][M],
                         double *rad_density_params){



    int nthreads = 1;
    #pragma omp parallel
    {
        #pragma omp single
        nthreads = omp_get_num_threads();
    }

    std::vector<std::ostringstream> buffers(nthreads);    

    #pragma omp parallel
    {
        thread_local std::mt19937 eng{std::random_device{}() + omp_get_thread_num()};

        int tid = omp_get_thread_num();

        auto &oss = buffers[tid];  // each thread’s personal buffer

        #pragma omp for
        for (size_t Nn = 0; Nn < NO_nucleus; Nn++) {
            //
            // int tid = omp_get_thread_num();   // thread id
            // auto &oss = buffers[Nn];         // private to this thread

            Nucleus nucleus = createNucleus(NO_nucleons, nucleon_radius, SH_positive_coef, SH_negative_coef, rad_density_params, eng);  

            for (const Nucleon &nucleon : nucleus){
                oss << nucleon[0] << "," << nucleon[1] << "," << nucleon[2] << std::endl;
            }

            // std::cerr << "Nucleus created" << std::endl;
        }
    }


    // Extract directory part of path
    std::filesystem::path dir = std::filesystem::path(path).parent_path();

    // Create directories if they don’t exist
    std::error_code ec;  // so it won’t throw on error
    std::filesystem::create_directories(dir, ec);

    if (ec) {
        std::cerr << "Error creating directories: " << ec.message() << "\n";
        return;
    }



    // 2️⃣ After the parallel section, write everything sequentially
    std::ofstream file(path);
    for (auto &oss : buffers) {
        file << oss.str();
    }
    file.close();
}



std::vector<Nucleus> read_file_nucleus(std::string path, int NO_nucleons){


    std::vector<Nucleus> data;

    std::ifstream file(path);
    std::string line;

    Nucleus nucleus;

    int counter = 0;
    while (std::getline(file, line)){
        
        if (counter == NO_nucleons){
            data.push_back(nucleus);
            counter = 0;
            nucleus.clear();
        }
        
        std::stringstream ss(line);                 // use stringstream to parse
        std::string value;

        Nucleon nucleon;
        
        int intern_counter = 0;
        while (std::getline(ss, value, ',')) {      // split by comma
            nucleon[intern_counter] = std::stod(value);
            intern_counter++;
        }
        intern_counter = 0;


        counter++;
        nucleus.push_back(nucleon);
    }
    
    return data;
} 
