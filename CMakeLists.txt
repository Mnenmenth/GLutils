cmake_minimum_required(VERSION 3.15)
project(GLutils)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/extlib)
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/extlib/glad/glad.c)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c" "src/*.h" "src/*.hpp")

add_executable(GLutils ${SRC} ${GLAD_SRC})
target_link_libraries(GLutils glfw3)