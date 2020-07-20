//
//  galassign.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "galassign.hpp"

//***// assigns positions from lens galaxies and shear from source galaxies to the pixel grid
void gals2pos(std::vector<double>& lens_density, std::vector<double>& source_density, std::vector<std::complex<double>>& gamma12, double& number_of_lenses, double& number_of_sources, double& field_size, const int N_pix, const std::string& input_lens_file, const std::string& input_source_file, const std::vector<tmean_type> theta, const int N_annuli, const std::string output_data_dir)
{
  for (int pix = 0 ; pix < N_pix*N_pix ; ++pix)
  {
    lens_density[pix] = 0. ;
    source_density[pix] = 0. ;
    gamma12[pix] = {0. , 0.} ;
  }

  // *** //
  // extract necessary information about lenses
  // *** //


  // initialize the wanted quantities from the ascii catalog
  double *xpos_lens, *ypos_lens  ;
  double *weight_lens            ; 
  int number_of_lensobjects = -1 ;


  // read the information from the .fits file
  read_lens_file(xpos_lens, ypos_lens, weight_lens, number_of_lensobjects, input_lens_file) ;


  // if no galaxies are in the file, stop routine but write an output file filled with zeros
  if (number_of_lensobjects < 1)
  {
    if(!C::shut_up)
      std::cerr << "\nWARNING: no lenses have been found in the file : " << input_lens_file << "\nfunction >gals2pos< in >galassign.cpp<" << std::endl;
    if (C::output2file)
      write_empty_output_file(theta, N_annuli, output_data_dir) ;
    else
      print_empty_result(theta, N_annuli) ;
    exit(1) ;
  }


  // *** //
  // extract necessary information about sources
  // *** //


  // initialize the wanted quantities from the ascii catalog
  double *xpos_source, *ypos_source    ;
  double *shear1,      *shear2         ; 
  double *weight_source                ; 
  int     number_of_sourceobjects = -1 ;


  // read the information from the .fits file
  read_source_file(xpos_source, ypos_source, shear1, shear2, weight_source, number_of_sourceobjects, input_source_file) ;


  // if no galaxies are in the file, stop routine but write an output file filled with zeros
  if (number_of_sourceobjects < 1)
  {
    if(!C::shut_up)
      std::cerr << "\nWARNING: no sources have been found in the file : " << input_source_file << "\nfunction >gals2pos< in >galassign.cpp<" << std::endl;
    if (C::output2file)
      write_empty_output_file(theta, N_annuli, output_data_dir) ;
    else
      print_empty_result(theta, N_annuli) ;
    exit(1) ;
  }


std::cout << "\n" << number_of_sourceobjects << std::endl;
  for (int i = 0; i < number_of_sourceobjects; ++i)
    std::cout << xpos_source[i] << "   " << ypos_source[i] << "   " << weight_source[i] << std::endl ;
exit(1);


  // *** //
  // calculate size of the field in 'input_coord_units'
  // *** // 


  // get max and min from x and y coordinate for lens field
  double max_x_lens = xpos_lens[0], max_y_lens = ypos_lens[0] ;
  double min_x_lens = xpos_lens[0], min_y_lens = ypos_lens[0] ;
  for (int lens = 0 ; lens < number_of_lensobjects ; ++lens)
  {
      max_x_lens = std::max(max_x_lens,xpos_lens[lens]) ;
      max_y_lens = std::max(max_y_lens,ypos_lens[lens]) ;
      min_x_lens = std::min(min_x_lens,xpos_lens[lens]) ; 
      min_y_lens = std::min(min_y_lens,ypos_lens[lens]) ;
  }


  // get max and min from x and y coordinate for source field
  double max_x_source = xpos_source[0], max_y_source = ypos_source[0] ;
  double min_x_source = xpos_source[0], min_y_source = ypos_source[0] ;
  for (int source = 0 ; source < number_of_sourceobjects ; ++source)
  {
      max_x_source = std::max(max_x_source,xpos_source[source]) ;
      max_y_source = std::max(max_y_source,ypos_source[source]) ;
      min_x_source = std::min(min_x_source,xpos_source[source]) ; 
      min_y_source = std::min(min_y_source,ypos_source[source]) ;
  }


  // get max and min from both fields
  double max_x = std::max(max_x_lens, max_x_source) ;
  double max_y = std::max(max_y_lens, max_y_source) ;
  double min_x = std::min(min_x_lens, min_x_source) ;
  double min_y = std::min(min_y_lens, min_y_source) ;


  // calculate difference in x and y direction, take maximum
  field_size  = std::max(max_x-min_x,max_y-min_y) ;
  field_size *= 1. + 1./(1.*N_pix) ;
  
  //size of a pixel in 'input_coord_units'
  const double pix_size = field_size / (1.*N_pix) ;


  // error management
  if (field_size < 0. )
  {
    if(!C::shut_up)
      std::cerr << "\nERROR: cannot determine the size of the input field, something is fishy with the x- and y-coordinates \nfunction >gals2pos< in >galassign.cpp<" << std::endl;
    exit(1) ;
  }



  // *** //
  // calculate lens_density
  // *** //


  // iterate through all lenses
  // calculate the position in pixel units, consider weights
  for(int lens = 0 ; lens < number_of_lensobjects ; ++lens)
  {
    int x_lens = static_cast<int>( round( (xpos_lens[lens]-min_x) /  pix_size ) ) ;
    int y_lens = static_cast<int>( round( (ypos_lens[lens]-min_y) /  pix_size ) ) ;

    // crudely manage lenses too close to the edge
    // (can only appear due to precision issues)
    if(x_lens == N_pix) x_lens = N_pix-1 ;
    if(y_lens == N_pix) y_lens = N_pix-1 ;

    lens_density[ x_lens * N_pix + y_lens ] += 1.*weight_lens[lens] ;
    number_of_lenses += 1.*weight_lens[lens] ;
  }


  // normalize by number_of_lenses
  for (int pix = 0 ; pix < N_pix*N_pix ; ++pix)
    lens_density[pix] /= 1.*number_of_lenses ;


  // free allocated memory
  free(xpos_lens)   ;
  free(ypos_lens)   ;
  free(weight_lens) ;



  // *** // calculate source_density


  // iterate through all sources
  // calculate the position in pixel units, consider weights
  for(int source = 0 ; source < number_of_sourceobjects ; ++source)
  {
    int x_source = static_cast<int>( round( (xpos_source[source]-min_x) /  pix_size ) ) ;
    int y_source = static_cast<int>( round( (ypos_source[source]-min_y) /  pix_size ) ) ;

    // crudely manage sources too close to the edge
    if(x_source == N_pix) x_source = N_pix-1 ;
    if(y_source == N_pix) y_source = N_pix-1 ;

    source_density[ x_source * N_pix + y_source ] += 1.*weight_source[source] ;
    number_of_sources += 1.*weight_source[source] ;

    gamma12[ x_source * N_pix + y_source ] = { shear1[source]*weight_source[source], shear2[source]*weight_source[source] } ;
  }


  // normalize by number_of_sources
  for (int pix = 0 ; pix < N_pix*N_pix ; ++pix)
    if (source_density[pix] > 0.)
      gamma12[pix] =  { std::real(gamma12[pix]) / source_density[pix] , std::imag(gamma12[pix]) / source_density[pix] } ;


  // free allocated memory
  free(xpos_source)   ;
  free(ypos_source)   ;
  free(shear1)        ;
  free(shear2)        ;
  free(weight_source) ;


  // if weighted number of lenses is zero, stop routine but write an output file filled with zeros
  if (number_of_lenses < 1e-3 )
  {
    if(!C::shut_up)
      std::cerr << "\nWARNING: the weighted number of lenses found in file : " << input_lens_file << " is zero \nfunction >gals2pos< in >galassign.cpp<" << std::endl;
    if (C::output2file)
      write_empty_output_file(theta, N_annuli, output_data_dir) ;
    else
      print_empty_result(theta, N_annuli) ;
    exit(1) ;
  }

  // if weighted number of sources is zero, stop routine but write an output file filled with zeros
  if (number_of_sources < 1e-3 )
  {
    if(!C::shut_up)
      std::cerr << "\nWARNING: the weighted number of sources found in file : " << input_source_file << " is zero \nfunction >gals2pos< in >galassign.cpp<" << std::endl;
    if (C::output2file)
      write_empty_output_file(theta, N_annuli, output_data_dir) ;
    else
      print_empty_result(theta, N_annuli) ;
    exit(1) ;
  }

  // function changes lens_density, number_of_lenses, source_density, gamma12, number_of_sources, field_size
}



//***// selects random lenses (~70% of all pixels)
std::vector<double> lens2pos_forLSS(std::vector<double>& lens_density, const int N_pix)
{
  // put .7*N_pix*N_pix lenses randomly on pixel, this will actually fill ~50% of all pixels 
  double N_rand = N_pix*N_pix/sqrt(2) ;
  for (int rand = 0 ; rand < N_rand ; ++rand)
    lens_density[GetRand(N_pix*N_pix)] += 1./N_rand ;

  return lens_density ;
}