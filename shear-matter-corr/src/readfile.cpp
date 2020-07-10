
//  readfile.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "readfile.hpp"

//***// read ascii file of lenses
void read_lens_file(double *&xpos, double *&ypos, double *&weight, int& number_of_galaxies, const std::string& input_file)
{

  // open file where lens galaxy data are stored
  std::ifstream file(input_file) ;

  // check if file exists
  if(!file)
  {
    if(!C::shut_up)
      std::cerr << "\nERROR: cannot open the file : " << input_file << "\nfunction >read_lens_file< in >readfile.cpp<" << std::endl ;
    exit(1);
  }

  // helpy variables to read the list
  std::string str ;
  std::string x, y, w ;
  std::string skip ;

  // get number of galaxies from file
  number_of_galaxies = 0 ;
  while (std::getline(file, str))
  {
    std::istringstream iss(str);
    y="empty";
    iss >> x >> y ;

    // skip all comment lines
    if (x.at(0) == '#' || x.at(0) == '%' || x.at(0) == '/' || x.at(0) == '!')
      continue ;
    
    // easy way: first line contains only the number of galaxies
    if (y=="empty")
    {
      number_of_galaxies = std::stoi(x) ;
      break ;
    }
    // hard way: loop through file to get number of galaxies
    else
    {
      while (std::getline(file, str))
        number_of_galaxies++ ;
      if (number_of_galaxies > 0)
        number_of_galaxies++ ;

      // jump back to line one
      file.clear() ;
      file.seekg(0, std::ios::beg) ;
      break ;
    }
  }

  // allocate memory for all galaxies
  xpos   = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  ypos   = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  weight = (double *)calloc(number_of_galaxies, sizeof(double)) ;

  // loop through file and assign values
  int idx = 0 ;
  while (std::getline(file, str))
  {
    std::istringstream iss(str);
    iss >> x >> y >> skip >> skip >> w ;

    // skip all comment lines
    if (x.at(0) == '#' || x.at(0) == '%' || x.at(0) == '/' || x.at(0) == '!')
      continue ;

    xpos[idx]   = std::stod(x) ;
    ypos[idx]   = std::stod(y) ;
    weight[idx] = std::stod(w) ;

    idx++ ;
  }

  // give warning if the indicated number of lenses is not the same as objects in file
  if (number_of_galaxies > idx+1 && !C::shut_up)
    std::cerr << "\nWARNING: apparantly the number of objects (lines) " << idx+1 << " in the code is inconsistent with the given number of galaxies " << number_of_galaxies << " in the file" << input_file << ", this should not cause a problem, but I won't guarantee ;) \nfunction >read_lens_file< in >readfile.cpp<" << std::endl ;

  // happy end
  file.close() ;

  // function changes xpos_lens, ypos_lens, weight, number_of_lensobjects on the fly
}


//***// read ascii file of sources
void read_source_file(double *&xpos, double *&ypos, double *&shear1, double *&shear2, double *&weight, int& number_of_galaxies, const std::string& input_file)
{

  // open file where source galaxy data are stored
  std::ifstream file(input_file) ;

  // check if file exists
  if(!file)
  {
    if(!C::shut_up)
      std::cerr << "\nERROR: cannot open the file : " << input_file << "\nfunction >read_source_file< in >readfile.cpp<" << std::endl ;
    exit(1);
  }

  // helpy variables to read the list
  std::string str    ;
  std::string x,  y  ;
  std::string s1, s2 ;
  std::string w      ;

  // get number of galaxies from file
  number_of_galaxies = 0 ;
  while (std::getline(file, str))
  {
    std::istringstream iss(str);
    y="empty";
    iss >> x >> y ;

    // skip all comment lines
    if (x.at(0) == '#' || x.at(0) == '%' || x.at(0) == '/' || x.at(0) == '!')
      continue ;
    
    // easy way: first line contains only the number of galaxies
    if (y=="empty")
    {
      number_of_galaxies = std::stoi(x) ;
      break ;
    }
    // hard way: loop through file to get number of galaxies
    else
    {
      while (std::getline(file, str))
        number_of_galaxies++ ;
      if (number_of_galaxies > 0)
        number_of_galaxies++ ;

      // jump back to line one
      file.clear() ;
      file.seekg(0, std::ios::beg) ;
      break ;
    }
  }

  // allocate memory for all galaxies
  xpos   = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  ypos   = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  shear1 = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  shear2 = (double *)calloc(number_of_galaxies, sizeof(double)) ;
  weight = (double *)calloc(number_of_galaxies, sizeof(double)) ;

  // loop through file and assign values
  int idx = 0 ;
  while (std::getline(file, str))
  {
    std::istringstream iss(str);
    iss >> x >> y >> s1 >> s2 >> w ;

    // skip all comment lines
    if (x.at(0) == '#' || x.at(0) == '%' || x.at(0) == '/' || x.at(0) == '!')
      continue ;

    xpos[idx]   = std::stod(x)  ;
    ypos[idx]   = std::stod(y)  ;
    shear1[idx] = std::stod(s1) ;
    shear2[idx] = std::stod(s2) ;
    weight[idx] = std::stod(w)  ;

    idx++ ;
  }

  // give warning if the indicated number of sources is not the same as objects in file
  if (number_of_galaxies > idx+1 && !C::shut_up)
    std::cerr << "\nWARNING: apparantly the number of objects (lines) " << idx+1 << " in the code is inconsistent with the given number of galaxies " << number_of_galaxies << " in the file" << input_file << ", this should not cause a problem, but I can't guarantee ;) \nfunction >read_source_file< in >readfile.cpp<" << std::endl ;

  // happy end
  file.close() ;

  // function changes xpos_source, ypos_source, shear1, shear2, weight, number_of_sourceobjects on the fly
}