CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ReportProgressFilterTest)
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ADD_EXECUTABLE(ReportProgressFilterTest	ReportProgressFilterTest.cpp
                                        vtkReportProgressFilter.h 
                                         vtkReportProgressFilter.cpp)
TARGET_LINK_LIBRARIES(ReportProgressFilterTest ${VTK_LIBRARIES})
