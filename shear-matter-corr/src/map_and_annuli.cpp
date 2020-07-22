//
//  map_and_annuli.cpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#include "map_and_annuli.hpp"

//***// calculates minimum number of pixel needed for the zero-padding method
int zero_padding_size(const double theta_out, const double field_size, double& field_size_fft, const int N_pix)
{
  field_size_fft = field_size + 2.*ceil( theta_out *N_pix/field_size/2. ) *field_size/N_pix ;

  // function changes field_size_fft on the fly
  return ceil( theta_out *N_pix/field_size/2. ) ;
}



//***// calculate different annuli with linearly or logarithmically spaced bins in degree
std::vector<double> setup_annuli(std::vector<double>& annuli_radius, const double tin, const double tout, const int N_annuli, const std::string& bin_type)
{
  // logarithmic bins
  if (bin_type == "log" || bin_type == "Log" || bin_type == "LOG")
  {
    double space = log10(tout/tin)/ (1.*N_annuli) ;
    if (isinf(space))
    {
      if (!C::shut_up)
        std::cerr << "\nERROR: annuli cannot be calculated, the quantity 'log10(theta_out/theta_in)/ N_annuli' is inf\n function >setup_annuli< in >intarea.cpp<" << std::endl ;
      exit(1) ;
    }
    
    // corresponding radius for annuli in degree
    for (int val = 0; val < N_annuli+1; ++val)
      annuli_radius[val] = pow(10., log10(tin)+ space*val) ;
  }
  // linear bins
  else if (bin_type == "lin" || bin_type == "Lin" || bin_type == "LIN")
  {
    double space = (tout-tin)/ (1.*N_annuli) ;
    if (isinf(space))
    {
      if (!C::shut_up)
        std::cerr << "\nERROR: annuli cannot be calculated, the quantity '(theta_out-theta_in)/ N_annuli' is inf\n function >setup_annuli< in >map_and_annuli.cpp<" << std::endl ;
      exit(1) ;
    }
    
    // corresponding radius for annuli in degree
    for (int val = 0; val < N_annuli+1; ++val)
      annuli_radius[val] = tin + space*val ;
  }
  // error handling
  else
  {
    if (!C::shut_up)
      std::cerr << "\nERROR: the 'bin_type' " << bin_type << " is neither 'lin' nor 'log'\n function >setup_annuli< in >map_and_annuli.cpp<" << std::endl ;
    exit(1) ;
  }
  
  // happy end
  return annuli_radius ;
}



//***// calculate the middle of an annulus [(inner+outer)/2] + unit conversion
std::vector<tmean_type> calc_thetamean(std::vector<tmean_type>& thetamean, const std::vector<double>& annuli_radius, const int N_annuli)
{
  // get linear and logarithmic mean
  for (int val = 0; val < N_annuli; ++val)
  {
    thetamean[val].mean[0] = .5*(annuli_radius[val] + annuli_radius[val+1]) ;
    thetamean[val].mean[1] = pow(10., .5*log10(annuli_radius[val]*annuli_radius[val+1])) ;
  }

  // happy end
  return thetamean ;
}



//***// convert units_input to units_output
double unit_conv(double toconv, const std::string& units_input, const std::string& units_output, const bool bin_in_R, const double conv_R2theta)
{
  // nothing to do here
  if (units_output == units_input)
    return toconv ;

  // angular unit conversions
  if (units_output == "deg")
  {
    if (units_input == "arcmin")
      return toconv/60. ;
    if (units_input == "arcsec")
      return toconv/60./60. ;
    if (units_input == "rad")
      return toconv/M_PI*180. ;
  }
  if (units_output == "arcmin")
  {
    if (units_input == "deg")
      return toconv*60. ;
    if (units_input == "arcsec")
      return toconv/60. ;
    if (units_input == "rad")
      return toconv/M_PI*180.*60. ;
  }
  if (units_output == "arcsec")
  {
    if (units_input == "deg")
      return toconv*60.*60. ;
    if (units_input == "arcmin")
      return toconv*60. ;
    if (units_input == "rad")
      return toconv/M_PI*180.*60.*60. ;
  }
  if (units_output == "rad")
  {
    if (units_input == "deg")
      return toconv/180.*M_PI ;
    if (units_input == "arcmin")
      return toconv/60./180.*M_PI ;
    if (units_input == "arsec")
      return toconv/60./60./180.*M_PI ;
  }

  // comoving unit conversions
  if (units_output == "Mpc" && units_input == "kpc")
    return toconv/1000. ;
  if (units_output == "kpc" && units_input == "Mpc")
    return toconv*1000. ;

  // from angular to comoving unit conversions
  if (bin_in_R == true)
    return toconv/conv_R2theta ;

  if (!C::shut_up)
  {
    std::cerr << "\nERROR: unit conversion failed, please check whether units_input [" << units_input << "] and units_output [" << units_output << "] are in the list [arcsec, arcmin, deg, rad, Mpc, kpc, ?]" << std::endl ;
    exit(1) ;
  }

  return 0 ;
}



//***// calculate the true mean distance per annulus
// std::vector<tmean_type> calc_thetatruemean(std::vector<tmean_type>& thetamean, const std::vector<double>& lens_density, const std::vector<double>& source_density, const double tin, const double tout, const int pix_zp, const int N_annuli, const int N_pix)
// {
//   std::vector<double> count_theta(N_annuli);
//   double space = log10(tout/tin)/ (1.*N_annuli) ;

//   for(int x_lens = 0 ; x_lens < N_pix ; x_lens++)
//   for(int y_lens = 0 ; y_lens < N_pix ; y_lens++)
//   {
//     if (!lens_density[x_lens*N_pix+y_lens])
//       continue ;

//     for(int x = x_lens-pix_zp ; x < x_lens+pix_zp ; x++)
//     for(int y = y_lens-pix_zp ; y < y_lens+pix_zp ; y++)
//     {
//       if (x<0 || y<0 || x>=N_pix || y>=N_pix)
//         continue ;

//       if (!source_density[x*N_pix+y])
//         continue ;

//       double deltatheta = sqrt((x-x_lens)*(x-x_lens) + (y-y_lens)*(y-y_lens)) ;
//       deltatheta *= 4.*60./(1.*N_pix) ;

//       if(deltatheta-tin<0)
//         continue ;
//       if(deltatheta>=tout)
//         continue ;

//       int bin = floor(log10(deltatheta/tin)/space) ;

//       thetamean[bin].mean[3] += deltatheta ;
//       count_theta[bin] += 1. ;
//     }
//   }

//   for(int bin = 0 ; bin < N_annuli ; bin++)
//     thetamean[bin].mean[3] /= count_theta[bin] ;


//   return thetamean ;
// }