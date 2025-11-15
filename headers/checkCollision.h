#pragma once 
#include <cmath>
#include <vector> 
#include <array>

#include "nucleonSampling.h"

std::array<int, 3> checkCollisions(const Nucleus& nucleus1, const Nucleus& nucleus2, const double& b, const double& rad2){
    
    int N_coll_1 = 0;
    int N_part_1 = 0;

    for (auto &nuc1 : nucleus1){
        // bool part = 2;
    
        int og_N_coll = N_coll_1;

        for (auto &nuc2 : nucleus2){
            
            if ((pow(nuc1[0] - nuc2[0], 2) + pow((nuc1[1] + b) - nuc2[1], 2)) <= rad2){

                N_coll_1++;

                // N_part += part;
                // part = 1;

            }

        }

        if (og_N_coll != N_coll_1){ N_part_1++; }

    }


    int N_coll_2 = 0;
    int N_part_2 = 0;

    for (auto &nuc1 : nucleus2){
        // bool part = 2;
    
        int og_N_coll = N_coll_2;

        for (auto &nuc2 : nucleus1){
            
            if ((pow(nuc1[0] - nuc2[0], 2) + pow((nuc1[1] + b) - nuc2[1], 2)) <= rad2){

                N_coll_2++;

                // N_part += part;
                // part = 1;

            }

        }

        if (og_N_coll != N_coll_2){ N_part_2++; }

    }

    std::array<int, 3> ret = {N_coll_1, N_part_1, N_part_2};
   return ret; 
}




