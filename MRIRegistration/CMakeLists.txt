FIND_PATH(VTK_INCLUDE_PATH vtkLogo.jpg c:/lorensen/vtk /usr/local/vtk  )
FIND_PATH(VTK_BIN_PATH vtkConfigure.h c:/lorensen/vtkbin /usr/local/vtk  )
FIND_LIBRARY(VTK_LIBRARY vtkdll vtk c:/lorensen/vtkbin/Debug/vtkdll /vtkbin/Debug/vtkdll /usr/local/vtk )

INCLUDE_DIRECTORIES(
${VTK_INCLUDE_PATH}/common
${VTK_INCLUDE_PATH}/graphics
${VTK_INCLUDE_PATH}/imaging
${VTK_INCLUDE_PATH}/contrib
${VTK_BIN_PATH}
)

IF(UNIX)
LINK_LIBRARIES(
${VTK_LIB_PATH}/libVTKCommon.so
${VTK_LIB_PATH}/libVTKImaging.so
${VTK_LIBRARY}
VXLNumerics
ITKCommon
)
ENDIF(UNIX)

IF(WIN32)
LINK_LIBRARIES(
${VTK_LIBRARY}
VXLNumerics
ITKCommon
)
ENDIF(WIN32)

SOURCE_FILES(MRIRegistration_SRCS
OptionList
RegisterAD
vtkImageStatistics
)

ADD_EXECUTABLE(MRIRegistration MRIRegistration_SRCS)

