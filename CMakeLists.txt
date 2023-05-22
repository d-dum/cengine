cmake_minimum_required(VERSION 3.24)
project(engine C)

find_package(PkgConfig REQUIRED)
add_subdirectory(lib/glfw)
pkg_check_modules(GLEW REQUIRED glew)
add_subdirectory(lib/cglm)

include(CheckLibraryExists)

CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

add_subdirectory(lib/fast_obj)
add_subdirectory(lib/SOIL2)

if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)


set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions")

file(GLOB_RECURSE ENGINE_SRC "src/*.c")
file(GLOB_RECURSE ENGINE_HEADER "src/*.h")

set(ENGINE_SRC ${ENGINE_SRC} ${ENGINE_HEADER})

add_executable(engine_test
    ${ENGINE_SRC}
    lib/fast_obj/fast_obj.c
    lib/fast_obj/fast_obj.h
    lib/cgltf/cgltf.h src/engine/Utils/Arr.c 
)
#target_include_directories(engine_test PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(engine_test PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(engine_test PRIVATE cglm)
target_include_directories(engine_test PUBLIC ${SOIL2_INCLUDE_DIRS})
target_link_libraries(engine_test PRIVATE glfw)
target_link_libraries(engine_test PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${CGLM_LIBRARIES})
target_link_libraries(engine_test PUBLIC ${PNG_LIBRARIES})

target_link_libraries(engine_test PUBLIC ${EXTRA_LIBS})
target_link_libraries(engine_test PRIVATE soil2)