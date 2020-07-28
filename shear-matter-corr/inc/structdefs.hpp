//
//  structdefs.hpp
//  ShearCo
//
//  Created by Sandra Unruh 2019
//  Copyright © 2019 ShearCo. All rights reserved.
//

#ifndef structdefs_hpp
#define structdefs_hpp

#include <array>
#include <complex>

#include <vector>



// a 2-dim, float vector to hold information on galaxy position
struct Galaxy_Position
{
    float pos[2];
} ;

// a 2-dim, double vector to hold information on lin, log and true mean
struct tmean_type
{
    double mean[3];
} ;


// cheating my way around the evil global variables
namespace C
{
  // option to toggle off the output file and print result in terminal instead
  extern bool output2file ;
  // option to mute the programm (incl error messages)
  extern bool shut_up ;

  // calculate convergence-matter correlation function instead of shear-matter
  // needs the convergence as an inout catalogue
  extern bool convergence ;
}


// convert numbers to strings
namespace SH
{
  template <typename Type_>
  inline std::string
  ToString(const Type_& x_)
  {
    std::ostringstream s_;
    s_ << x_;
    return s_.str();
  }
    
  template <typename Type_>
  inline std::string
  ToString(const Type_& x_, const unsigned width_, const char fillchar_)
  {
    std::ostringstream s_;
    s_.width(width_);
    s_.fill(fillchar_);
    s_ << x_;
    return s_.str();
  }
}

#endif /* structdefs_hpp */