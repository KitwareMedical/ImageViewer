
PROJECT(BasicIOAndFilter)

# Find ITK
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF (USE_ITK_FILE)
  INCLUDE (${USE_ITK_FILE})
ENDIF (USE_ITK_FILE)

#indicate directores and libraries

LINK_LIBRARIES (
ITKIO
)

SET(BasicIOAndFilter_SRCS
BasicIOAndFilter.cxx
)

#specify executables
ADD_EXECUTABLE(BasicIOAndFilter ${BasicIOAndFilter_SRCS})
