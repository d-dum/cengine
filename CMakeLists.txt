cmake_minimum_required(VERSION 3.24)
project(engine C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)



set(CMAKE_C_STANDARD 11)

add_executable(engine_test src/main.c src/engine/Core/DisplayManager.c src/engine/Core/DisplayManager.h src/engine/Core/Shader.c src/engine/Core/Shader.h src/engine/Utils/FileUtils.c src/engine/Utils/FileUtils.h src/engine/Core/ShaderProgram.c src/engine/Core/ShaderProgram.h src/engine/Core/MeshLoader.c src/engine/Core/MeshLoader.h src/engine/Core/Renderer.c src/engine/Core/Renderer.h)
target_include_directories(engine_test PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(engine_test PUBLIC ${GLFW3_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${GLEW_LIBRARIES})