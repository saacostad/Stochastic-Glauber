#pragma once 
#include <random> 
#include <omp.h>

// -------------------------------------
// INITIALIZE A GLOBAL RANDOM NUMBER GEN 
// -------------------------------------
// We are expecting to be using multithreading, so each one of our randoom number generators must be 
// safe inside it's own thread. We create, for each thread, a random engine to be passed by reference 
// to any function that needs random number generators. 
inline thread_local std::mt19937 eng{std::random_device{}() + omp_get_thread_num()};



// FERMI PDF OF 3 PARAMETERS
double Fermi3param_PDF(double r, double ferqReq,  double rho_0  = 1.0, double a = 1.0, double w = 0.0){
    using namespace std;
    return rho_0 * ( (1 + w * pow( r / ferqReq , 2) ) / (1 + exp( (r - ferqReq) / a )) );
}



double sup_lim(double x, double a, double w, double rsp){
    return (1 + w * std::pow(x / rsp, 2)) / (1 + ( std::pow( (x - rsp) / a , 2) ));
}


//--------------------------------
// SAMPLING FROM ARBITRARY 1D PDF 
//--------------------------------
double sample_from_pdf(std::mt19937 &gen, double rsp, double params[4])
{
    // This functions maps a uniform distibution to the F3P distribution using 
    // rejection method. 
    double rho_0 = params[1];
    double a = params[2];
    double w = params[3];
    double ylim;
    

    if (w < 0.001){
        ylim = Fermi3param_PDF(0.0, rsp, rho_0, a, w);
    } else {
       ylim = sup_lim(( 2 * ( w - 1 )*(a*a)*(rsp*rsp) + 2*w*(rsp*rsp*rsp*rsp) + std::sqrt( 16*w*(std::pow(a, 4) * std::pow(rsp, 4)) + std::pow( 2 * (w - 1) * a*a * rsp*rsp + 2*w*std::pow(rsp, 4) , 2) )) / ( 4*w*a*rsp*rsp), a, w, rsp);
    }

    
    
    // std::cerr << "PDF y limit: " << sup_lim(ylim, a, w, rsp) << std::endl;


    // TODO Still need to check the vertical and horizontal limits
    std::uniform_real_distribution<double> x_dist(0.0, 500.0);
    // std::uniform_real_distribution<double> y_dist(0.0, Fermi3param_PDF(0.0, rsp));
    std::uniform_real_distribution<double> y_dist(0.0, ylim);
    
    while (true) {
        double x = x_dist(gen);
        double y = y_dist(gen);
        double p = Fermi3param_PDF(x, rsp, params[1], params[2], params[3]);

        if (y < p)
            return x; // accepted sample
    }
}

 
