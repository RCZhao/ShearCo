//
//  calc_tshear.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef calc_tshear_hpp
#define calc_tshear_hpp

#include <iostream>
#include <fftw3.h>
#include <vector>
#include <math.h>

#include "structdefs.hpp"


//***// stacks (and averages) the shear fields behind all lens 
std::vector<std::complex<double>> do_FFT(std::vector<std::complex<double>>& gamma_g2l, const std::vector<std::complex<double>>& gamma12, const std::vector<double>& lens_density, const std::vector<double>& source_density, const int N_pix, const int N_fft) ;

//***// calculates the tangential and cross shear as a function of distance from the center
std::vector<std::complex<double>> calc_tshear(std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_g2l, const int pix_zp, const double field_size_fft, std::vector<double> annuli_radius, const int N_annuli, const std::string& bin_type, const int N_fft) ;


#endif /* calc_tshear_hpp */