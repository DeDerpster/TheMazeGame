add_subdirectory(mob)
add_subdirectory(item)
target_include_directories(${PROJECT_NAME} PUBLIC mob)
target_include_directories(${PROJECT_NAME} PUBLIC item)