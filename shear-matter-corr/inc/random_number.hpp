//
//  random_number.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef random_number_hpp
#define random_number_hpp

#include <iomanip>

//get a seed for a random number
void seed() ;

// create a random number in an intervall (a,b)
double unifRand(double a, double b)   ;

// create a random number modulus to an input number
int GetRand(const unsigned int modulo) ;

#endif /* random_number_hpp */
