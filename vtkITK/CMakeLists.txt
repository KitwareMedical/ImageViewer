INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${ITK_SOURCE_DIR}/Applications/vtkITK/vtkITK.cmake)

# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
