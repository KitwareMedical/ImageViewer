SET( VTKITKCommon_SRCS
vtkITKAntiAliasBinaryImageFilter.h
vtkITKBilateralImageFilter.h
vtkITKConfidenceConnectedImageFilter.cxx
vtkITKConnectedThresholdImageFilter.cxx
vtkITKCurvatureAnisotropicDiffusionImageFilter.h
vtkITKDanielssonDistanceMapImageFilter.cxx
vtkITKDiscreteGaussianImageFilter.h
vtkITKGradientAnisotropicDiffusionImageFilter.h
vtkITKGradientMagnitudeImageFilter.h
vtkITKImageToImageFilter.h
vtkITKImageToImageFilter2DFF.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterULUL.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
vtkITKIsolatedConnectedImageFilter.cxx
vtkITKLaplacianSegmentationLevelSetImageFilter.h
vtkITKMutualInformationTransform.cxx
vtkITKNeighborhoodConnectedImageFilter.h
vtkITKNormalizeImageFilter.cxx
vtkITKPDEDeformableRegistrationFilter.h
vtkITKThresholdSegmentationLevelSetImageFilter.h
vtkITKTobogganImageFilter.h
vtkITKWatershedImageFilter.cxx
)

SET_SOURCE_FILES_PROPERTIES ( 
vtkITKImageToImageFilter.h
vtkITKImageToImageFilter2DFF.h
vtkITKImageToImageFilterFF.h
vtkITKImageToImageFilterFUL.h
vtkITKImageToImageFilterULUL.h
vtkITKImageToImageFilterUSUL.h
vtkITKImageToImageFilterUSUS.h
ABSTRACT
)

ADD_LIBRARY ( vtkVTKITKCommon ${VTKITKCommon_SRCS} )

# Link in with ITK

# if we are wrapping into Tcl then add the library and extra 
# source files
#
  VTK_WRAP_TCL(vtkVTKITKCommonTCL VTKITKCommonTCL_SRCS ${VTKITKCommon_SRCS})
  ADD_LIBRARY ( vtkVTKITKCommonTCL ${VTKITKCommonTCL_SRCS} )
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonTCL 
                         vtkFilteringTCL 
                         vtkCommonTCL 
                         ${TCL_LIBRARY})
# if we are wrapping into Python then add the library and extra 
# source files
#
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON(vtkVTKITKCommonPython VTKITKCommonPython_SRCS ${VTKITKCommon_SRCS})
  ADD_LIBRARY(vtkVTKITKCommonPython SHARED ${VTKITKCommonPython_SRCS})
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython 
                         vtkVTKITKCommon
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


TARGET_LINK_LIBRARIES (
vtkVTKITKCommon
vtkCommon 
vtkFiltering
vtkImaging
vtkVTKITKCommon 
vtkexpat
vtkftgl
vtkfreetype
VXLNumerics
ITKCommon
ITKBasicFilters
ITKNumerics
ITKBasicFilters
)

