FIND_PATH(VTK_INCLUDE_PATH vtkLogo.jpg c:/vtk /usr/local/vtk  )
FIND_PATH(VTK_BIN_PATH vtkConfigure.h c:/vtkbin /usr/local/vtk  )
FIND_LIBRARY(VTK_LIBRARY vtkdll vtk c:/vtkbin/vtkdll /vtkbin/vtkdll /usr/local/vtk )

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
)

ADD_EXECUTABLE(MRIRegistration MRIRegistration_SRCS)

