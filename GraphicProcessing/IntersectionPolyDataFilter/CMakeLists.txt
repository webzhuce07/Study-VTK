
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(IntersectionPolyDataFilter)

find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
  message("Skipping IntersectionPolyDataFilter: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(IntersectionPolyDataFilter MACOSX_BUNDLE IntersectionPolyDataFilter.cxx )
  target_link_libraries(IntersectionPolyDataFilter PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(IntersectionPolyDataFilter MACOSX_BUNDLE IntersectionPolyDataFilter.cxx )
  target_link_libraries(IntersectionPolyDataFilter PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS IntersectionPolyDataFilter
    MODULES ${VTK_LIBRARIES}
    )
endif () 
