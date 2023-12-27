cmake_minimum_required(VERSION 3.9)

project(IDKmodules)


file(GLOB SOURCES
    "./EditorUI/*.cpp"
    "./EditorUI/menubar/*.cpp"
    "./EditorUI/tabs/*.cpp"
    "./EditorUI/common/*.cpp"
    "./EditorUI/common/imgui-1.89.7/*.cpp"
    "./voxel/*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

