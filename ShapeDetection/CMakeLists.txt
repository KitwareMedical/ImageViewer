
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
)

SOURCE_FILES( ShapeDetect_SRCS
ShapeDetectApp
ShapeDetection )


ADD_EXECUTABLE( ShapeDetection ShapeDetect_SRCS )


