INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Numerics/FEM
${ITK_SOURCE_DIR}/Code/Common
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Numerics/FEM
${ITK_BINARY_DIR}/Code/Common
)

LINK_LIBRARIES (
  VXLNumerics
  FEM
  ITKCommon
)

IF(FEM_VISUALIZATION)
# Define the macro that includes all the required functions and include MFC support
ADD_DEFINITIONS( -DFEM_BUILD_VISUALIZATION /D "_AFXDLL" )
ENDIF(FEM_VISUALIZATION)

# Create project files for all examples
ADD_EXECUTABLE(FEMRead FEMRead)
ADD_EXECUTABLE(FEMTruss FEMTruss)
