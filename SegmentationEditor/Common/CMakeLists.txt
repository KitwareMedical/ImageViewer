SOURCE_FILES( WSTCommon_SRCS
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
vtkLookupTableEquivalencyHash
vtkITKWatershedFilterAndWriter
)

WRAP_EXCLUDE_FILES(
vtkLookupTableEquivalencyHash
vtkBoundingBoxHash.h
)

ADD_LIBRARY ( vtkWSTCommon WSTCommon_SRCS )

# Link in with ITK
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Code/Numerics/ )
INCLUDE_DIRECTORIES ( ${ITK_SOURCE_DIR}/Examples/vtkITK/Common/ )

LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Examples/Examples/vtkITK/Common )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Algorithms )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/BasicFilters )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Common )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/IO )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Numerics )
LINK_DIRECTORIES ( ${ITK_BINARY_DIR}/Code/Numerics/Statistics )


# if we are wrapping into Tcl then add the library and extra 
# source files
#
  VTK_WRAP_TCL(vtkWSTCommonTCL WSTCommonTCL_SRCS WSTCommon_SRCS)
  ADD_LIBRARY ( vtkWSTCommonTCL WSTCommonTCL_SRCS )
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

