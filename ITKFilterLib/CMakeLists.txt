# To make this a stand-alone project that
# can be compiled outside of itk, copy this
# directory outside of the Insight hierarchy
# and then uncomment the following lines.
# You can then point cmake at this file and
# it will create the library.   If this is
# out of the Insight hierarchy, it won't be
# affected by future cvs commands and you
# modify it to include other filters...

# --start uncomment --

# PROJECT(ITKFilterLib)
# FIND_PATH( ITK_BINARY_DIR itkConfigure.h )
# IF ( ITK_BINARY_DIR )
#   LOAD_CACHE(${ITK_BINARY_DIR})
#   INCLUDE (${ITK_SOURCE_DIR}/itkCMakeOptions.cmake)
# ENDIF (ITK_BINARY_DIR )
# INCLUDE_REGULAR_EXPRESSION(".*")
# INCLUDE_DIRECTORIES(
# ${ITK_SOURCE_DIR}/Code/Common
# ${ITK_SOURCE_DIR}/Code/IO
# ${ITK_SOURCE_DIR}/Code/BasicFilters
# ${ITK_SOURCE_DIR}/Code/Algorithms
# )

# -- End uncomment --

LINK_LIBRARIES (
Example_ITKFilterLib
)

LINK_LIBRARIES (
ITKNumerics
ITKCommon
ITKIO
ITKBasicFilters 
)


ADD_LIBRARY( Example_ITKFilterLib ITKFilterLib )
TARGET_LINK_LIBRARIES(Example_ITKFilterLib ITKCommon ITKNumerics ITKIO ITKBasicFilters)
ADD_EXECUTABLE( Example_ITKFilterLibTest ITKFilterLibTest.cxx )
TARGET_LINK_LIBRARIES(Example_ITKFilterLibTest Example_ITKFilterLib)



