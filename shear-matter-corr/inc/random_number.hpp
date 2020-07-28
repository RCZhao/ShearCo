//
//  random_number.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef random_number_hpp
#define random_number_hpp

// #include <iomanip>
#include "structdefs.hpp"

// //get a seed for a random number
// void seed() ;

// // create a random number in an intervall (a,b)
// double unifRand(double a, double b)   ;

// // create a random number modulus to an input number
// int GetRand(const unsigned int modulo) ;

void            seed(const int thread_id)                               ;
double          unif_real_Rand(const double a, const double b)          ;
unsigned long   unif_int_Rand(const unsigned long n)                    ;
double          gaussian_Rand(const double mean, const double sigma)    ;

#endif /* random_number_hpp */
