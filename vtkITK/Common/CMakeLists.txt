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

# Configure Tcl wrappers.
VTK_WRAP_TCL2(vtkVTKITKCommonTCL VTKITKCommonTCL_SRCS ${VTKITKCommon_SRCS})
ADD_LIBRARY ( vtkVTKITKCommonTCL ${VTKITKCommonTCL_SRCS} )
TARGET_LINK_LIBRARIES(vtkVTKITKCommonTCL vtkFilteringTCL vtkCommonTCL)

# Configure Python wrappers.
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON2(vtkVTKITKCommonPython VTKITKCommonPython_SRCS
                   ${VTKITKCommon_SRCS})
  ADD_LIBRARY(vtkVTKITKCommonPython SHARED ${VTKITKCommonPython_SRCS})
  TARGET_LINK_LIBRARIES (vtkVTKITKCommonPython 
                         vtkVTKITKCommon
                         vtkFilteringPython 
                         vtkCommonPython)
ENDIF (VTK_WRAP_PYTHON)

TARGET_LINK_LIBRARIES(vtkVTKITKCommon vtkImaging
                      ITKIO ITKNumerics ITKBasicFilters)
