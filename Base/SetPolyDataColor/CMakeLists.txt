CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SetPolyDataColor)
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ADD_EXECUTABLE(SetPolyDataColor main.cpp)
TARGET_LINK_LIBRARIES(SetPolyDataColor ${VTK_LIBRARIES})

