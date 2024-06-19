cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_image_INCLUDE_DIRS})
include_directories(${SDL2_mixer_INCLUDE_DIRS})
include_directories(${SDL2_ttf_INCLUDE_DIRS})


project(meatnet)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})



target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    GLEW
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    SDL2_net
    ${OPENGL_LIBRARIES}

    libidk
    IDKGraphics

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKThreading
    IDKECS
)

