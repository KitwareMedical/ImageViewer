TubeTK ImageViewer
==================

Description
-----------

ImageViewer is a cross-platform image viewer based on [ITK](http://www.itk.org). This simple-yet-effective slice-based viewer works on 2D and 3D images, supports the probing of data values, painting overlays on images, and collecting landmarks from images.

This application was originally bundled with the [ITKApps](http://itk.org/ITKApps.git) repository.

Building
--------

Building requires [CMake](http://www.cmake.org), ITK, SlicerExectuionModel, Qt5, and a C++ compiler. 


*1) CMake*

Compling ITK (and therefore ImageViewer) requires that you have CMake 3.11 or later.

You can download CMake from http://www.cmake.org or via ```apt-get cmake-gui```

*2) ITK*

For ITK, we want to compile ITK v5.0 or later.   Begin by checking out ITK's source

    $ cd /                              (To keep paths short, start at a top-level dir)
    $ mkdir src
    $ cd src
    $ git clone https://github.com:/InsightSoftwareConsortium/ITK.git -b Release
    $ mkdir ITK-Release
    $ cmake-gui ../ITK

Using CMake, you should configure ITK with the following options
* CMAKE_BUILD_TYPE = Release           (This is an advanced option)
* ITK_LEGACY_SILENT = On               (This is an advanced option)
* ITK_WRAP_PYTHON = On                 (Note: Only Python3 is supported at this time)
* Module_MinimalPathExtraction = On    (This is an advanced option)
* You may also want to turn off building Tests and Examples

Once you have configured and generated your build files (e.g., for make or ninja or whatever):

    $ ninja  (or make, or nmake, or whatever is appropriate for your system)

This build process can take many hours on a Windows PC, because it is generating the files needed
to use ITK with Python, as well as the standard C++ libraries, applications, examples, and tests.

*3) SlicerExecutionModel*

After compiling ITK, we repeat the build process for SlicerExecutionModel (used by ITKTubeTK applications):

    $ cd /src
    $ git clone https://github.com:/Slicer/SlicerExecutionModel.git
    $ mkdir SlicerExecutionModel-Release
    $ cd SlicerExecutionModel-Release
    $ cmake-gui ../SlicerExecutionModel

Using CMake, you should configure SlicerExecutionModel as follows:
* CMAKE_BUILD_TYPE = Release
* ITK_DIR = /src/ITK-Release

Once SlicerExecutionModel's cmake files are configured and build files are
generated, you should build the application:

    $ ninja  (or make, or nmake, or whatever is appropriate for your system)

This build should be relatively quick.

*4) Qt5*

Install Qt5 developer packages via [qt.io](qt.io) or via apt-get.

*5) ImageViewer*

Installing 

    $ cd /src
    $ git clone https://github.com/KitwareMedical/ImageViewer.git
    $ mkdir ImageViewer-Release
    $ cd ImageViewer-Release
    $ cmake-gui ../ImageViewer
    
Using CMake, you should configure ImageViewer as follows:
* CMAKE_BUILD_TYPE = Release
* ITK_DIR = /src/ITK-Release
* SlicerExecutionModel_DIR = /src/SlicerExecutionModel-Release
* Qt5_DIR = (should be populated automatically, otherwise set it for your system)

Then configure and generate your build files using cmake, and compile

    $ ninja   (or make or whatever)
    
Now you will want to add ImageViewer's executable to your command-line PATH.
The directory to include in your PATH is:
    /src/ImageViewer-release/ImageViewer/ImageViewer/bin.

Usage
-----

    $ ImageViewer -h 
    
Returns the following

```
USAGE: 

  /src/ImageViewer-Release/ImageViewer/bin/ImageViewer
     [--returnparameterfile <std::string>]
     [--processinformationaddress <std::string>]
     [--xml]
     [--echo]
     [-d <Min|Max|Flip>]
     [-e <Min|Max|Flip>]
     [-l <Value|Inverse|Log|Deriv-X|Deriv-Y|Deriv-Z|Blend|MIP>]
     [-T]
     [-A]
     [-V]
     [-P]
     [-D <int>]
     [-C]
     [-b <double>]
     [-x]
     [-y]
     [-z]
     [-t]
     [-m <double>]
     [-a <double>]
     [-q <double>]
     [-s <int>]
     [-o <int>]
     [-j <std::string>]
     [--]
     [--version]
     [-h]
     <std::string>


Where: 

   --returnparameterfile <std::string>
     Filename in which to write simple return parameters (int, float,
     int-vector, etc.) as opposed to bulk return parameters (image,
     geometry, transform, measurement, table).

   --processinformationaddress <std::string>
     Address of a structure to store process information (progress, abort,
     etc.). (value: 0)

   --xml
     Produce xml description of command line arguments (value: 0)

   --echo
     Echo the command line arguments (value: 0)

   -d <Min|Max|Flip>,  --iwMin <Min|Max|Flip>
     Toggle between clipping and setting to black values above IW lower
     limit. (value: Min)

   -e <Min|Max|Flip>,  --iwMax <Min|Max|Flip>
     Toggle between clipping and setting to black values above IW upper
     limit. (value: Max)

   -l <Value|Inverse|Log|Deriv-X|Deriv-Y|Deriv-Z|Blend|MIP>,  --mode <Value
      |Inverse|Log|Deriv-X|Deriv-Y|Deriv-Z|Blend|MIP>
     Toggle the mode as the data is viewed.

   -T,  --points
     Toggle display of clicked points. (value: 0)

   -A,  --axis
     View axis labels: P posterior, L left and S superior. (value: 0)

   -V,  --value
     Disable Viewing of image values at last clicked point. (value: 0)

   -P,  --physical
     Toggle coordinates display between index and physical units. (value:
     0)

   -D <int>,  --details <int>
     Display details as an overlay on the image, inside a Controls Widget
     or turn it off. (value: 1)

   -C,  --crosshairs
     Disable viewing location of last position clicked via crosshairs.
     (value: 0)

   -b <double>,  --opacity <double>
     Set the overlay opacity. (value: 1)

   -x,  --xFlipped
     Flip the X axis. (value: 0)

   -y,  --yFlipped
     Flip the Y axis. (value: 0)

   -z,  --zFlipped
     Flip the Z axis. (value: 0)

   -t,  --transpose
     Transpose the axis of the slice being viewed. (value: 0)

   -m <double>,  --zoom <double>
     Set zoom value. (value: 1)

   -a <double>,  --minIntensity <double>
     Set the lower of the intensity widowing. (value: 0)

   -q <double>,  --maxIntensity <double>
     Set the upper of the intensity widowing. (value: 0)

   -s <int>,  --sliceOffset <int>
     Set slices number. (value: -1)

   -o <int>,  --orientation <int>
     Set slices orientation. (value: 2)

   -j <std::string>,  --overlayImage <std::string>
     Overlay Image.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <std::string>
     (required)  Input Image.


   Description: No description provided

   Author(s): Kitware

   Acknowledgements: This work is part of the TubeTK-ImageViewer project at
   Kitware.

```

Commands while running ImageViewer
----------------------------------

Once an image (and optional overlay) have been loaded and are being displayed, key commands are available to manipulate the visualization.   Pressing 'h' bring us a current list of commands.   Those commands include the following:

```
   0 - View slices along the x-axis
   1 - View slices along the y-axis
   2 - View slices along the z-axis (default)

   > , - View the next slice
   < , - View the previous slice

   r -reset all options
   h - help (this document)

   x - Flip the x-axis
   y - Flip the y-axis
   z - Flip the z-axis

   f - Toggle fast-pace mode: quicker movement and intensity windowing

   q w - Decrease, Increase the upper limit of the intensity windowing
   e - Toggle between clipping and setting-to-black values above IW upper limit
   
   a s - Decrease, Increase the lower limit of the intensity windowing
   d - Toggle between clipping and setting-to-white values below IW lower limit
   
   + = - Zoom-in by a factor of 2
   - _ - Zoom-out by a factor of 2
   
   i j k m - Shift the image in the corresponding direction
   t - Transpose the axis of the slice being viewed
   
   A - View axis labels: P=posterior, L=left, S=superior
   C - View crosshairs that illustrate last user-click in the window
   V - View image values as the user clicks in the window
   T - Toggle display of clicked points
   P - Toggle coordinates display between index and physical units
   D - View image details as an overlay on the image
   O - View a color overlay (application dependent)
   p - Save the clicked points in a file
   l - Toggle how the data is the window is viewed:
       Modes cycle between the following views:
         - Intensity
         - Inverse
         - Log
         - Derivative wrt x
         - Derivative wrt y
         - Derivative wrt z
         - Blend with previous and next slice
         - MIP
    
   \ - cycle between Select, Custom, and Paint mode
        - Default Custom is threshold connected components
   [ ] - increase / decrease paint sphere radius
   { } - increase / decrease paint color (0 erases)
   " - save the overlay to a file
```

"CUSTOM" mouse mode: Extending Capabilities
-------------------------------------------

It is possible to quickly and easily extend the functionality of ImageViewer by writing a callback function that is called each
time the user presses, moves, or relases the left mouse button.   This callback is typically added to the application's top-level .cxx file.   For example, see the
callback in ```ImageViewer/ImageViewer/ImageViewer.cxx```

By default, ImageViewer provides a threshold connected component filter as a "CUSTOM" method.   The first point selected by pressing the mouse button sets the lower/upper threshold and a seed point for ITK's threshold connected component filter.  If the mouse is dragged into another voxel, the current voxel pointed to by the mouse sets the opposing upper/lower threshold and the filter is re-run.   Output of this filter becomes the overlay of the image, which can then be edited using the PAINT mode (chosen by pressing the '\' key.

Acknowledgements
----------------

The development of TubeTK is supported in part by the

* [NCI](http://www.cancer.gov/) under award numbers R01CA138419, R01CA170665, R43CA165621, and R44CA143234;
* [NIBIB](http://www.nibib.nih.gov) (NBIB) of the National Institutes of Health (NIH) under award numbers R01EB014955, R41EB015775, R43EB016621, and U54EB005149;
* [NIBIB](http://www.nibib.nih.gov) and [NIGMS](http://www.nigms.nih.gov) R01EB021396;
* [NINDS](http://www.ninds.nih.gov) R42NS086295 and R41NS081792;
* [Defense Advanced Research Projects Agency](http://www.darpa.mil) (DARPA) under the TRUST program.

License
-------

This software is distributed under the Apache 2.0 license. Please see
the *LICENSE* file for details.
