PROJECT( vtkITK )

INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${ITK_SOURCE_DIR}/Applications/vtkITK/vtkITK.cmake)

INCLUDE_DIRECTORIES ( ${vtkITK_SOURCE_DIR}/Common )

# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
