#pragma once 
#include <random> 


// -------------------------------------
// INITIALIZE A GLOBAL RANDOM NUMBER GEN 
// -------------------------------------
// We are expecting to be using multithreading, so each one of our randoom number generators must be 
// safe inside it's own thread. We create, for each thread, a random engine to be passed by reference 
// to any function that needs random number generators. 
inline thread_local std::mt19937 eng{std::random_device{}()};



// FERMI PDF OF 3 PARAMETERS
double Fermi3param_PDF(double r, double r_0, double w, double R, double a){
    using namespace std;
    return r_0 * ( (1 + w * pow( r / R , 2) ) / (1 + exp( (r - R) / a )) );
}



//--------------------------------
// SAMPLING FROM ARBITRARY 1D PDF 
//--------------------------------
double sample_from_pdf(std::mt19937 &gen, double r_0, double w, double R, double a)
{

    std::uniform_real_distribution<double> x_dist(0.0, 10.0);
    std::uniform_real_distribution<double> y_dist(0.0, 2.0);

    while (true) {
        double x = x_dist(gen);
        double y = y_dist(gen);
        double p = Fermi3param_PDF(x, r_0, w, R, a);

        if (y < p)
            return x; // accepted sample
    }
}

 
