add_subdirectory(spawner)
add_subdirectory(particle)

target_include_directories(${PROJECT_NAME} PUBLIC spawner particle)