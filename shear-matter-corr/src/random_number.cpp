//
//  random_number.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "random_number.hpp"

//get a seed for a random number
void seed(const int thread_id)
{
    unsigned long time_since_epoch = RG::myclock::now().time_since_epoch().count();
    unsigned long rand_seed = (time_since_epoch*thread_id)%1000000000000000;
    RG::random_generator.seed(rand_seed);
}

// return a uniformaly distributed real number R \in [a, b).
double unif_real_Rand(const double a, const double b)
{
    std::uniform_real_distribution<double> randU(a, b);
    return randU(RG::random_generator);
}

// return a long int R \in [0, n) that follows the uniform discrete distribution.
unsigned long unif_int_Rand(const unsigned long n)
{
    std::uniform_int_distribution<unsigned long> randU_int(0, n-1);
    return randU_int(RG::random_generator);
}

// return a double that follows the Gaussian distribution with mean as the mean and sigma as the standard deviation
double gaussian_Rand(const double mean, const double sigma)
{
    std::normal_distribution<double> gaussian(mean, sigma);
    return gaussian(RG::random_generator);
}