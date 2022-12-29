cmake_minimum_required(VERSION 3.24)
project(engine C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(CGLM REQUIRED cglm)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)


set(CMAKE_C_STANDARD 11)

add_executable(engine_test src/main.c src/engine/Core/DisplayManager.c src/engine/Core/DisplayManager.h src/engine/Core/Shader.c src/engine/Core/Shader.h src/engine/Utils/FileUtils.c src/engine/Utils/FileUtils.h src/engine/Core/ShaderProgram.c src/engine/Core/ShaderProgram.h src/engine/Core/MeshLoader.c src/engine/Core/MeshLoader.h src/engine/Core/Renderer.c src/engine/Core/Renderer.h src/engine/Utils/Mat.c src/engine/Utils/Mat.h src/engine/Utils/Mem.c src/engine/Utils/Mem.h)
target_include_directories(engine_test PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${CGLM_INCLUDE_DIRS})
target_link_libraries(engine_test PUBLIC ${GLFW3_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${CGLM_LIBRARIES})

target_link_libraries(engine_test PUBLIC ${EXTRA_LIBS})