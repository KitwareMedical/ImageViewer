PROJECT(EllipsoidInteriorExteriorSpatialFunction)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${EllipsoidInteriorExteriorSpatialFunction_SOURCE_DIR}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/BasicFilters
)

SET(ELLIPSOID_SPATIAL_FUNCTION_SRCS
itkEllipsoidInteriorExteriorSpatialFunctionExample.cxx
)

LINK_LIBRARIES (
ITKCommon
ITKIO
ITKBasicFilters
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(itkEllipsoidInteriorExteriorSpatialFunctionExample 
${ELLIPSOID_SPATIAL_FUNCTION_SRCS})

