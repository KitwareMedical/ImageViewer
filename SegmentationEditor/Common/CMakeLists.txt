SET( WSTCommon_SRCS
vtkBinaryVolume.cxx
vtkBinaryVolumeLogic.cxx
vtkBoundingBox.cxx
vtkPatchedImageReader.cxx
vtkPatchedLookupTable.cxx
vtkWSBoundingBoxManager.cxx
vtkWSLookupTableManager.cxx
vtkBinaryVolume.h
vtkBinaryVolumeLogic.h
vtkBoundingBox.h
vtkPatchedImageReader.h
vtkPatchedLookupTable.h
vtkWSBoundingBoxManager.h
vtkWSLookupTableManager.h
vtkLookupTableEquivalencyHash.cxx
vtkITKWatershedFilterAndWriter.h
)


SET_SOURCE_FILES_PROPERTIES(
vtkLookupTableEquivalencyHash.cxx  WRAP_EXCLUDE)

ADD_LIBRARY ( vtkWSTCommon ${WSTCommon_SRCS} )

# Link in with ITK
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Examples/vtkITK/Common/ )
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Examples/SegmentationEditor/Common/ )

LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Examples/Examples/SegmentationEditor/Common )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Examples/Examples/vtkITK/Common )


# if we are wrapping into Tcl then add the library and extra 
# source files
#
  VTK_WRAP_TCL(vtkWSTCommonTCL WSTCommonTCL_SRCS ${WSTCommon_SRCS})
  ADD_LIBRARY ( vtkWSTCommonTCL ${WSTCommonTCL_SRCS} )
  TARGET_LINK_LIBRARIES (vtkWSTCommonTCL 
                         vtkFilteringTCL 
                         vtkCommonTCL 
                         ${TCL_LIBRARY})

LINK_LIBRARIES (
vtkCommon 
vtkFiltering
vtkImaging
vtkWSTCommon 
vtkVTKITKCommon
VXLNumerics
ITKCommon
ITKBasicFilters
ITKNumerics
ITKBasicFilters
ITKIO
)

