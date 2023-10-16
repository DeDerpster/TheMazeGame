add_subdirectory(rooms)

target_include_directories(${PROJECT_NAME} PUBLIC rooms)

target_sources(${PROJECT_NAME} PRIVATE Tile.cpp)