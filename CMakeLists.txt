cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


add_subdirectory(src/ComponentSystems/)


file(GLOB SOURCES
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    # ${CMAKE_CURRENT_SOURCE_DIR}/external/include/
)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKGraphics
    IDKGameEngine

    IDKcomponentsystems

    IDKBuiltin
)

