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
# if we are wrapping into Python then add the library and extra 
# source files
#
IF (VTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON(vtkWSTCommonPython WSTCommonPython_SRCS WSTCommon_SRCS)
  ADD_LIBRARY(vtkWSTCommonPython SHARED WSTCommonPython_SRCS)
  TARGET_LINK_LIBRARIES (vtkWSTCommonPython 
                         vtkFilteringPython 
                         vtkCommonPython)
  IF(WIN32)
    TARGET_LINK_LIBRARIES (vtkWSTCommonPython 
                           debug ${PYTHON_DEBUG_LIBRARY} 
                           optimized ${PYTHON_LIBRARY})
  ELSE(WIN32)
    TARGET_LINK_LIBRARIES (vtkWSTCommonPython ${PYTHON_LIBRARY})
  ENDIF(WIN32)

ENDIF (VTK_WRAP_PYTHON)


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

