#---------------------------------------------------------------------------
# Get and build itk

set (itk_depends)

if (VTK_EXTERNAL_PROJECT)
  set( itk_depends VTK ) # because of ITKVtkGlue
endif()

set( ITK_TAG "v4.3.0" )
ExternalProject_Add( ITK
  GIT_REPOSITORY "${git_protocol}://itk.org/ITK.git"
  GIT_TAG "${ITK_TAG}"
  SOURCE_DIR ITK
  BINARY_DIR ITK-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DITK_BUILD_ALL_MODULES:BOOL=ON
    -DITK_LEGACY_REMOVE:BOOL=OFF
    -DITKV3_COMPATIBILITY:BOOL=ON
    -DVTK_DIR:PATH=${VTK_DIR}
    -DModule_ITKVtkGlue:BOOL=ON
  INSTALL_COMMAND ""
  DEPENDS ${itk_depends}
)

set(ITK_DIR ${CMAKE_BINARY_DIR}/ITK-build)
set(ITK_EXTERNAL_PROJECT "TRUE")
