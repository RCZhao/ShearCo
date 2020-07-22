//
//  config.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//***// if no config file exists yet, create one (dialogue style)
void create_config_file(std::string& config) ;

//***// if a config file exists, read it
void read_config_file(std::string& config, std::string& input_lens_file, std::string& input_source_file, double& theta_in, double& theta_out, int& N_annuli, std::string& output_data_dir, int& N_pix, std::string& bin_type, std::string&  units_input, std::string& units_output, bool& bin_in_R, double& conv_R2theta) ;

//***// handles the terminal dialogue to create a config file or not
bool yes_or_no(bool conclusion, int& sassy_remark, std::string& newconfig, std::string& config) ;


#endif /* config_hpp */
