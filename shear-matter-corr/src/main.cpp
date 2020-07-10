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
}

// include subroutines
#include "config.hpp"
#include "map_and_annuli.hpp"
#include "galassign.hpp"
#include "calc_tshear.hpp"
#include "readfile.hpp"
#include "writefile.hpp"


int main(int argc, char * const argv[])
{
  // *** //
  // definitions and initializations
  // *** //

  // changeable stuff by config
  std::string input_lens_file, input_source_file, output_data_dir, bin_type ;
  double theta_in = -1, theta_out = -1 ;
  double conv_R2theta = -1 ;
  int N_pix = -1, N_annuli = -1 ;
  bool bin_in_R = false ;


  // toggle 'output2file' and 'shut_up' when running code
  // enable as './programm -q -o'
  int arg ;
  static struct option longopts[] = 
  {
    {"mute_mode", no_argument, 0, 'q'} , //q like quiet
    {"no_output", no_argument, 0, 'o'} , //o like output
    {0, 0, 0, 0}
  };

  while ((arg = getopt_long(argc, argv, "qo", longopts, NULL)) != -1)
  {
    switch (arg)
    {
      case 'q': /* --mute_mode */
        C::shut_up = true ;
        break ;

      case 'o': /* --no_output */
        C::output2file = false ;
        break ;
                
      case '?':
      default:    /* invalid option */
        std::cout << "./programm -q -o \n q: program is mute (incl error messages) \n o: toggle outputfile off \n treat yourself to a cookie" <<  std::endl ;
        return(1);
    }
  }

  if (!C::shut_up)
    sayhello() ;

  read_config_file(input_lens_file, input_source_file, output_data_dir, N_pix, theta_in, theta_out, N_annuli, bin_type, bin_in_R, conv_R2theta) ;

  if (!C::shut_up)
      std::cout << "finished configuring\nsetting-up annuli\r" << std::flush ;


  // change from angular to comoving coordinates
  if (bin_in_R == true)
  {
    theta_in  *= conv_R2theta ; //deg
    theta_out *= conv_R2theta ; //deg
  }


  // calculate binning for tangential shear measurements in catalog units
  std::vector<double> annuli_radius(N_annuli+1) ;
  annuli_radius = setup_annuli(annuli_radius, theta_in, theta_out, N_annuli, bin_type) ;


  // calculate the mean theta in an angular bin
  // MAKE 'calc_thetatruemean'
  std::vector<tmean_type> thetamean(N_annuli) ;
  thetamean = calc_thetamean(thetamean, annuli_radius, N_annuli, bin_in_R, conv_R2theta) ;
  

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
  gals2pos(lens_density, source_density, gamma12, number_of_lenses, number_of_sources, field_size, N_pix, input_lens_file, input_source_file, thetamean, N_annuli, output_data_dir) ;

  std::vector<double> lens_density_forLSS(N_pix*N_pix) ;
  lens_density_forLSS = lens2pos_forLSS(lens_density_forLSS, N_pix) ;


  // FFT assumes periodic boundary conditions, so we zero-pad with the minimum number of pixel
  double    field_size_fft ;
  const int pix_zero_padding = zero_padding_size(theta_out, field_size, field_size_fft, N_pix) ;
  const int N_fft = N_pix + 2*pix_zero_padding ;

  if (!C::shut_up)
  {
    std::cout << "finished reading in galaxy catalogs" << std::endl ;
    std::cout << "\nreporting some values here:\nfield size: " << field_size << "[cat units]\nresolution: " << std::scientific << field_size/N_pix << "[cat units/pix]" << std::endl ;
    std::cout << "\ncalculating the 2D-average shear field\r" << std::flush ;
  }



  // *** //
  // calculating tangential shear estimate
  // *** //

  // calculate the 2D-averaged (Cartesian) shear field around lenses and random positions
  std::vector<std::complex<double>> gamma_g2l(N_fft*N_fft) ;
  gamma_g2l = do_FFT_lenspos(gamma_g2l, gamma12, lens_density, source_density, N_pix, N_fft) ;

  std::vector<std::complex<double>> gamma_lss(N_fft*N_fft) ;
  gamma_lss = do_FFT_lenspos(gamma_lss, gamma12, lens_density_forLSS, source_density, N_pix, N_fft) ;

  if (!C::shut_up)
    std::cout << "calculated the 2D-average shear field\ncalculating the azimuthally-averaged tang. shear\r" << std::flush;


  // calculate the azimuthally averaged, tangential/cross shear profile
  std::vector<std::complex<double>> gamma_R(N_annuli) ;
  gamma_R = calc_tshear_annulus(gamma_R, gamma_g2l, pix_zero_padding, field_size_fft, annuli_radius, N_annuli, bin_type, N_fft) ;

  std::vector<std::complex<double>> gamma_Rlss(N_annuli) ;
  gamma_Rlss = calc_tshear_annulus(gamma_Rlss, gamma_lss, pix_zero_padding, field_size_fft, annuli_radius, N_annuli, bin_type, N_fft) ;

  if (!C::shut_up)
    std::cout << "calculated the azimuthally-averaged tangential shear" << std::endl ;


  // write output file with result
  if(C::output2file)
  {
    if (!C::shut_up)
    std::cout << "write output\r" << std::flush;
    write_file_for_GammaR(thetamean, gamma_R, gamma_Rlss, number_of_lenses, number_of_sources, N_annuli, output_data_dir) ;
    if (!C::shut_up)
    std::cout << "wrote output\n" << std::endl ;
  }
  // or print result in terminal
  else
    print_result(thetamean, gamma_R, gamma_Rlss, number_of_lenses, number_of_sources, N_annuli) ;

  if (!C::shut_up)
    std::cout << "\nAll done! :)\n" << std::endl ;

  // happy end
  return 0;
}