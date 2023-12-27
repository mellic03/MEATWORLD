cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-g" "-fno-ident" "-fno-asynchronous-unwind-tables")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Debugging
# --------------------------------------------
add_definitions(-DIDK_DEBUG)
# --------------------------------------------
project(game)



include_directories(${CMAKE_SOURCE_DIR}/external/include/)
include_directories(${CMAKE_SOURCE_DIR}/external/include/libidk/IDKcontainers)
include_directories(${CMAKE_SOURCE_DIR}/IDKGame/src/)
include_directories(${CMAKE_SOURCE_DIR}/../IDKGameEngine/build/include)



add_subdirectory(external/sources/imgui-1.90/)
add_subdirectory(external/sources/imgui-docking/)
add_subdirectory(IDKGame/src/ComponentSystems/)
add_subdirectory(IDKGame/src/Modules/)


set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_image_INCLUDE_DIRS})
include_directories(${SDL2_mixer_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})


add_executable(${PROJECT_NAME} "IDKGame/src/main.cpp")

# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath=./../../IDKGameEngine/build.lib/,-rpath=IDKGE/shipping/")

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    GLEW
    SDL2
    SDL2_image
    SDL2_mixer
    ${OPENGL_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/../IDKGameEngine/build/lib/libIDKengine.so
    # ImGui_190
    ImGui_docking
    IDKcomponentsystems
    IDKmodules
)

