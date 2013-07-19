ImageViewer
===========

[![Build Status](https://travis-ci.org/TubeTK/ImageViewer.png?branch=master)](https://travis-ci.org/TubeTK/ImageViewer)

Description
-----------

ImageViewer is a cross-platform [ITK](http://www.itk.org) and [FLTK](http://www.fltk.org)-based medical image viewer. This simple-yet-effective slice-based viewer works on 2D and 3D images and supports the probing of data values.

This application was extracted for [TubeTK](http://www.tubetk.org) from the [ITKApps](http://itk.org/ITKApps.git) repository with ```git subtree```.

Building
--------

Building requires [CMake](http://www.cmake.org) and a C++ compiler. To build:

```shell
git clone https://github.com/TubeTK/ImageViewer.git
mkdir ImageViewer-build
cd ImageViewer-build
cmake ../ImageViewer
make -j3
```

The resulting executable can be found at *ImageViewer-build/ImageViewer/ImageViewer*.

Usage
-----

Pass in images to view as command line arguments. Press **h** to see keyboard shortcuts.

