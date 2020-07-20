//
//  config.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "config.hpp"

//***// if a config file exists, read it
void read_config_file(std::string& config, std::string& input_lens_file, std::string& input_source_file, std::string& output_data_dir, int& N_pix, double& theta_in, double& theta_out, int& N_annuli, std::string& bin_type, bool bin_in_R, double& conv_R2theta)
{
  // check if a config file exists if not redirect to 'create_config_file()'
  if (config == "config")
  {
    std::ifstream cfile(config) ;
    if (!cfile.good())
      create_config_file(config) ;
    cfile.close() ;
  }
  else
  {
    std::ifstream cfile(config) ;
    if (!cfile.good())
    {
      std::cerr << "\nWARNING: the specified configuration file " << config << " does not exist\ndo you want to create one now? [yes, no]" << std::endl ;

      std::string newconfig ;
      std::cin >> newconfig ;

      bool conclusion = false ;
      int insuspicious_counter = 0 ;
      while(!conclusion)
        conclusion = yes_or_no(conclusion, insuspicious_counter, newconfig, config) ;
    }

  
    if (cfile.peek() == std::ifstream::traits_type::eof())
    {
      std::cerr << "\nWARNING: the specified configuration file " << config << " is empty\ndo you want to create one now? [yes, no]" << std::endl ;

      std::string newconfig ;
      std::cin >> newconfig ;

      bool conclusion = false ;
      int insuspicious_counter = 0 ;
      while(!conclusion)
        conclusion = yes_or_no(conclusion, insuspicious_counter, newconfig, config) ;
    }  

    cfile.close() ;
  }

  // at this point it's ensured that the config exists and it can be read
  std::ifstream cfile(config);
  std::string cline;

  // loop through the lines and assign variables
  while (std::getline(cfile, cline))
  {
    std::istringstream iss(cline);
    std::string cvar, cinput ;
    iss >> cvar >> cinput;
    
    if (cvar == "#")
      continue ;
    else if (cvar == "input_lens_file")
      input_lens_file = cinput ;
    else if (cvar == "input_source_file")
      input_source_file = cinput ;
    else if (cvar == "output_data_dir")
      output_data_dir = cinput ;
    else if (cvar == "N_pix")
      N_pix = std::stoi(cinput);
    else if (cvar == "theta_in")
      theta_in = std::stod(cinput);
    else if (cvar == "theta_out")
      theta_out = std::stod(cinput);
    else if (cvar == "N_annuli")
      N_annuli = std::stoi(cinput);
    else if (cvar == "bin_type")
      bin_type = cinput ;
    else if (cvar == "bin_in_R" && cinput == "true")
      bin_in_R = true ;
    else if(cvar == "conv_R2theta")
      conv_R2theta = std::stod(cinput);
  }

  // if a binning in comoving distance is wanted, the conversion factor to angular coordinates is needed
  // should be the last line in the config file 
  if (bin_in_R == true && conv_R2theta < 0.)
  {
    std::cout << "\nIf the binning should be in comoving distance, you need to specify the cosmology-dependent conversion factor from 'coord_units' to Mpc. This must be added in the config file as the last line, e.g., \n'conv_R2theta      0.03116' \nAlternatively, you can delete the config file and create a new one by running the code again.\n\nKeep the program running by manually adding the conversion factor now (units: catalog's angular coordinates to Mpc)." << std::endl ;
    std::cin >> conv_R2theta ;
  }

  // check contents of file for obvious mistakes and exit program
  bool config_cool = true ;
  if (input_lens_file.empty())
  {
    std::cerr << "\nERROR: there is no input_lens_file :(" << std::endl ;
    config_cool = false ;
  }
  if (input_source_file.empty())
  {
    std::cerr << "\nERROR: there is no input_source_file :(" << std::endl ;
    config_cool = false ;
  }
  if (output_data_dir.empty())
    output_data_dir = "./" ;
  if (bin_type.empty())
    bin_type = "log" ;
  if (N_pix == -1)
    N_pix = 4096 ;
  if (theta_in == -1)
  {
    std::cerr << "\nERROR: theta_in is not specified :(" << std::endl ;
    config_cool = false ;
  }
  if (theta_out == -1)
  {
    std::cerr << "\nERROR: theta_out is not specified :(" << std::endl ;
    config_cool = false ;
  }
  if (N_annuli == -1)
  {
    std::cerr << "\nERROR: N_annuli is not specified :(" << std::endl ;
    config_cool = false ;
  }
  if (bin_in_R == true && conv_R2theta < 1e-3)
    bin_in_R = false ;

  if (!config_cool)
    exit(1) ;

  //close file
  cfile.close();

  // // in case someone (including me) wants to see the input again at this point
  // std::cout << input_lens_file << std::endl;
  // std::cout << input_source_file << std::endl;
  // std::cout << output_data_dir << std::endl;
  // std::cout << N_pix << std::endl;
  // std::cout << theta_in << std::endl;
  // std::cout << theta_out << std::endl;
  // std::cout << N_annuli << std::endl;
  // std::cout << bin_type << std::endl;
  // std::cout << std::boolalpha << bin_in_R << std::endl;
  // std::cout << conv_R2theta << std::endl;
}



//***// if no config file exists yet, create one (dialogue style)
void create_config_file(std::string& config)
{
  // initialize the config input variables
  std::string input_lens_file, input_source_file, output_data_dir, bin_type ;
  double theta_in, theta_out, conv_R2theta ;
  int N_pix, N_annuli ;
  bool bin_in_R ;
  
  // open a new file called 'config'
  std::ofstream cfile(config);

  // onto the dialogue
  std::cout << "\nHello there, seems you lack a config file, so let's make one." << std::endl;
  std::cout << "\nFirst, I need the file where the lens galaxies are stored." << std::endl;
  std::cout << "a string like [/mypath/myfile.filetype]" << std::endl;
  std::cin >> input_lens_file;

  std::cout << "\nNow, your source galaxy file, please." << std::endl;
  std::cout << "a string like [/mypath/myfile.filetype]" << std::endl;
  std::cin >> input_source_file ;

  std::cout << "\nAny specific output data directory?" << std::endl ;
  std::cout << "a string like [/mypath/]" << std::endl ;
  std::cin >> output_data_dir ;

  std::cout << "\nThe calculation is done on a grid. What should be the size of the side of one grid?" << std::endl ;
  std::cout << "an int like [4096], I recommend 4096" << std::endl ;
  std::cin >> N_pix ;

  std::cout << "\nWhat is the minimum radius that should calculated..." << std::endl;
  std::cout << "a double in angular or comoving units, like [0.1 or 2]" << std::endl;
  std::cin >> theta_in;

  std::cout << "\n... and what is the maximum radius that should calculated?" << std::endl;
  std::cout << "a double in units of 'theta_in', like [10 or 15.7]" << std::endl;
  std::cin >> theta_out;

  std::cout << "\nHow many bins would you like?" << std::endl;
  std::cout << "an int like [30]" << std::endl;
  std::cin >> N_annuli;

  std::cout << "\nShould the binning be done in linearly-spaced bins or logarithmically-spaced bins?" << std::endl;
  std::cout << "choose from the strings [lin, log]" << std::endl;
  std::cin >> bin_type;

  std::cout << "\nAre the units of 'theta_in' and 'theta_out' in comoving coordinates but the catalog is given in angular coordinates?" << std::endl;
  std::cout << "choose from bools [true, false]" << std::endl;
  std::cin >> std::boolalpha >> bin_in_R ;

  // if a binning in comoving distance is wanted, the conversion factor to angular coordinates must be given, makes life easier for stacking the signal from different lens redshifts
  if( bin_in_R == true )
  {
    std::cout << "\nIn that case, you need to specify the conversion factor from the catalog's angular coordinates (e.g., deg) to Mpc." << std::endl;
    std::cout << "a double like [0.03116]" << std::endl;
    std::cin >> conv_R2theta;
  }
  
      
  // write config file
  cfile << "# foreground galaxy catalog file [string /mypath/myfile.filetype]" << std::endl ;
  cfile << "# format should be [xpos   ypos  0   0   weight]" << std::endl ;
  cfile << "input_lens_file     " << input_lens_file << std::endl ;
  cfile << "# background galaxy catalog file [string /mypath/myfile.filetype]" << std::endl ;
  cfile << "# format should be [xpos   ypos  shear1   shear2   weight]" << std::endl ;
  cfile << "input_source_file   " << input_source_file << std::endl ;
  cfile << "# output directory [string: /mypath/]" << std::endl ;
  cfile << "output_data_dir     " << output_data_dir << std::endl ;
  cfile << "# number of pixel, field size is N_pix*N_pix [int e.g., 4096]" << std::endl ;
  cfile << "N_pix               " << N_pix << std::endl ;
  cfile << "# inner radius in units of 'input coordinates' [double e.g., 0.1 or 2]" << std::endl ;
  cfile << "theta_in            " << theta_in << std::endl ;
  cfile << "# outer radius in units of 'input coordinates' [double e.g., 10 or 15.7]" << std::endl ;
  cfile << "theta_out           " << theta_out << std::endl ;
  cfile << "# number of bins [int e.g., 17 or 30]" << std::endl ;
  cfile << "N_annuli            " << N_annuli << std::endl ;
  cfile << "# logarithmic or linear binning [string: lin, log]" << std::endl ;
  cfile << "bin_type            " << bin_type << std::endl ;
  cfile << "# binning [bool: true, false]" << std::endl ;
  cfile << "bin_in_R            " << std::boolalpha << bin_in_R << std::endl ;
  if (bin_in_R == true)
  {
    cfile << "# conversion factor 'bin_in_R = true', units: catalog's angular coordinates (e.g., deg) to Mpc [double e.g., 0.03116]" << std::endl;
    cfile << "conv_R2theta        " << conv_R2theta << std::endl;
  }

  //motivate user
  std::cout << "\nGreat, all set and done. You can also edit the config file directly or delete it and repeat the process. :)\n" << std::endl;

  //close file, happy ending
  cfile.close();
}

bool yes_or_no (bool conclusion, int& sassy_remark, std::string& newconfig, std::string& config)
{
  conclusion = true ;

  if (newconfig == "y" || newconfig == "yes" || newconfig == "Yes" || newconfig == "YES" || newconfig == "yEs" )
    create_config_file(config) ;
  else if (newconfig == "n" || newconfig == "no" || newconfig == "No" || newconfig == "NO" || newconfig == "nO" )
    exit(1) ;
  else
  {
    conclusion = false ;
    if (sassy_remark < 2)
      std::cout << "I can do this all day\ndo you want to create one now? [yes, no]" << std::endl ;
    else if (sassy_remark < 4)
      std::cout << "what about now? [yes, no]" << std::endl ;
    else if (sassy_remark < 5)
      std::cout << "it's getting boring, so \ndo you want to create one now? [yes, no]" << std::endl ;
    else if (sassy_remark < 6)
      std::cout << "dO yOu WaNt To CrEaTe OnE nOw? [yEs, nO]" << std::endl ;
    else if (sassy_remark < 7)
      std::cout << "last call? yes or no???" << std::endl ;
    else if (sassy_remark < 8)
      std::cout << "this time I really mean it! yes or no?" << std::endl ;
    else if (sassy_remark < 9)
      std::cout << "..." << std::endl ;
    else if (sassy_remark < 10)
      std::cout << "still here?" << std::endl ;
    else
      std::cout << "someone here needs a new hobby and it's not me" << std::endl ;
    std::cin >> newconfig ;
    sassy_remark ++ ;

    if (sassy_remark == 11)
    {
      std::cout << "\nwelcome to the endless loop... muhahaha\n" << std::endl ;
      sassy_remark = 0 ;
    }
  }

  return conclusion ;
}