PROJECT( ShapeDetection )

INCLUDE_DIRECTORIES(
${ShapeDetection_SOURCE_DIR}/../IBSRValidation/Common
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


