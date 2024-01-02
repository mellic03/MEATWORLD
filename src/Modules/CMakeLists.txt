cmake_minimum_required(VERSION 3.9)

project(IDKmodules)


file(GLOB SOURCES
    "./EditorUI/*.cpp"
    "./EditorUI/menubar/*.cpp"
    "./EditorUI/tabs/*.cpp"
    "./EditorUI/common/*.cpp"
    "./voxel/*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/include/
)


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

