cmake_minimum_required(VERSION 3.9)

project(IDKcomponentsystems)


file(GLOB SOURCES
    "./*.cpp"
    "./playercontroller_cs/*.cpp"
    "./terrain_cs/*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 17
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/include/
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKRenderEngine
    IDKGameEngine
)

