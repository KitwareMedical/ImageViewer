SOURCE_FILES( VTKITKCommon_SRCS
vtkITKAntiAliasBinaryImageFilter.h
vtkITKGradientAnisotropicDiffusionImageFilter.h
vtkITKGradientMagnitudeImageFilter.h
vtkITKMutualInformationTransform.cxx
vtkITKWatershedImageFilter.cxx
vtkITKNormalizeImageFilter.cxx
vtkITKDiscreteGaussianImageFilter.h
vtkITKConnectedThresholdImageFilter.cxx
vtkITKConfidenceConnectedImageFilter.cxx
vtkITKIsolatedConnectedImageFilter.cxx
vtkITKDanielssonDistanceMapImageFilter.cxx
vtkITKPDEDeformableRegistrationFilter.h
vtkITKImageToImageFilter.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterF3F3.h
vtkITKImageToImageFilter2DFF.h
)

ABSTRACT_FILES ( 
vtkITKImageToImageFilter.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterF3F3.h
vtkITKImageToImageFilter2DFF.h
)

ADD_LIBRARY ( vtkVTKITKCommon VTKITKCommon_SRCS )

# Link in with ITK
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Code/Numerics/ )
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Examples/vtkITK/Common/ )

LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Algorithms )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/BasicFilters )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Common )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/IO )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Numerics )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Numerics/Statistics )

# if we are wrapping into Tcl then add the library and extra 
# source files
#
  VTK_WRAP_TCL(vtkVTKITKCommonTCL VTKITKCommonTCL_SRCS VTKITKCommon_SRCS)
  ADD_LIBRARY ( vtkVTKITKCommonTCL VTKITKCommonTCL_SRCS )
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonTCL 
                         vtkFilteringTCL 
                         vtkCommonTCL 
                         ${TCL_LIBRARY})
# if we are wrapping into Python then add the library and extra 
# source files
#
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON(vtkVTKITKCommonPython VTKITKCommonPython_SRCS VTKITKCommon_SRCS)
  ADD_LIBRARY(vtkVTKITKCommonPython SHARED VTKITKCommonPython_SRCS)
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython 
                         vtkFilteringPython 
                         vtkCommonPython)
  IF(WIN32)
    TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython 
                           debug ${PYTHON_DEBUG_LIBRARY} 
                           optimized ${PYTHON_LIBRARY})
  ELSE(WIN32)
    TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython ${PYTHON_LIBRARY})
  ENDIF(WIN32)

ENDIF (VTK_WRAP_PYTHON)


LINK_LIBRARIES (
vtkCommon 
vtkFiltering
vtkImaging
vtkVTKITKCommon 
VXLNumerics
ITKCommon
ITKBasicFilters
ITKNumerics
ITKBasicFilters
)

