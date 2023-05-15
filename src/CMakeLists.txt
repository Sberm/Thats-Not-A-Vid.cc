find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(encode encode.cpp)
add_library(decode decode.cpp)
add_library(cli cli.cpp)

target_link_libraries(cli
                      PRIVATE decode
                      PRIVATE encode
                      PRIVATE ${OpenCV_LIBS} 
                      PRIVATE ${Boost_LIBRARIES})