PROJECT( vtkITK )

INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${vtkITK_SOURCE_DIR}/vtkITK.cmake)

INCLUDE_DIRECTORIES ( ${vtkITK_SOURCE_DIR}/Common )

# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
