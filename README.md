# ShearCo
Version 1.0 (07/2020) by Sandra Unruh

Second-order correlation functions with fast-Fourier transforms in C++

## Table of contents
* [General info](#general-info)
* [Install & run](#install-&-run)
* [Configuration file](#configuration-file)
* [Features](#features)
* [Status](#status)
* [Acknowledgements](#acknowledgements)
* [Contact](#contact)

## General info
_ShearCo_ is a C++-based code that calculates weak lensing two-point correlation functions efficiently. Currently, only shear-matter correlation functions (galaxy-galaxy lensing signals) an be computed which will be expanded in the near future. It is suitable for large data sets and independent of the survey footprint. This code estimates and/or mitigates weak lensing magnification effects. A detailed account is given in the [@PhD thesis](.) (not published yet) and an abridged version can be found in the appendix of [@Unruh2020](https://ui.adsabs.harvard.edu/abs/2020A%26A...638A..96U/abstract).


## Install & run
Dependencies:
* standard C compiler
* [@FFTW3](http://www.fftw.org)

Install:
* go to your desired directory (as of now, there is only one `cd shear-matter-corr` :P)
* compile with `make`

Run:
* run as `./bin/shearco`
* option `-c your_config_file` lets you specify a configuration file
* option `-o`: no output file is generated, the result will be displayed in the terminal
* option `-q`: shuts up _all_ communication from the running code (including progress, warnings and errors) except missing config files 

Input catalogs:
* the form should be [xpos ypos shear1 shear2 weight], where the shear is ignored for lens galaxies
* the first line can but doesn't need to contain the number of objects (for long files it is recommended though)
* comment lines are allowed with the usual specifiers [#, %, /, //, !]

Testing the code:
* the folder `test` contains a lens and a source input catalog, the test config file and the test output file
* run with `./bin/shearco -c test_config` and compare "tshear.ascii" and "test_tshear.ascii"


## Configuration file
The default is "config" has to contain the following entries. If you do not give a config file, the terminal will display a dialogue to build one. You can specify a config file using the `-c` option.

COMPULSORY
* input_lens_file     string   ./test/cat_lens.ascii   lens galaxy catalogue in format [xpos ypos 0 0 weight]
* input_source_file   string   ./test/cat_source.ascii source galaxy catalogue in format [xpos ypos shear1 shear2 weight]
* theta_in            double   0.01                    inner radius in input catalog units
* theta_out           double   0.30                    inner radius in input catalog units
* N_annuli            integer  10                      number of bins

OPTIONAL
* output_data_dir     string   ./                      output directory
* N_pix               integer  4096                    number of pixel N_pix^2 that the FFT uses (limits the precision)
* bin_type            string   log                     logarithmic or linear binning [lin, log]
* bin_in_R            bool     false                   option to using comoving coord instead of angular units from catalog

IF bin_in_R = true:
* conv_R2theta        double   0.03                    since the conversion from R to theta is cosmology dependent, the factor must be specified here

<table style="width:100%">
  <tr>
    <th>Firstname</th>
    <th>Lastname</th> 
    <th>Age</th>
  </tr>
  <tr>
    <td>Jill</td>
    <td>Smith</td> 
    <td>50</td>
  </tr>
  <tr>
    <td>Eve</td>
    <td>Jackson</td> 
    <td>94</td>
  </tr>
</table>


## Features
* calculates the galaxy-shear correlation functions (galaxy-galaxy lensing signal)
* independent on the number of objects
* works with masks
* works on any survey footprint
* binning in angular or comoving coordinates

To-do list:
* magnification effects in galaxy-galaxy lensing (already coded, needs to to be made compatible with this version)
* implement convergence as an alternative to shear
* calculation of shear-shear correlations
* different input formats like .bin, .fits
* optimize to some simulations I already used (Millennium, SLICS)


## Status
Project is: _in progress_ (motto is slowly but steady)
Last Update: July 2020


## Acknowledgements
I thank Sven Heydenreich for thoroughly testing the code (aka bug hunting), Peter Schneider and Pierre Burger for valuable discussions. 

## Contact
Created by [@SandraUnruh](sandra.unruh@uni-bonn.de) - feel free to contact me!
I am grateful for all kinds of suggestion, criticism, bugs and encouragement as this project emerged from my PhD thesis and my experience is naturally limited.