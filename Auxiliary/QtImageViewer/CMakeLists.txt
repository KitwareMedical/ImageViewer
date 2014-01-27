Project( QtSlicer )

INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)

IF(USE_ITK_FILE)
   INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

# 
# Find QT
#
INCLUDE (${CMAKE_ROOT}/Modules/FindQt.cmake)
INCLUDE_DIRECTORIES(
 ${QT_INCLUDE_PATH}
 ${QT_INCLUDE_DIR}
 ${QtSlicer_BINARY_DIR}
 ${QtSlicer_SOURCE_DIR}
)

find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

#LINK_LIBRARIES(
 #${ITK_LIBRARIES}
 #${QT_LIBRARIES}
 #${GLU_LIBRARIES}
 #${OPENGL_LIBRARIES}
#)

SET(QtSlicer_SRCS 
QtSlicerTest.cxx
QtGlSliceView.cxx
QtSlicer.cxx
)

SET(QtSlicer_GUI_SRCS 
QtSlicerGUI.ui
QtSlicerHelpGUI.ui
)


SET(QtSlicer_MOC_SRCS 
QtGlSliceView.h
QtSlicer.h
)

QT4_WRAP_UI( QtSlicer_UI_CXX ${QtSlicer_GUI_SRCS} )
QT4_WRAP_CPP( QtSlicer_SRCS ${QtSlicer_MOC_SRCS} )

ADD_DEFINITIONS(-DQT_DLL)

ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)


ADD_EXECUTABLE(QtSlicer ${QtSlicer_SRCS} ${QtSlicer_GUI_SRCS} ${QtSlicer_UI_CXX} )

install( TARGETS QtSlicer DESTINATION bin )
set( QtSlicer_LIBS  ${ITK_LIBRARIES} ) # ${VTK_LIBRARIES} )
if( USE_FLTK )
  list( APPEND QtSlicer_LIBS ${FLTK_LIBRARIES} FltkImageViewer )
else()
  list( APPEND QtSlicer_LIBS ${QT_LIBRARIES})
  list( APPEND QtSlicer_LIBS ${GLU_LIBRARY})
  list( APPEND QtSlicer_LIBS ${OPENGL_LIBRARIES})
endif()
target_link_libraries( QtSlicer ${QtSlicer_LIBS})





