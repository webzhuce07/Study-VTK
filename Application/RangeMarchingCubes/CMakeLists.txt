CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(VolumeRendering)
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ADD_EXECUTABLE(VolumeRendering VolumeRendering.cpp)
TARGET_LINK_LIBRARIES(VolumeRendering ${VTK_LIBRARIES})
