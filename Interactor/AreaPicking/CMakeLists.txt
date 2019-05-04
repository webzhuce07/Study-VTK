
cmake_minimum_required(VERSION 2.8)
 
PROJECT(AreaPicking)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(AreaPicking MACOSX_BUNDLE AreaPicking.cxx )
 
target_link_libraries(AreaPicking ${VTK_LIBRARIES})
