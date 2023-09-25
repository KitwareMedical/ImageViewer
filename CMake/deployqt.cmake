# Copied from https://stackoverflow.com/questions/60854495/qt5-cmake-include-all-libraries-into-executable

find_package(Qt5Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

if( WIN32 )
  function(deployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND "${_qt_bin_dir}/windeployqt.exe"         
              --verbose 1
              --translations en
              --release
              --no-svg
              --no-angle
              --no-system-d3d-compiler
              \"$<TARGET_FILE:${target}>\"
      COMMENT "Deploying using windeployqt for target '${target}' ..."
    )
  endfunction()
elseif( APPLE )
  function(deployqt target)
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND "${_qt_bin_dir}/macdeployqt"         
              \"$<TARGET_FILE_DIR:${target}>/../..\"
      COMMENT "Deploying using macdeployqt for target '${target}' ..."
    )
    add_custom_command(TARGET ${target} POST_BUILD
      COMMAND "chmod"
                 +x
                 \"$<TARGET_FILE:${target}>\"
      COMMENT "Ensuring ImageViewer application is executable."
    )
  endfunction()
else()
  function(deployqt target)
    message(STATUS "DeployQt for linux = NOP")
  endfunction()
endif()
