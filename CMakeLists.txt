cmake_minimum_required(VERSION 3.15)
project(GLutils)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/extlib)
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/extlib/glad/glad.c)

file(GLOB_RECURSE UTIL_SRC "src/glutils/*.cpp" "src/glutils/*.c" "src/glutils/*.h" "src/glutils/*.hpp")
set(CURRENT_TEST_SRC "src/test_mains/test_idea/MultipassRendering.cpp")

add_executable(GLutils ${CURRENT_TEST_SRC} ${UTIL_SRC} ${GLAD_SRC})
target_link_libraries(GLutils glfw3)

add_custom_target(COPY_SHADERS
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
        )

add_dependencies(GLutils COPY_SHADERS)