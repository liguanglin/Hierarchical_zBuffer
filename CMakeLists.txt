cmake_minimum_required(VERSION 3.10.2)

project(cg_hierarchical_zbuffer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/utils
)

add_library(${PROJECT_NAME} SHARED
utils/model.cpp
utils/OctTree.cpp
utils/zBuffer.cpp
utils/Type.cpp
utils/Display.cpp
)

add_executable(main main.cpp)

target_link_libraries(main
${PROJECT_NAME}
${OPENGL_LIBRARIES}
${GLUT_LIBRARIES}
)
