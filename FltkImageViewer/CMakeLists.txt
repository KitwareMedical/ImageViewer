##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

##############################################################################
#
# Program:   Insight Segmentation & Registration Toolkit
#
# Copyright (c) 2002 Insight Consortium. All rights reserved.
# See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#    This software is distributed WITHOUT ANY WARRANTY; without even
#    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#    PURPOSE.  See the above copyright notices for more information.
#
##############################################################################

include_directories(  ${ImageViewer_SOURCE_DIR}/FltkImageViewer
  ${ImageViewer_BINARY_DIR}/FltkImageViewer )

set( FltkImageViewer_GUI_SRCS
  fltkClippingPlane3DDrawerGUI.fl
  fltkDisplayGlWindowFlatGUI.fl
  fltkDisplayGlWindowGUI.fl
  fltkFrustumFunctionControlGUI.fl
  fltkImage2DViewerGUI.fl
  fltkImageViewerGUI.fl
  fltkRegularStepGradientDescentGUI.fl
  fltkRGBImage2DViewerGUI.fl
  fltkSlice2DDrawerGUI.fl
  fltkSlice3DDrawerGUI.fl
  fltkSphereFunctionControlGUI.fl
  fltkTimeProbesCollectorGUI.fl )
  
fltk_wrap_ui( FltkImageViewer ${FltkImageViewer_GUI_SRCS} )

set( FltkImageViewer_SRCS
  fltkBox3D.cxx
  fltkClippingPlane3DDrawer.cxx
  fltkCone3D.cxx
  fltkCylinder3D.cxx
  fltkFrustum3D.cxx
  fltkGlWindow.cxx
  fltkGlWindowInteractive.cxx
  fltkGlWindowInteractiveFlat.cxx
  fltkImage2DViewerWindow.cxx
  fltkLightButton.cxx
  fltkProgressBar.cxx
  fltkRGBImage2DViewerWindow.cxx
  fltkShape3D.cxx
  fltkSphere3D.cxx
  fltkTimeProbeGUI.cxx
  fltkTimeProbesCollector.cxx
  fltkUtils.cxx
  fltkWindowInteractive.cxx
  ${FltkImageViewer_FLTK_UI_SRCS} )
  
DisableFLTKWarnings( "${FltkImageViewer_SRCS}" )

if( CMAKE_COMPILER_IS_GNUCXX )
  set_source_files_properties( fltkGlWindow.cxx
    PROPERTIES COMPILE_FLAGS -Wno-cast-qual )
  set_source_files_properties( fltkImage2DViewerWindow.cxx
    PROPERTIES COMPILE_FLAGS -Wno-cast-qual )
  set_source_files_properties( fltkRGBImage2DViewerWindow.cxx
    PROPERTIES COMPILE_FLAGS -Wno-cast-qual )
endif( CMAKE_COMPILER_IS_GNUCXX )

if( WIN32 )
  add_definitions( -DWIN32 )
endif( WIN32 )

add_library( FltkImageViewer ${FltkImageViewer_SRCS}
  ${FltkImageViewer_FLTK_UI_SRCS} )
target_link_libraries( FltkImageViewer ${FLTK_LIBRARIES} ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY} )

install( TARGETS FltkImageViewer DESTINATION lib )
