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