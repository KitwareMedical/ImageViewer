
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKIO
)

SOURCE_FILES( ShapeDetect_SRCS
ShapeDetectApp.cxx
ShapeDetection.cxx
)


ADD_EXECUTABLE( ShapeDetection ShapeDetect_SRCS )


