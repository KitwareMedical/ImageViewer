LINK_LIBRARIES (
  ITKFEM
  ITKCommon
  ITKNumerics
  ITKBasicFilters 
  ITKIO  
)

IF(FEM_VISUALIZATION)
# Define the macro that includes all the required functions and include MFC support
ADD_DEFINITIONS( -DFEM_BUILD_VISUALIZATION /D "_AFXDLL" )
ENDIF(FEM_VISUALIZATION)

SET(FEMImageRegLMEx_SRCS
  FEMImageRegLMEx.cxx
)

# Header and template source files for FEM project
# we can only include them if compiling on windows
IF(NOT UNIX)
IF(WIN32)
SET(FEMImageRegLMEx_SRCS ${FEMImageRegLMEx_SRCS}
  FEMImageRegLMEx.h
)
ENDIF(WIN32)
ENDIF(NOT UNIX)




# Create project files for all examples
ADD_EXECUTABLE(FEMRead FEMRead.cxx)
ADD_EXECUTABLE(FEMTruss FEMTruss.cxx)
ADD_EXECUTABLE(FEMImageRegLMEx ${FEMImageRegLMEx_SRCS})
ADD_EXECUTABLE(FEMSolverHyperbolicExample FEMSolverHyperbolicExample.cxx)
