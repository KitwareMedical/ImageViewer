CMAKE_MINIMUM_REQUIRED(VERSION 1.6)
PROJECT(VolviewPlugIns)

# Don't build anything unless the version of CMake is high enough.
# The matching ELSE/ENDIF should be the last lines in the file.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)


#
#  Find where ITK was Built or Installed.
#
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build VolviewPlugIns without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)



#
#   Find the directory where the Header 
#   for Volview Plugins API can be found.
#
#   This file can be found in the source 
#   tree of VolView Light
#
FIND_PATH(VOLVIEW_PLUGIN_INCLUDE_DIR vtkVVPluginAPI.h)
INCLUDE_DIRECTORIES(${VOLVIEW_PLUGIN_INCLUDE_DIR})
IF( VOLVIEW_PLUGIN_INCLUDE_DIR )
  INCLUDE_DIRECTORIES(${VOLVIEW_PLUGIN_INCLUDE_DIR})
ENDIF( VOLVIEW_PLUGIN_INCLUDE_DIR )


#
#  Find where the Plugin library should be finally copied
#
FIND_PATH( VOLVIEW_PLUGINS_DIR  vvBoundary.so "Where the shared libraries of the Plugins should be copied")



#
#  Define where to build the shared library
#
SET (LIBRARY_OUTPUT_PATH ${VolviewPlugIns_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")



# list all plugins here
SET (PLUGINS 
vvCurvatureFlow 
vvGradientAnisotropicDiffusion
vvCurvatureAnisotropicDiffusion
vvConfidenceConnected
vvSigmoid
)

# probably could be cleaned up...
ADD_LIBRARY(vvCurvatureFlow MODULE itkVVCurvatureFlowPlugin.cxx)
TARGET_LINK_LIBRARIES(vvCurvatureFlow ITKCommon ITKBasicFilters)

# probably could be cleaned up...
ADD_LIBRARY(vvGradientAnisotropicDiffusion MODULE itkVVGradientAnisotropicDiffusionPlugin.cxx)
TARGET_LINK_LIBRARIES(vvGradientAnisotropicDiffusion ITKCommon ITKBasicFilters)

# probably could be cleaned up...
ADD_LIBRARY(vvCurvatureAnisotropicDiffusion MODULE itkVVCurvatureAnisotropicDiffusionPlugin.cxx)
TARGET_LINK_LIBRARIES(vvCurvatureAnisotropicDiffusion ITKCommon ITKBasicFilters)

# probably could be cleaned up...
ADD_LIBRARY(vvConfidenceConnected MODULE itkVVConfidenceConnectedPlugin.cxx)
TARGET_LINK_LIBRARIES(vvConfidenceConnected ITKCommon ITKBasicFilters)

# probably could be cleaned up...
ADD_LIBRARY(vvSigmoid MODULE itkVVSigmoidPlugin.cxx)
TARGET_LINK_LIBRARIES(vvSigmoid ITKCommon ITKBasicFilters)


# copy the plugins to a plugin directory
FOREACH (PLUG ${PLUGINS})
  ADD_CUSTOM_COMMAND(
#    SOURCE ${LIBRARY_OUTPUT_PATH}/${INTDIR}/${PLUG}${CMAKE_SHARED_MODULE_SUFFIX}
    SOURCE ${PLUG}
    COMMAND ${CMAKE_COMMAND} 
    ARGS -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/${CMAKE_SHARED_MODULE_PREFIX}${PLUG}${CMAKE_SHARED_MODULE_SUFFIX} ${VOLVIEW_PLUGINS_DIR}/${PLUG}${CMAKE_SHARED_MODULE_SUFFIX}
    TARGET ${PLUG}
    OUTPUTS ${VOLVIEW_PLUGINS_DIR}/Plugins/${CMAKE_SHARED_MODULE_PREFIX}${PLUG}${MODULE_EXTENSION})
ENDFOREACH (PLUG)




# If the version of CMake was too old, complain and build nothing.
# These should be the last lines in this file.
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)
  MESSAGE(SEND_ERROR
          "This version of CMake is too old to build VolviewPlugIns.  "
          "Please upgrade to CMake 1.6.")
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.5)


