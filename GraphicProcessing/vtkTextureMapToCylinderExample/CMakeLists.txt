#**********************************************************************
#
#  Copyright (c) Mr.Bin. All rights reserved.
#  For more information visit: http://blog.csdn.net/webzhuce 
#
#**********************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(vtkTextureMapToCylinderExample)
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ADD_EXECUTABLE(vtkTextureMapToCylinderExample    main.cpp)
TARGET_LINK_LIBRARIES(vtkTextureMapToCylinderExample  ${VTK_LIBRARIES})