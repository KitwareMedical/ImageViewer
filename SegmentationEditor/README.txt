1. To build this application:

* You will need VTK 4.0 installed on your machine.  In your VTK build, make
sure you include the Patented classes and Tcl wrapping.  You will need 
the Wrapping/Tcl directories from the VTK source tree.

* When you configure Itk with CMake, turn on the following options, which
will cause this application to build:

   BUILD_EXAMPLES
   USE_VTK 
   SEGMENTER_WRAP_TCL
   VTKITK_WRAP_TCL

* You may need to provide CMake with the VTK_BINARY_PATH, 
  VTK_INSTALL_PATH


2. To run this application:

 cd INSIGHT_SOURCE/Examples/SegmentationEditor/Scripts
 INSIGHT_BUILD/Examples/SegmentationEditor/Wrapping/Tcl/WSTITK Main.tcl

 where "INSIGHT_SOURCE" is the path to your Insight source tree and
 "INSIGHT_BUILD" is the path to your Insight build.


3. The rest of the documentation you will need is displayed by the program
itself.  Also see the documentation in the Examples section of the
www.itk.org website.


4. Report any problems to the Insight-users list (see www.itk.org).
