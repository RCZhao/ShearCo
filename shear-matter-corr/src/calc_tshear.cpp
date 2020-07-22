//
//  calc_tshear.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "calc_tshear.hpp"


//***// stacks (and averages) the shear fields behind all lens 
std::vector<std::complex<double>> do_FFT(std::vector<std::complex<double>>& gamma_g2l, const std::vector<std::complex<double>>& gamma12, const std::vector<double>& lens_density, const std::vector<double>& source_density, const int N_pix, const int N_fft)
{
  //***// preparation for the FFT according to FFTW3 syntax

  // initialize the plans (first!) for the FFT that need to be done
  fftw_plan plan_lens_density, plan_gamma, plan_convolution1, plan_convolution2, plan_sourcepos ;
  // initialize an FFTW compatible complex
  fftw_complex *lens_density_FFTW, *gamma12_FFTW, *convolution1, *convolution2, *sourcepos_FFTW ;


  // allocate memory for initialized complex variables (1dim of size number of pixels)
  lens_density_FFTW = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_fft*N_fft) ;
  sourcepos_FFTW    = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_fft*N_fft) ;
  gamma12_FFTW      = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_fft*N_fft) ;
  convolution1      = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_fft*N_fft) ;
  convolution2      = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N_fft*N_fft) ;


  // let FTTW create plans to carry out the FFT
  // telling it dimension, in- and output variable, as well as direction of FFT
  plan_lens_density = fftw_plan_dft_2d(N_fft, N_fft, lens_density_FFTW, lens_density_FFTW, FFTW_FORWARD, FFTW_ESTIMATE) ;
  plan_sourcepos    = fftw_plan_dft_2d(N_fft, N_fft, sourcepos_FFTW, sourcepos_FFTW, FFTW_FORWARD, FFTW_ESTIMATE) ;
  plan_gamma        = fftw_plan_dft_2d(N_fft, N_fft, gamma12_FFTW, gamma12_FFTW, FFTW_FORWARD, FFTW_ESTIMATE) ;
  plan_convolution1 = fftw_plan_dft_2d(N_fft, N_fft, convolution1, convolution1, FFTW_BACKWARD, FFTW_ESTIMATE) ;
  plan_convolution2 = fftw_plan_dft_2d(N_fft, N_fft, convolution2, convolution2, FFTW_BACKWARD, FFTW_ESTIMATE) ;



  //***// carry out the FFT

  // iterate over the whole grid and assign gamma12 to its correspondent FTTW compatible variable
  for (int pix_x = 0 ; pix_x < N_pix; ++pix_x)
  for (int pix_y = 0 ; pix_y < N_pix; ++pix_y)
  {
    int pix     = pix_x*N_pix + pix_y ;
    int pix_fft = pix_x*N_fft + pix_y ;

    gamma12_FFTW[pix_fft][0] = 0. ;
    gamma12_FFTW[pix_fft][1] = 0. ;

    if (source_density[pix]>0.)
    {
      gamma12_FFTW[pix_fft][0] = std::real( gamma12[pix] ) ;
      gamma12_FFTW[pix_fft][1] = std::imag( gamma12[pix] ) ;
    }

    lens_density_FFTW[pix_fft][0] = lens_density[pix] ;
    lens_density_FFTW[pix_fft][1] = 0. ;

    sourcepos_FFTW[pix_fft][0] = source_density[pix] ;
    sourcepos_FFTW[pix_fft][1] = 0. ;
  }

  //***// carry out the FFT

  // use forward FFT on the lens/source map and on the gamma12 values
  fftw_execute(plan_gamma) ;
  fftw_execute(plan_lens_density) ;
  fftw_execute(plan_sourcepos) ;


  // convolution theorem in action
  // iterate over the whole grid
  for(int pix = 0 ; pix < N_fft*N_fft ; ++pix)
  {
    convolution1[pix][0] = lens_density_FFTW[pix][0] * gamma12_FFTW[pix][0] + lens_density_FFTW[pix][1] * gamma12_FFTW[pix][1] ;
    convolution1[pix][1] = lens_density_FFTW[pix][0] * gamma12_FFTW[pix][1] - lens_density_FFTW[pix][1] * gamma12_FFTW[pix][0] ;

    convolution2[pix][0] = lens_density_FFTW[pix][0] * sourcepos_FFTW[pix][0] + lens_density_FFTW[pix][1] * sourcepos_FFTW[pix][1] ;
    convolution2[pix][1] = lens_density_FFTW[pix][0] * sourcepos_FFTW[pix][1] - lens_density_FFTW[pix][1] * sourcepos_FFTW[pix][0] ;
  }


  // use backward FFT on the convolved values to obtain stacked gamma_12 and stacked number of source-lens pairs per pixel
  fftw_execute(plan_convolution1) ; // stacked shear signals
  fftw_execute(plan_convolution2) ; // number of lens-source pairs per pixel


  // iterate over the whole grid and assign the FFT variable to the return variable
  for (int pix_x = 0 ; pix_x < N_fft; ++pix_x)
  for (int pix_y = 0 ; pix_y < N_fft; ++pix_y)
  {
    // FFT shifts the signal to the origin which I don't like
    // hence, it goes to the middle
    int pix_shiftx ;
    int pix_shifty ;

    if (pix_x < N_fft/2)
      pix_shiftx = pix_x+N_fft/2 ;
    else
      pix_shiftx = pix_x-N_fft/2 ;

    if (pix_y < N_fft/2)
      pix_shifty = pix_y+N_fft/2 ;
    else
      pix_shifty = pix_y-N_fft/2 ;

    int pix_shift = pix_shiftx*N_fft + pix_shifty ;
    int pix = pix_x*N_fft + pix_y ;

    // no lens-source pair could be found for this pixel
    if (convolution2[pix][0] == 0.)
      gamma_g2l[pix_shift]  = { 0., 0.} ;
    // caverage the stacked shear signal by number of lens-source pair
    else
      gamma_g2l[pix_shift]  = { convolution1[pix][0]/convolution2[pix][0], convolution1[pix][1]/convolution2[pix][0] } ; 
  }

  // free allocated memory of FFTW variables
  fftw_destroy_plan(plan_lens_density) ;
  fftw_destroy_plan(plan_sourcepos)    ;
  fftw_destroy_plan(plan_gamma)        ;
  fftw_destroy_plan(plan_convolution1) ;
  fftw_destroy_plan(plan_convolution2) ;
  fftw_free(lens_density_FFTW) ;
  fftw_free(sourcepos_FFTW)    ;
  fftw_free(gamma12_FFTW)      ;
  fftw_free(convolution1)       ;
  fftw_free(convolution2)       ;

  // happy ending for the computationally most expensive part
  return gamma_g2l ;
}

//***// calculates the tangential and cross shear as a function of distance from the center
std::vector<std::complex<double>> calc_tshear(std::vector<std::complex<double>>& gamma_R, const std::vector<std::complex<double>>& gamma_g2l, const int pix_zp, const double field_size_fft, std::vector<double> annuli_radius, const int N_annuli, const std::string& bin_type, const int N_fft)
{
  // convert pixel positions to catalog units
  std::vector<Galaxy_Position> PixelPos(N_fft*N_fft) ;
  for(int pix_x = 0 ; pix_x < N_fft ; ++pix_x)
  for(int pix_y = 0 ; pix_y < N_fft ; ++pix_y)
  {
    PixelPos[pix_x*N_fft +pix_y].pos[0] = 1.*pix_x *field_size_fft/(1.*N_fft) - field_size_fft/2. ;
    PixelPos[pix_x*N_fft +pix_y].pos[1] = 1.*pix_y *field_size_fft/(1.*N_fft) - field_size_fft/2. ;
  }

  std::cout << PixelPos[0].pos[0] << std::endl ;
  std::cout << PixelPos[0].pos[1] << std::endl ;

  // initialize container that collect all gamma_tx values in a given annuli and its number
  std::vector<std::complex<double>> gamma_sum(N_annuli) ;
  std::vector<int> bin_counts(N_annuli,  0.) ;


  //  to find correct bins for all gamma values, set bin width
  double space ;
  if (bin_type == "log" || bin_type == "Log" || bin_type == "LOG")
    space = log10(annuli_radius[1]/annuli_radius[0]) ;
  else
    space = annuli_radius[1] - annuli_radius[0] ;

  double sep ;
  std::complex<double> theta ;
  std::complex<double> tang_shear ;
  for(int pix_x = N_fft/2-2*pix_zp ; pix_x < N_fft/2+2*pix_zp ; ++pix_x)
  for(int pix_y = N_fft/2-2*pix_zp ; pix_y < N_fft/2+2*pix_zp ; ++pix_y)
  {
    // handle requested pixel outside the grid
    if(pix_x<0 || pix_x>=N_fft) continue ;
    if(pix_y<0 || pix_y>=N_fft) continue ;

    // handle a shear value of 0
    if (abs(std::abs(gamma_g2l[pix_x*N_fft+pix_y])) < 1.e-7) continue ;

    // get distance vector for specific pixel
    theta = { static_cast<double>(pix_x-N_fft/2) , static_cast<double>(pix_y-N_fft/2) } ;

    // don't consider the centre
    if (std::abs(theta) < 1.e-7) continue ;

    // calculate tangential (and cross) shear from gamma_12
    tang_shear = -gamma_g2l[pix_x*N_fft+pix_y]  *std::conj(theta)/theta ;

    // calculate the separation to center in catalog units
    sep = sqrt( pow(PixelPos[pix_x*N_fft+pix_y].pos[0], 2.) + pow(PixelPos[pix_x*N_fft + pix_y].pos[1], 2.) ) ;

    // only consider values theta_in < sep < theta_out
    if(sep < annuli_radius[0] || sep>=annuli_radius[N_annuli]) continue ;

    // find idx of annulus that matches the separation
    int annulus ;
    if (bin_type == "log" || bin_type == "Log" || bin_type == "LOG")
      annulus = floor(log10(sep/annuli_radius[0])/space) ;
    else
      annulus = floor((sep-annuli_radius[0])/space) ;

    // finally add the tangential (and cross) shear and increment the counter
    gamma_sum[annulus]  += tang_shear ;
    bin_counts[annulus] += 1 ;
  }


  // calculate the radial profile by dividing the stacked gamma_t/x values by its counts per annulus
  // do only if gamma_t/x values have been found in that annulus, else set gamma_R to zero
  for(int ann = 0 ; ann < N_annuli ; ++ann )
  {
    if (bin_counts[ann] != 0)
      gamma_R[ann] = {std::real(gamma_sum[ann]) / static_cast<double>(bin_counts[ann]), std::imag(gamma_sum[ann]) / static_cast<double>(bin_counts[ann])} ;
    else
      gamma_R[ann] = {0.,0.} ;
  }

  return gamma_R ;
}