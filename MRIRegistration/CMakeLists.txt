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

LINK_LIBRARIES(
${VTK_LIBRARY}
VXLNumerics
ITKCommon
)

SOURCE_FILES(MRIRegistration_SRCS
OptionList
RegisterAD
vtkImageStatistics
)

SOURCE_FILES(RegisterADAffine_SRCS
OptionList
RegisterADAffine
vtkImageStatistics
)

ADD_EXECUTABLE(MRIRegistration MRIRegistration_SRCS)
ADD_EXECUTABLE(RegisterADAffine RegisterADAffine_SRCS)

