INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/FloodFilledSpatialFunction
)

LINK_LIBRARIES (
ITKCommon
VXLNumerics
)

ADD_EXECUTABLE(itkFloodFilledSpatialFunctionExample itkFloodFilledSpatialFunctionExample)
