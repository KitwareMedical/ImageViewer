
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
)

LINK_LIBRARIES (
ITKCommon
ITKIO
)

SET( ShapeDetect_SRCS
ShapeDetectApp.cxx
ShapeDetection.cxx
)


ADD_EXECUTABLE( ShapeDetection ${ShapeDetect_SRCS} )


