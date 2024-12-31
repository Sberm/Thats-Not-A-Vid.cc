cmake_minimum_required (VERSION 2.8.11)
project (Thats-Not-A-Vid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable (Thats-Not-A-Vid src/main.cpp)

target_link_libraries(Thats-Not-A-Vid cli)
