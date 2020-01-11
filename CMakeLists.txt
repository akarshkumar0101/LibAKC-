cmake_minimum_required(VERSION 3.13.4)
project(ak)

set(CMAKE_CXX_STANDARD 17)

set(LIB_ROOT /Users/akarshkumar0101/Developer/Library/CppLibraries/)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SRC
        ./include/*.h
        ./src/*.cpp
        )

add_library(ak STATIC
        ${SRC})
add_executable(aktest
        main.cpp)

target_include_directories(ak PUBLIC ./include/)

target_include_directories(ak PUBLIC ${LIB_ROOT}/Glad/glad/include/)
target_include_directories(ak PUBLIC ${LIB_ROOT}/GLM/glm/)
target_include_directories(ak PUBLIC ${LIB_ROOT}/ImGUI/)
target_include_directories(ak PUBLIC ${LIB_ROOT}/ImGUI/examples/)

target_include_directories(ak PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(ak glfw)
target_link_libraries(ak ${OPENGL_gl_LIBRARY})
target_link_libraries(ak ${OPENGL_glu_LIBRARY})

target_link_libraries(ak ${LIB_ROOT}/Glad/libGlad.a)
target_link_libraries(ak ${LIB_ROOT}/ImGUI/libImGUI.a)






target_include_directories(aktest PRIVATE ./include/)

target_include_directories(aktest PRIVATE ${LIB_ROOT}/Glad/glad/include/)
target_include_directories(aktest PRIVATE ${LIB_ROOT}/GLM/glm/)
target_include_directories(aktest PRIVATE ${LIB_ROOT}/ImGUI/)
target_include_directories(aktest PRIVATE ${LIB_ROOT}/ImGUI/examples/)

target_include_directories(aktest PUBLIC ${OPENGL_INCLUDE_DIR})


target_link_libraries(aktest ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/libak.a)
target_link_libraries(aktest glfw)
target_link_libraries(aktest ${OPENGL_gl_LIBRARY})
target_link_libraries(aktest ${OPENGL_glu_LIBRARY})

target_link_libraries(aktest ${LIB_ROOT}/Glad/libGlad.a)
target_link_libraries(aktest ${LIB_ROOT}/ImGUI/libImGUI.a)

