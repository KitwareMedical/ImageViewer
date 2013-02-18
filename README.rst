ImageViewer
===========

Description
-----------

This is cross-platform ITK_ and FLTK_-based medical image viewer.  This simple
yet effective slice-based viewer works on 2D and 3D images and supports probing
of data values.

This application was extracted for TubeTK_ from the ITKApps_ repository with git
subtree.

Building
--------

Building requires CMake_ and a C++ compiler.  To build::

  git clone https://github.com/TubeTK/ImageViewer.git
  mkdir ImageViewer-Build
  cd ImageViewer-Build
  cmake ../ImageViewer
  make -j3

The resulting executable can be found at
*ImageViewer-build/ImageViewer/ImageViewer*.

Usage
-----

Pass in images to view as command line arguments.  Press *h* to see keyboard
shortcuts.

.. _CMake:                 http://cmake.org/
.. _FLTK:                  http://www.fltk.org/
.. _ITK:                   http://itk.org/
.. _ITKApps:               http://itk.org/ITKApps.git
.. _TubeTK:                http://tubetk.org
