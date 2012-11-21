cmake_minimum_required(VERSION 2.8)
if(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

project( ImageViewer )
include( ${CMAKE_SOURCE_DIR}/Superbuild.cmake )
