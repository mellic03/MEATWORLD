cmake_minimum_required(VERSION 3.9)

project(ImGui_190)

file(GLOB SOURCES
    "./*.cpp"
)

add_library(${PROJECT_NAME} OBJECT ${SOURCES})
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/imgui-1.90
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
)

