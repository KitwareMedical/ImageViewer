INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/OperatingRoom
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/OperatingRoom
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/OperatingRoom
)

SOURCE_FILES(OperatingRoom_SRCS
liC_Arm3D.cxx
liCommandGLRedrawModelFixed.cxx
liCommandGLRedrawModelSensitive.cxx
liCommandUpdateImageIntensifier.cxx
liCommandUpdateRegistratorFieldOfView.cxx
liCommandUpdateSpineModelGUI.cxx
liCommandTubesGeneratePointSet.cxx
liFluoroscopyCArm.cxx
liFluoroscopyCArmMobile.cxx
liFluoroscopyUnitView.cxx
liLight.cxx
liOperatingRoomModel.cxx
liOperatingRoomView.cxx
liPatient.cxx
liPointSet3D.cxx
liSampleGenerator.cxx
liSampleGeneratorBase.cxx
liSampleGeneratorBodyPlane.cxx
liSampleGeneratorCollection.cxx
liSampleGeneratorCollectionBase.cxx
liSampleGeneratorPedicle.cxx
liSpineInterventionConfiguration.cxx
liSpineModel.cxx
liSurgeryTable.cxx
liVectorTubes3D.cxx
liVertebralBody.cxx
liVertebralPedicle.cxx
liVertebralSampleGenerator.cxx
liVertebralSegment.cxx
liVertebralSegmentRegistrator.cxx
liVesselsRegistrator.cxx
liTube3D.cxx
main.cxx
)

SOURCE_FILES(OperatingRoom_GUI_SRCS
liVesselsModelGUI.fl
liVertebralSegmentGUI.fl
liSurgeryTableGUI.fl
liSpineModelGUI.fl
liSpineInterventionConfigurationGUI.fl
liSampleGeneratorPedicleGUI.fl
liSampleGeneratorGUI.fl
liSampleGeneratorCollectionGUI.fl
liSampleGeneratorBodyPlaneGUI.fl
liOperatingRoomViewGUI.fl
liFluoroscopyUnitViewGUI.fl
liFluoroscopyCArmMobileGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO 
MetaIO
VXLNumerics
FltkImageViewer
${FLTK_LIBRARY}
${GLU_LIBRARY}
${OPENGL_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  ${CMAKE_X_LIBS} -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(OperatingRoom OperatingRoom_SRCS)

FLTK_WRAP_UI(OperatingRoom OperatingRoom_GUI_SRCS)

