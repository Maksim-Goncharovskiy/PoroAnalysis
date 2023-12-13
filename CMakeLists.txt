cmake_minimum_required(VERSION 3.14)
include_directories(.)

add_subdirectory(ObjData)

find_package(OpenCV REQUIRED)
find_package(doctest REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main main.cpp)
add_executable(checking checking.cpp)

target_link_libraries(main PRIVATE opencv_core opencv_highgui ObjData_lib doctest::doctest nlohmann_json::nlohmann_json)
target_link_libraries(checking PRIVATE opencv_core opencv_highgui ObjData_lib doctest::doctest)

