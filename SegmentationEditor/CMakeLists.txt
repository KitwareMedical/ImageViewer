PROJECT( SegmentationEditor )

INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${ITK_SOURCE_DIR}/Applications/SegmentationEditor/WST.cmake)

# Link in with ITK
INCLUDE_DIRECTORIES ( ${SegmentationEditor_SOURCE_DIR}/../vtkITK/Common )
INCLUDE_DIRECTORIES ( ${SegmentationEditor_SOURCE_DIR}/Common/ )

LINK_DIRECTORIES ( ${SegmentationEditor_BINARY_DIR}/../vtkITK/Common )
LINK_DIRECTORIES ( ${SegmentationEditor_BINARY_DIR}/Common )


# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
