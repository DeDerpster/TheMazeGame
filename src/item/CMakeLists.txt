add_subdirectory(weapon)
target_include_directories(${PROJECT_NAME} PUBLIC weapon)

target_sources(${PROJECT_NAME} PRIVATE Item.cpp)