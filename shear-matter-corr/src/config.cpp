//
//  config.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "config.hpp"

//***// if a config file exists, read it
void read_config_file(std::string& config, std::string& input_lens_file, std::string& input_source_file, double& theta_in, double& theta_out, int& N_annuli, std::string& output_data_dir, int& N_pix, std::string& bin_type, std::string&  units_input, std::string& units_output, bool& bin_in_R, double& conv_R2theta)
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
      getline(std::cin, newconfig) ;

      bool conclusion = false ;
      int insuspicious_counter = 0 ;
      while(!conclusion)
        conclusion = yes_or_no(conclusion, insuspicious_counter, newconfig, config) ;
    }

  
    if (cfile.peek() == std::ifstream::traits_type::eof())
    {
      std::cerr << "\nWARNING: the specified configuration file " << config << " is empty\ndo you want to create one now? [yes, no]" << std::endl ;

      std::string newconfig ;
      getline(std::cin, newconfig) ;

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
    else if (cvar == "theta_in")
      theta_in = std::stod(cinput);
    else if (cvar == "theta_out")
      theta_out = std::stod(cinput);
    else if (cvar == "N_annuli")
      N_annuli = std::stoi(cinput);

    else if (cvar == "output_data_dir")
      output_data_dir = cinput ;
    else if (cvar == "N_pix")
      N_pix = std::stoi(cinput);
    else if (cvar == "bin_type")
      bin_type = cinput ;
    else if (cvar == "units_input" || cvar == "unit_input")
      units_input = cinput ;
    else if (cvar == "units_output" || cvar == "unit_output")
      units_output = cinput ;

    else if(cvar == "conv_R2theta")
      conv_R2theta = std::stod(cinput);
  }

  // if a binning in comoving distance is wanted, the conversion factor angular-to-comoving coordinates is needed
  // should be the last line in the config file 
  if (units_output == "Mpc" || units_output == "kpc")
  {
    if (units_input == "arcsec" || units_input == "arcmin" || units_input == "deg" || units_input == "rad")
    {
      bin_in_R = true ;

      if (conv_R2theta < 0. )
      {
        std::cout << "\nIf the binning should be in comoving distance, you need to specify the cosmology-dependent conversion factor from units_input " << units_input << " to units_output " << units_output << ". This must be added in the config file as the last line, e.g., \n'conv_R2theta      0.03116' \nAlternatively, you can delete the config file and create a new one by running the code again.\n\nKeep the program running by manually adding the conversion factor now (units: catalog's angular coordinates to comoving units)." << std::endl ;

        std::string user_input ;
        getline(std::cin, user_input) ;
        if (!user_input.empty())
          conv_R2theta = std::stod(user_input) ;
        else
        {
          std::cerr << "ERROR: No valid input has been given, stopping program here." << std::endl ;
          exit(1) ;
        }
      }
    }
  }
  // why would someone even have a catalogue in comoving coordinates but want binning in angular coordinates?
  else if (units_input == "Mpc" || units_input == "kpc")
  {
    if (units_output == "arcsec" || units_output == "arcmin" || units_output == "deg" || units_output == "rad")
    {
      std::cerr << "\nERROR: >,< sorry, I have not covered the conversion comoving-to-angular (" << units_input << "->" << units_output << ") coordinates as I did not imagine a science case, just drop me an email and I will implement it\nsandra.unruh@uni-bonn.de" << std::endl ;
      exit(1) ;
    }
  }
  
  // for all the indecisive people
  if (units_input == "?" && units_output != "?")
  {
    units_input = units_output ;

    std::cerr << "\nWARNING: units_input is implicitly set to units_output" << std::endl ;
  }
  // more indecisiveness is covered
  else if (units_input != "?" && units_output == "?")
  {
    units_output = units_input ;

    std::cerr << "\nWARNING: units_output is implicitly set to units_input" << std::endl ;
  }
  // for readibility
  else if (units_input == "?" && units_output == "?")
  {
    units_input  = "cat units" ;
    units_output = "cat units" ;
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

  if (!config_cool)
    exit(1) ;

  if (output_data_dir.empty())
    output_data_dir = "./" ;
  if (N_pix == -1)
    N_pix = 4096 ;
  if (bin_type.empty())
    bin_type = "log" ;
  if (units_input.empty())
    units_input = "?" ;
  if (units_output.empty())
    units_output = "?" ;
  
  if (bin_in_R == true && conv_R2theta < 1.e-6)
  {
    bin_in_R = false ;
    units_output = units_input ;

    std::cerr << "\nWARNING: the conversion factor from comoving to angular coordinates (" << conv_R2theta << ") is smaller than 1e-6, units_output is implicitly set to units_input" << std::endl ;
  }

  //close file
  cfile.close();

  // // in case someone (including me) wants to see the input again at this point
  // std::cout << input_lens_file << std::endl;
  // std::cout << input_source_file << std::endl;
  // std::cout << theta_in << std::endl;
  // std::cout << theta_out << std::endl;
  // std::cout << N_annuli << std::endl;
  // std::cout << output_data_dir << std::endl;
  // std::cout << N_pix << std::endl;
  // std::cout << bin_type << std::endl;
  // std::cout << units_input << std::endl;
  // std::cout << units_output << std::endl;
  // std::cout << std::boolalpha << bin_in_R << std::endl;
  // std::cout << conv_R2theta << std::endl;
}



//***// if no config file exists yet, create one (dialogue style)
void create_config_file(std::string& config)
{
  // initialize the config input variables
  std::string input_lens_file, input_source_file, output_data_dir, bin_type ;
  std::string units_input, units_output ;
  double theta_in, theta_out, conv_R2theta ;
  int N_pix, N_annuli ;
  
  // open a new file called 'config'
  std::ofstream cfile(config);
  std::string user_input ;

  // onto the dialogue
  std::cout << "\nHello there, seems you lack a config file, so let's make one." << std::endl;


  std::cout << "\nCompulsory entries first:" << std::endl;
  std::cout << "the file where the lens galaxies are stored." << std::endl;
  std::cout << "a string like [/mypath/myfile.filetype]" << std::endl;
  getline(std::cin, input_lens_file) ;

  std::cout << "\nNow, your source galaxy file, please." << std::endl;
  std::cout << "a string like [/mypath/myfile.filetype]" << std::endl;
  getline(std::cin, input_source_file) ;

  std::cout << "\nWhat is the minimum radius that should be calculated..." << std::endl;
  std::cout << "a double in angular or comoving units, like [0.1 or 2]" << std::endl;
  getline(std::cin, user_input) ;
  if (!user_input.empty())
    theta_in = std::stod(user_input) ;

  std::cout << "\n... and what is the maximum radius that should be calculated?" << std::endl;
  std::cout << "a double in units of 'theta_in', like [10 or 15.7]" << std::endl;
  getline(std::cin, user_input) ;
  if (!user_input.empty())
    theta_out = std::stod(user_input) ;

  std::cout << "\nHow many bins would you like?" << std::endl;
  std::cout << "an int like [30]" << std::endl;
  getline(std::cin, user_input) ;
  if (!user_input.empty())
    N_annuli = std::stoi(user_input) ;
  else
    N_annuli = -1 ;


  std::cout << "\nOptional entries next:" << std::endl;
  std::cout << "Any specific output data directory?" << std::endl ;
  std::cout << "a string like [/mypath/]" << std::endl ;
  std::cout << "default: ./" << std::endl ;
  getline(std::cin, output_data_dir) ;
  if (output_data_dir.empty())
    output_data_dir = "./" ;

  std::cout << "\nThe calculation is done on a grid. What should be the size of the side of one grid?" << std::endl ;
  std::cout << "an int like [4096], I recommend 4096" << std::endl ;
  std::cout << "default: 4096" << std::endl ;
  getline(std::cin, user_input) ;
  if (!user_input.empty())
    N_pix = std::stoi(user_input) ;
  else
    N_pix = 4096 ;

  std::cout << "\nShould the binning be done in linearly-spaced bins or logarithmically-spaced bins?" << std::endl;
  std::cout << "choose from the strings [lin, log]" << std::endl;
  std::cout << "default: log" << std::endl ;
  getline(std::cin, bin_type) ;
  if (bin_type.empty())
    bin_type = "log" ;

  std::cout << "\nWhat are the units of xpos/ypos in the input catalog?" << std::endl;
  std::cout << "choose from the strings [arcsec, arcmin, deg, rad, Mpc, kpc, ?]" << std::endl;
  std::cout << "default: ?" << std::endl ;
  getline(std::cin, units_input) ;
  if (units_input.empty())
    units_input = "?" ;

  std::cout << "\nWhat are the units for bins of the output?" << std::endl;
  std::cout << "choose from the strings [arcsec, arcmin, deg, rad, Mpc, kpc, ?]" << std::endl;
  std::cout << "default: ?" << std::endl ;
  getline(std::cin, units_output) ;
  if (units_output.empty())
    units_output = "?" ;
  

  // if a binning in comoving distance is wanted, the conversion factor to angular coordinates must be given, makes life easier for stacking the signal from different lens redshifts
  if (units_output == "Mpc" || units_output == "kpc")
    if (units_input == "arcsec" || units_input == "arcmin" || units_input == "deg" || units_input == "rad")
    {
      std::cout << "\nIn that case, you need to specify the conversion factor from angular to comoving coordinates, in " << units_input << "/" << units_output << "." << std::endl;
      std::cout << "a double like [0.03116]" << std::endl;
      getline(std::cin, user_input) ;
      conv_R2theta = std::stod(user_input) ;
    }
  
      
  // write config file
  cfile << "### compulsory ###" << std::endl ;
  cfile << "# foreground galaxy catalog file [string /mypath/myfile.filetype]" << std::endl ;
  cfile << "# format should be [xpos   ypos  0   0   weight]" << std::endl ;
  cfile << "input_lens_file     " << input_lens_file << std::endl ;
  cfile << "# background galaxy catalog file [string /mypath/myfile.filetype]" << std::endl ;
  cfile << "# format should be [xpos   ypos  shear1   shear2   weight]" << std::endl ;
  cfile << "input_source_file   " << input_source_file << std::endl ;
  cfile << "# inner radius in units of 'units_output' [double e.g., 0.1 or 2]" << std::endl ;
  cfile << "theta_in            " << theta_in << std::endl ;
  cfile << "# outer radius in units of 'units_output' [double e.g., 10 or 15.7]" << std::endl ;
  cfile << "theta_out           " << theta_out << std::endl ;
  cfile << "# number of bins [int e.g., 17 or 30]" << std::endl ;
  cfile << "N_annuli            " << N_annuli << std::endl ;

  cfile << "\n### optional ###" << std::endl ;
  cfile << "# output directory [string: /mypath/]" << std::endl ;
  cfile << "output_data_dir     " << output_data_dir << std::endl ;
  cfile << "# number of pixel, field size is N_pix*N_pix [int e.g., 4096]" << std::endl ;
  cfile << "N_pix               " << N_pix << std::endl ;
  cfile << "# logarithmic or linear binning [string: lin, log]" << std::endl ;
  cfile << "bin_type            " << bin_type << std::endl ;
  cfile << "# units of xpos/ypos in the input catalog [string: arcsec, arcmin, deg, rad, Mpc, kpc, ?]" << std::endl ;
  cfile << "units_input         " << units_input << std::endl ;
  cfile << "# units for bins of the output [string: arcsec, arcmin, deg, rad, Mpc, kpc, ?]" << std::endl ;
  cfile << "units_output        " << units_output << std::endl ;

  if (units_output == "Mpc" || units_output == "kpc")
    if (units_input == "arcsec" || units_input == "arcmin" || units_input == "deg" || units_input == "rad")
    {
      cfile << "\n### IFs ###" << std::endl ;
      cfile << "# conversion factor from angular to comoving coordinates, [double e.g., 0.03116] in " << units_input << "/" << units_output << std::endl;
      cfile << "conv_R2theta        " << conv_R2theta << std::endl;
    }

  //motivate user
  std::cout << "\nGreat, all set and done. You can also edit the config file directly or delete it and repeat the process. :)\n" << std::endl;

  //close file, happy ending
  cfile.close();
}



//***// handles the terminal dialogue to create a config file or not
bool yes_or_no(bool conclusion, int& sassy_remark, std::string& newconfig, std::string& config)
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

    getline(std::cin, newconfig) ;
    sassy_remark ++ ;

    if (sassy_remark == 11)
    {
      std::cout << "\nwelcome to the endless loop... muhahaha\n" << std::endl ;
      sassy_remark = 0 ;
    }
  }

  return conclusion ;
}