//
//  main.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//
//  g++ -std=c++11 *.cpp -o banana -O3 -march=native -fprefetch-loop-arrays -funit-at-a-time  -finline-limit=6000 -fgcse-sm -fgcse-las -funsafe-loop-optimizations -Wall -Wextra -Wunused -Wshadow -Wno-non-virtual-dtor  -Winit-self -Wswitch-default -Wpointer-arith -Wmissing-noreturn -Wdisabled-optimization -lm -lfftw3


// standard libraries (all listed that are needed for all subroutines)
#include <getopt.h>
// #include <iomanip>
// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <vector>
// #include <array>
// #include <complex>
// #include <random>
// #include <fftw3.h>


// include some structs available for all routines
#include "structdefs.hpp"
namespace C {
  // option to toggle off the output file and print result in terminal instead
  bool output2file = true ;
  // option to mute the programm (incl error messages)
  bool shut_up = false ;

  // calculate convergence-matter correlation function instead of shear-matter
  // needs the convergence as an inout catalogue
  bool convergence = false ;
}

namespace RG {
  std::mt19937 random_generator;
}

// include subroutines
#include "config.hpp"
#include "map_and_annuli.hpp"
#include "galassign.hpp"
#include "calc_tshear.hpp"
#include "readfile.hpp"
#include "writefile.hpp"
#include "random_number.hpp"


int main(int argc, char * const argv[])
{
  // *** //
  // definitions and initializations
  // *** //

  // changeable stuff by config
  std::string input_lens_file, input_source_file, output_data_dir, bin_type ;
  std::string units_input, units_output ;
  bool bin_in_R = false ;
  double theta_in = -1, theta_out = -1 ;
  double conv_R2theta = -1 ;
  int N_pix = -1, N_annuli = -1 ;
  std::string config =  "config" ;

  // initialize the random generator
  seed(1);


  // optional different name for config file
  // toggle 'output2file' and 'shut_up' when running code
  // enable as './programm -q -o'
  int arg ;
  static struct option longopts[] = 
  {
    {"config", required_argument, 0, 'c'} ,
    {"mute_mode", no_argument, 0, 'q'} ,    //q like quiet
    {"no_output", no_argument, 0, 'o'} ,    //o like output
    {"convergence", no_argument, 0, 'k'} ,  //k like kappa
    {0, 0, 0, 0}
  };

  while ((arg = getopt_long(argc, argv, "c:qok", longopts, NULL)) != -1)
  {
    switch (arg)
    {
      case 'c': /* --config */
        config = optarg ;
        break ;
         
      case 'q': /* --mute_mode */
        C::shut_up = true ;
        break ;

      case 'o': /* --no_output */
        C::output2file = false ;
        break ;

      case 'k': /* --convergence */
        C::convergence = true ;
        break ;
                
      case '?':
      default:    /* invalid option */
        std::cout << "./programm -q -o -c config -k\n q: program is mute (incl error messages) \n o: toggle outputfile off\n c: name of your config file\n k: input catalogues contain convergence instead of shear\n treat yourself to a cookie" <<  std::endl ;
        return(1);
    }
  }

  if (!C::shut_up)
    sayhello() ;

  read_config_file(config, input_lens_file, input_source_file, theta_in, theta_out, N_annuli, output_data_dir, N_pix, bin_type, units_input, units_output, bin_in_R, conv_R2theta) ;

  if (!C::shut_up)
      std::cout << "finished configuring\nsetting-up annuli\r" << std::flush ;


  // calculate binning for tangential shear measurements in catalog units
  std::vector<double> annuli_radius(N_annuli+1) ;
  annuli_radius = setup_annuli(annuli_radius, theta_in, theta_out, N_annuli, bin_type) ;


  // calculate the mean theta in an angular bin
  // MAKE 'calc_thetatruemean'
  std::vector<tmean_type> thetamean(N_annuli) ;
  thetamean = calc_thetamean(thetamean, annuli_radius, N_annuli) ;
  

  if (!C::shut_up)
    std::cout << "finished setting-up annuli\nreading in galaxy catalogs\r" << std::flush;

  // *** //
  // read input catalogues
  // *** //

  // calculate the distribution of lens galaxies and random positions
  // calculate the shear and distribution of source galaxies
  double number_of_lenses  =  0. ;
  double number_of_sources =  0. ;
  double field_size        = -1. ;

  std::vector<double> lens_density  (N_pix*N_pix) ;
  std::vector<double> source_density(N_pix*N_pix) ;
  std::vector<std::complex<double>> gamma12(N_pix*N_pix) ;


  // change lens_density, number_of_lenses, source_density, gamma12, number_of_sources, field_size
  gals2pos(lens_density, source_density, gamma12, number_of_lenses, number_of_sources, field_size, N_pix, input_lens_file, input_source_file, thetamean, N_annuli, units_output, output_data_dir) ;

  std::vector<double> lens_density_forLSS(N_pix*N_pix) ;
  lens_density_forLSS = lens2pos_forLSS(lens_density_forLSS, N_pix) ;


  if (!C::shut_up)
  {
    std::cout.precision(2) ;
    std::cout << "finished reading in galaxy catalogs" << std::endl ;
    std::cout << "\nreporting some values here:\nfield size: " << field_size << "[" << units_input << "]\nresolution: " << std::scientific << field_size/N_pix << "[" << units_input << "/pix]" << std::endl ;
    std::cout << "\ncalculating the 2D-average shear field\r" << std::flush ;
  }


  // FFT assumes periodic boundary conditions, so we zero-pad with the minimum number of pixel
  field_size = unit_conv(field_size, units_input, units_output, bin_in_R, conv_R2theta) ;
  double    field_size_fft ;
  const int pix_zero_padding = zero_padding_size(theta_out, field_size, field_size_fft, N_pix) ;
  const int N_fft = N_pix + 2*pix_zero_padding ;



  // *** //
  // calculating tangential shear estimate
  // *** //

  // calculate the 2D-averaged (Cartesian) shear field around lenses and random positions
  std::vector<std::complex<double>> gamma_g2l(N_fft*N_fft) ;
  gamma_g2l = do_FFT(gamma_g2l, gamma12, lens_density, source_density, N_pix, N_fft) ;

  std::vector<std::complex<double>> gamma_lss(N_fft*N_fft) ;
  gamma_lss = do_FFT(gamma_lss, gamma12, lens_density_forLSS, source_density, N_pix, N_fft) ;

  if (!C::shut_up)
    std::cout << "calculated the 2D-average shear field\ncalculating the azimuthally-averaged tang. shear\r" << std::flush;


  // calculate the azimuthally averaged, tangential/cross shear profile
  std::vector<std::complex<double>> gamma_R(N_annuli) ;
  gamma_R = calc_tshear(gamma_R, gamma_g2l, pix_zero_padding, field_size_fft, annuli_radius, N_annuli, bin_type, N_fft) ;

  std::vector<std::complex<double>> gamma_Rlss(N_annuli) ;
  gamma_Rlss = calc_tshear(gamma_Rlss, gamma_lss, pix_zero_padding, field_size_fft, annuli_radius, N_annuli, bin_type, N_fft) ;

  if (!C::shut_up)
    std::cout << "calculated the azimuthally-averaged tangential shear" << std::endl ;


  // write output file with result
  if (!C::shut_up)
    std::cout << "write output\r" << std::flush;
  write_output_file(thetamean, gamma_R, gamma_Rlss, number_of_lenses, number_of_sources, N_annuli, units_output, output_data_dir) ;
  if (!C::shut_up)
    std::cout << "wrote output\n" << std::endl ;

  if (!C::shut_up)
    std::cout << "\nAll done! :)\n" << std::endl ;

  // happy end
  return 0;
}