cmake_minimum_required(VERSION 3.24)
project(engine C)

find_package(PkgConfig REQUIRED)
add_subdirectory(lib/glfw)
pkg_check_modules(GLEW REQUIRED glew)
add_subdirectory(lib/cglm)
add_subdirectory(lib/libpng)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

add_subdirectory(lib/fast_obj)
add_subdirectory(lib/SOIL2)

if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)


set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions")

add_executable(engine_test src/main.c src/engine/Core/DisplayManager.c src/engine/Core/DisplayManager.h src/engine/Core/Shader.c src/engine/Core/Shader.h src/engine/Utils/FileUtils.c src/engine/Utils/FileUtils.h src/engine/Core/ShaderProgram.c src/engine/Core/ShaderProgram.h src/engine/Core/MeshLoader.c src/engine/Core/MeshLoader.h src/engine/Core/Renderer.c src/engine/Core/Renderer.h src/engine/Utils/Mat.c src/engine/Utils/Mat.h src/engine/Utils/Mem.c src/engine/Utils/Mem.h src/engine/GameObject/Entity.c src/engine/GameObject/Entity.h src/engine/GameObject/Camera.c src/engine/GameObject/Camera.h src/engine/Utils/Misc.h src/engine/Utils/Primitives.h src/engine/engine.h lib/fast_obj/fast_obj.c lib/fast_obj/fast_obj.h)
target_include_directories(engine_test PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${CGLM_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${PNG_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${SOIL2_INCLUDE_DIRS})
target_link_libraries(engine_test PUBLIC ${GLFW3_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${CGLM_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${PNG_LIBRARIES})

target_link_libraries(engine_test PUBLIC ${EXTRA_LIBS})
target_link_libraries(engine_test PUBLIC ${SOIL2_LIBRARIES})