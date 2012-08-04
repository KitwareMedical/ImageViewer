#---------------------------------------------------------------------------
# Get and build itk

set( ITK_TAG "v4.2.0" )
set(ITK_TAG "2a7eed39f0a26f5eb9d0d9ec413938e9f0b9995c") # July 17, 2012
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
  DEPENDS VTK
)

set(ITK_DIR ${CMAKE_BINARY_DIR}/ITK-build)
