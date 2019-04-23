cmake_minimum_required(VERSION 2.8)

project(TestSPHInterpolator2D)
 
find_package(VTK REQUIRED)
find_package( ITK REQUIRED )
  
include(${VTK_USE_FILE} )
INCLUDE( ${ITK_USE_FILE} )
 
add_executable(TestSPHInterpolator2D TestSPHInterpolator2D.cxx)
 
target_link_libraries(TestSPHInterpolator2D  ${VTK_LIBRARIES} ${ITK_LIBRARIES})