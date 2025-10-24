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
double Fermi3param_PDF(double r, double ferqReq,  double rho_0  = 1.0, double a = 1.0, double w = 0.0){
    using namespace std;
    return rho_0 * ( (1 + w * pow( r / ferqReq , 2) ) / (1 + exp( (r - ferqReq) / a )) );
}



//--------------------------------
// SAMPLING FROM ARBITRARY 1D PDF 
//--------------------------------
double sample_from_pdf(std::mt19937 &gen, double rsp, double params[4])
{
    // This functions maps a uniform distibution to the F3P distribution using 
    // rejection method. 


    // TODO Still need to check the vertical and horizontal limits
    std::uniform_real_distribution<double> x_dist(0.0, 15.0);
    std::uniform_real_distribution<double> y_dist(0.0, Fermi3param_PDF(0.0, rsp));

    while (true) {
        double x = x_dist(gen);
        double y = y_dist(gen);
        double p = Fermi3param_PDF(x, rsp, params[1], params[2], params[3]);

        if (y < p)
            return x; // accepted sample
    }
}

 
