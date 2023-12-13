add_library(ObjData_lib ObjData.cpp include/ObjData/ObjData.hpp)
find_package(OpenCV REQUIRED)
set_property(TARGET ObjData_lib PROPERTY CXX_STANDARD 20)
target_include_directories(ObjData_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ObjData_lib PRIVATE opencv_core opencv_highgui)