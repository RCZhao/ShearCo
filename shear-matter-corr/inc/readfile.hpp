//
//  readfile.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef readfile_hpp
#define readfile_hpp

#include <iostream>
#include <fstream>
#include <vector>

#include "structdefs.hpp"


//***// read ascii file of lenses
void read_lens_file(double *&xpos, double *&ypos, double *&weight, int& number_of_galaxies, const std::string& input_file) ;

//***// read ascii file of sources
void read_source_file(double *&xpos, double *&ypos, double *&shear1, double *&shear2, double *&weight, int& number_of_galaxies, const std::string& input_file) ;


#endif /* readfile_hpp */