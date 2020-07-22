//
//  galassign.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef galassign_hpp
#define galassign_hpp


#include <iostream>

#include "random_number.hpp"
#include "readfile.hpp"
#include "writefile.hpp"


//***// assigns positions from lens galaxies and shear from source galaxies to the pixel grid
void gals2pos(std::vector<double>& lens_density, std::vector<double>& source_density, std::vector<std::complex<double>>& gamma12, double& number_of_lenses, double& number_of_sources, double& field_size, const int N_pix, const std::string& input_lens_file, const std::string& input_source_file, const std::vector<tmean_type> theta, const int N_annuli, const std::string& units_output, const std::string output_data_dir) ;


//***// selects random lenses (~70% of all pixels)
std::vector<double> lens2pos_forLSS(std::vector<double>& lens_density, const int N_pix);


#endif /* galassign_hpp */