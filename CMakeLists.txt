cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


add_subdirectory(src/ComponentSystems/)


file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/character/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKGraphics
    IDKGameEngine

    IDKBuiltinCS
    IDKcomponentsystems
)



FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/
)

FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/
)
