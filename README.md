# ShearCo
Version 1.0 (07/2020) by Sandra Unruh<br>
Second-order correlation functions with fast-Fourier transforms in C++

## Table of contents
* [General info](#general-info)
* [Install and run](#install-and-run)
* [Configuration file](#configuration-file)
* [Features](#features)
* [Status](#status)
* [Acknowledgements](#acknowledgements)
* [Contact](#contact)

## General info
_ShearCo_ is a C++-based code that calculates weak lensing two-point correlation functions efficiently. Currently, only shear-matter correlation functions (galaxy-galaxy lensing signals) an be computed which will be expanded in the near future. It is suitable for large data sets and independent of the survey footprint. This code estimates and/or mitigates weak lensing magnification effects. A detailed account is given in the [@PhD thesis](.) (not published yet) and an abridged version can be found in the appendix of [@Unruh2020](https://ui.adsabs.harvard.edu/abs/2020A%26A...638A..96U/abstract).


## Install and run
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
* run with `./bin/shearco -c test/test_config` and compare "tshear.ascii" and "test_tshear.ascii"


## Configuration file
The default is "config". It has to contain the following entries. If you do not give a config file, the terminal will display a dialogue to build one. You can specify a config file using the `-c` option.

<table style="width:100%">
  <tr>
    <th colspan="4">COMPULSORY</th>
  </tr>
  <tr>
    <td>input_lens_file</td>
    <td>string</td> 
    <td>./test/cat_lens.ascii</td>
    <td>lens galaxy catalogue in format [xpos ypos 0 0 weight]</td>
  </tr>
  <tr>
    <td>input_source_file</td>
    <td>string</td> 
    <td>./test/cat_source.ascii</td>
    <td>source galaxy catalogue in format [xpos ypos shear1 shear2 weight]</td>
  </tr>
  <tr>
    <td>theta_in</td>
    <td>double</td> 
    <td>0.01</td>
    <td>inner radius in input catalog units</td>
  </tr>
  <tr>
    <td>theta_out</td>
    <td>double</td> 
    <td>0.3</td>
    <td>outer radius in input catalog units</td>
  </tr>
  <tr>
    <td>N_annuli</td>
    <td>integer</td> 
    <td>10</td>
    <td>number of bins</td>
  </tr>
</table>

<table style="width:100%">
  <tr>
    <th colspan="4">OPTIONAL</th>
  </tr>
  <tr>
    <td>output_data_dir</td>
    <td>string</td> 
    <td>./</td>
    <td>output directory</td>
  </tr>
  <tr>
    <td>N_pix</td>
    <td>integer</td> 
    <td>4096</td>
    <td>number of pixel N_pix^2 that the FFT uses (limits the precision)</td>
  </tr>
  <tr>
    <td>bin_type</td>
    <td>string</td> 
    <td>log</td>
    <td>logarithmic or linear binning [lin, log]</td>
  </tr>
  <tr>
    <td>bin_in_R</td>
    <td>bool</td> 
    <td>false</td>
    <td>option to use comoving coordinates instead of angular units from the catalog</td>
  </tr>
</table>

<table style="width:100%">
  <tr>
    <th colspan="4">IF bin_in_R = true:</th>
  </tr>
  <tr>
    <td>conv_R2theta</td>
    <td>double</td> 
    <td>0.03</td>
    <td>since the conversion from R to theta is cosmology dependent, the factor must be specified here</td>
  </tr>
</table>


## Features
* calculates the galaxy-shear correlation functions (galaxy-galaxy lensing signal)
* independent of the number of objects
* works with masks
* works on any survey footprint
* binning in angular or comoving coordinates

To-do list:
* magnification effects in galaxy-galaxy lensing (coding complete)
* implement convergence as an alternative to shear (coding complete)
* calculation of shear-shear correlations
* different input formats like .bin, .fits
* optimize to some simulations I already used (Millennium, SLICS)
* go beyond flat sky approximation


## Status
Project is: _in progress_ (motto is slowly but steady) <br>
Last Update: July 2020


## Acknowledgements
I thank Sven Heydenreich for thoroughly testing the code (aka bug hunting), Peter Schneider and Pierre Burger for valuable discussions. 

## Contact
Created by [@SandraUnruh](sandra.unruh@uni-bonn.de) - feel free to contact me!
I am grateful for all kinds of suggestion, criticism, bugs and encouragement as this project emerged from my PhD thesis and my experience is naturally limited.