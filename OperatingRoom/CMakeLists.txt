INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/OperatingRoom
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Examples/OperatingRoom
)

SOURCE_FILES(OperatingRoom_SRCS
liC_Arm3D
liCommandGLRedrawModel
liCommandGLRedrawModelFixed
liCommandGLRedrawModelSensitive
liCommandUpdateImageIntensifier
liCommandUpdateSpineModelGUI
liCommandUpdateRegistratorFieldOfView
liCommandTubesGeneratePointSet
liFluoroscopyCArm
liFluoroscopyCArmMobile
liFluoroscopyCArmMobileGUI
liFluoroscopyUnitView
liFluoroscopyUnitViewGUI
liLight
liOperatingRoomModel
liOperatingRoomView
liOperatingRoomViewGUI
liPatient
liPointSet3D
liSampleGenerator
liSampleGeneratorBase
liSampleGeneratorBodyPlane
liSampleGeneratorBodyPlaneGUI
liSampleGeneratorCollection
liSampleGeneratorCollectionBase
liSampleGeneratorCollectionGUI
liSampleGeneratorGUI
liSampleGeneratorPedicle
liSampleGeneratorPedicleGUI
liSpineInterventionConfiguration
liSpineInterventionConfigurationGUI
liSpineModel
liSpineModelGUI
liSurgeryTable
liSurgeryTableGUI
liVectorTubes3D
liVertebralBody
liVertebralPedicle
liVertebralSampleGenerator
liVertebralSegment
liVertebralSegmentGUI
liVertebralSegmentRegistrator
liVesselsModelGUI
liVesselsRegistrator
liTube3D
main
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
MetaImageIO
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
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(OperatingRoom OperatingRoom_SRCS)

