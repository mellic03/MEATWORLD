cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})


project(meatnet)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})



if (IDK_DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -Werror -fsanitize=undefined -lstdc++fs -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Ofast)
endif ()


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

