InsightApplications README
--------------------------

This directory contains a variety of applications written with ITK. Many are
quite simple, come are complex and include GUI. Insight/Applications was
originally a directory within the Insight source code repository. However,
because of the size and complexity of this directory, the applications are
now a separate software module (e.g., CVS checkout). Please make sure that
you successfully build the core ITK prior to building this directory. Many of
these applications are tricky to build and depend on external packages. 

Some important configuration details:

     * VTK requires VTK version 4.2

     * CMake version 1.6.5 or later should be used.

     * FLTK version 1.1.2 is required

     * Qt version is required

Building with any other configurations is liable to give problems. 
Unfortunately, developers are unlikely to help with problems due to
using other configurations.



