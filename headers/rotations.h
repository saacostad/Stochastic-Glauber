#pragma once 
#include <stdio.h>
#include <random>
#include <array>
#include <cmath>
#include "nucleonSampling.h"

constexpr std::array<double,3> operator/(
    const std::array<double,3>& v, double s)
{
    return { v[0] / s, v[1] / s, v[2] / s };
}
constexpr std::array<double,3> operator*(
    const std::array<double,3>& v, double s)
{
    return { v[0] * s, v[1] * s, v[2] * s };
}

// DOT PRODUCT BETWEEN TWO NUCLEONS
constexpr double dot(const std::array<double,3>& a,
                     const std::array<double,3>& b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}



// CROSS PRODUCT BETWEEN THE NUCLEON AND THE Z AXIS
constexpr std::array<double,3> cross(const std::array<double,3>& v)
{
    return { v[1], -v[0], 0.0 };
}


// NORM OF A VECTOR
constexpr double norm(const std::array<double,3>& v)
{
    return std::sqrt(dot(v, v));
}



// Definition of a quaternion 
struct Quaternion {
    double w, x, y, z;

    Quaternion conjugate() const {
        return { w, -x, -y, -z };
    }

    Quaternion operator*(const Quaternion& q) const {
        return {
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        };
    }

    
    void print() const {
        std::cout << "(" 
                  << w << ", "
                  << x << "i, "
                  << y << "j, "
                  << z << "k)";
    }
};



Quaternion createQuaternion(double theta, double phi){

    std::array<double, 3> v{sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};

    std::array<double, 3> u = cross(v);
    u = norm(u) != 0 ? u / norm(u) : u; 

    
    double beta = -acos( v[2] / norm(v) );

    double s_beta = sin(beta / 2.0);

    Quaternion q{cos(beta / 2.0), u[0] * s_beta, u[1] * s_beta, u[2] * s_beta };
    

    return q; 
}





Nucleus rotateNucleus(Nucleus NUC, Quaternion q){

    Nucleus rotatedNucleus; 

    for (Nucleon nuc : NUC){

        Quaternion qnuc{0.0, nuc[0], nuc[1], nuc[1]};       

        Quaternion qnuc_rotated = q * Quaternion{0.0, nuc[0], nuc[1], nuc[2]} * q.conjugate();

        rotatedNucleus.push_back(Nucleon{qnuc_rotated.x, qnuc_rotated.y, qnuc_rotated.z});

    }

    return rotatedNucleus;

}
