cmake_minimum_required(VERSION 3.9)

project(IDKmodules)


file(GLOB SOURCES
    "./imgui/*.cpp"
    "./imgui/imgui-1.89.7/*.cpp"
    "./voxel/*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

