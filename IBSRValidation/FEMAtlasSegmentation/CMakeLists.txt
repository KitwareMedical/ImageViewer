
SET(FEMBrainStripApp_SRCS
  Code/FEMBrainStripValidationApp.cxx
#  Code/
)

# Header and template source files for FEM project
# we can only include them if compiling on windows
IF(NOT UNIX)
IF(WIN32)
SET(FEMBrainStripApp_SRCS ${FEMBrainStripApp_SRCS}
  Code/FEMValidationInputParser.h
  Code/FEMValidationInputParser.txx
  Code/FEMPreprocessor.h
  Code/FEMPreprocessor.txx
  Code/FEMRegistrator.h
  Code/FEMRegistrator.txx
  Code/FEMAtlasLabeler.h
  Code/FEMAtlasLabeler.txx
  Code/FEMBrainStripValidationApp.h
  Code/FEMBrainStripValidationApp.txx
)
ENDIF(WIN32)
ENDIF(NOT UNIX)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/FEMAtlasSegmentation
)

SOURCE_GROUP("Source Files Template" ./*.txx)


# Add an option to build the visualization functions
IF(WIN32)
    OPTION(FEM_VISUALIZATION "Select whether the visualization functions will be built for Finite Elements Modeling (FEM) classes or not. If you select to build visualization functions, be sure to define FEM_BUILD_VISUALIZATION macro before including the FEM classes.")
    MARK_AS_ADVANCED( FEM_VISUALIZATION )
    IF(FEM_VISUALIZATION)
    # Define the macro that includes all the required functions and include MFC support
    ADD_DEFINITIONS( -DFEM_BUILD_VISUALIZATION /D "_AFXDLL" )
    ENDIF(FEM_VISUALIZATION)
ENDIF(WIN32)


ADD_EXECUTABLE(FEMBrainStripValidationApp ${FEMBrainStripApp_SRCS} )
TARGET_LINK_LIBRARIES(FEMBrainStripValidationApp ITKBasicFilters ITKNumerics FEM ITKIO)



