INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Numerics/FEM
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_BINARY_DIR}/Code/Numerics/vxl/vcl
#${ITK_SOURCE_DIR}/Auxiliary/MetaImage

)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Numerics/FEM
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/Numerics
${ITK_BINARY_DIR}/Code/Numerics/FEM
${ITK_BINARY_DIR}/Code/Numerics/vxl
${ITK_BINARY_DIR}/Code/Numerics/vxl/vcl
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
#${ITK_BINARY_DIR}/Auxiliary/MetaImage

)

LINK_LIBRARIES (
  VXLNumerics
  FEM
  ITKCommon
  ITKNumerics
  ITKBasicFilters 
 # MetaImageIO
  ITKIO  
)

IF(FEM_VISUALIZATION)
# Define the macro that includes all the required functions and include MFC support
ADD_DEFINITIONS( -DFEM_BUILD_VISUALIZATION /D "_AFXDLL" )
ENDIF(FEM_VISUALIZATION)

SOURCE_FILES(FEMImageRegEx_SRCS
  FEMImageRegEx.cxx
)

# Header and template source files for FEM project
# we can only include them if compiling on windows
IF(NOT UNIX)
IF(WIN32)
SOURCE_FILES(FEMImageRegEx_SRCS 
  FEMImageRegEx.h
)
ENDIF(WIN32)
ENDIF(NOT UNIX)




# Create project files for all examples
ADD_EXECUTABLE(FEMRead FEMRead)
ADD_EXECUTABLE(FEMTruss FEMTruss)
ADD_EXECUTABLE(FEMImageRegEx FEMImageRegEx_SRCS)
