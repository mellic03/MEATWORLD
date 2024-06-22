cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/meatnet/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../meatnet/src/meatnet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../meatnet/src/meatnet_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../meatnet/src/meatnet_server.cpp
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if (IDK_DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -Werror -fsanitize=undefined -lstdc++fs -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Ofast)
endif ()




target_include_directories(
    ${PROJECT_NAME} PUBLIC
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKGraphics

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKThreading
    IDKECS

    ImGui
    IDKBuiltinUI

    SDL2_net
)



FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/
)

FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/
)

