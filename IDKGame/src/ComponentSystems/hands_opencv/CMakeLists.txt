cmake_minimum_required(VERSION 3.9)

project(hands_opencv)


file(GLOB SOURCES
    "./*.cpp"
)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/${idkengine_source_dir})

find_package( OpenCV REQUIRED )
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS} )


target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKcommon
    IDKgraphics
    ${OpenCV_LIBS}
)
