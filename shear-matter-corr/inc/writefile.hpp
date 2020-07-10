//
//  writefile.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef writefile_hpp
#define writefile_hpp

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "structdefs.hpp"


//***// write an output file for the tangential shear gamma(R) as a function of distance from the (stacked) lenses
void write_file_for_GammaR(const std::vector<tmean_type>& theta, const std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_Rlss, const int numlens, const int numsource, const int N_annuli, const std::string& output_data_dir) ;

//***// write an output file filled with zeros to keep consistency
void write_empty_output_file(const std::vector<tmean_type> theta, const int N_annuli, const std::string output_data_dir) ;

//***// output result to terminal for the tangential shear gamma(R) as a function of distance from the (stacked) lenses
void print_result(const std::vector<tmean_type>& theta, const std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_Rlss, const int numlens, const int numsource, const int N_annuli) ;

//***// output result to terminal with zeros to let people know
void print_empty_result(const std::vector<tmean_type>& theta, const int N_annuli) ;

//***// say hello "this is SHEARCO"
void sayhello() ;

#endif /* writefile_hpp */
