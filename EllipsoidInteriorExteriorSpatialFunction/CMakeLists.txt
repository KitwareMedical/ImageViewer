INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/EllipsoidInteriorExteriorSpatialFunction
)

IF(WIN32)
  IF(NOT BORLAND)
    SET(LIBS wsock32)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

IF(UNIX)
  SET(LIBS -lm)
ENDIF(UNIX)

ADD_EXECUTABLE(itkEllipsoidInteriorExteriorSpatialFunctionExample 
               itkEllipsoidInteriorExteriorSpatialFunctionExample.cxx)
TARGET_LINK_LIBRARIES(itkEllipsoidInteriorExteriorSpatialFunctionExample
  ITKBasicFilters ITKIO ${LIBS}
)
