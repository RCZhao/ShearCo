//
//  map_and_annuli.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef map_and_annuli_hpp
#define map_and_annuli_hpp

#include <iostream>
#include <vector>
#include <math.h>

#include "structdefs.hpp"

//***// calculates minimum number of pixel needed for the zero-padding method
int zero_padding_size(const double theta_out, const double field_size, double& field_size_fft, const int N_pix) ;

//***// calculate different annuli with linearly or logarithmically spaced bins in degree
std::vector<double> setup_annuli(std::vector<double>& annuli_radius, const double tin, const double tout, const int N_annuli, const std::string& bin_type) ;

//***// calculate the middle of an annulus [(inner+outer)/2]
std::vector<tmean_type> calc_thetamean(std::vector<tmean_type>& thetamean, const std::vector<double>& annuli_radius, const int N_annuli, const bool bin_in_R, const double conv_R2theta) ;

//***// calculate the true mean distance per annulus
// std::vector<tmean_type> calc_thetatruemean(std::vector<tmean_type>& thetamean, const std::vector<double>& lens_density, const std::vector<double>& source_density, const double tin, const double tout, const int pix_zp, const int N_annuli, const int N_pix) ;


#endif /* map_and_annuli_hpp */
