PROJECT( SegmentationEditor )

SET( WSTCommon_SRCS
vtkBinaryVolume.cxx
vtkBinaryVolumeLogic.cxx
vtkBoundingBox.cxx
vtkPatchedImageReader.cxx
vtkPatchedLookupTable.cxx
vtkWSBoundingBoxManager.cxx
vtkWSLookupTableManager.cxx
vtkLookupTableEquivalencyHash.cxx
vtkITKWatershedFilterAndWriter.h
)


SET_SOURCE_FILES_PROPERTIES(
vtkLookupTableEquivalencyHash.cxx  WRAP_EXCLUDE)

ADD_LIBRARY ( vtkWSTCommon ${WSTCommon_SRCS} )

# Link in with ITK
INCLUDE_DIRECTORIES ( ${SegmentationEditor_SOURCE_DIR}/../../vtkITK/Common/ )
INCLUDE_DIRECTORIES ( ${SegmentationEditor_SOURCE_DIR}/Common/ )

LINK_DIRECTORIES ( ${SegmentationEditor_BINARY_DIR}/../../vtkITK/Common )
LINK_DIRECTORIES ( ${SegmentationEditor_BINARY_DIR}/Common )


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

