cmake_minimum_required(VERSION 2.8)

project (cvProj)

find_package( OpenCV REQUIRED)

add_executable(cvProj main.cpp)

target_link_libraries(cvProj ${OpenCV_LIBS})
