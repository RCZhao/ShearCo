//
//  random_number.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "random_number.hpp"


//get a seed for a random number
void seed()
{
    srand( static_cast<unsigned int>(time(NULL)) ) ;
}

// create a random number in an intervall (a,b)
double unifRand(double a, double b)
{
    return (b-a)*rand() / double(RAND_MAX) + a ;
}

// create a random number modulus to an input number
int GetRand(const unsigned int modulo)
{
    static int Init = 0 ;
    int randnum ;
    
    if (Init == 0)
    {
        seed() ;
        Init = 1 ;
    }
    
    randnum = rand() % modulo ;
    
    return randnum;
}


double unif_real_Rand(const double a, const double b)
{
    std::uniform_real_distribution<double> randU(a, b);
    return randU(random_generator);
}

int unif_int_Rand(const unsigned int n)
{
    std::uniform_int_distribution<int> randU_int(0, n);
    return randU_int(random_generator);
}

double gaussian_Rand(const double mean, const double sigma)
{
    std::normal_distribution<double> gaussian(mean, sigma);
    return gaussian(random_generator);
}