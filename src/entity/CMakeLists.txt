add_subdirectory(movableEntity)
add_subdirectory(item)
target_include_directories(${PROJECT_NAME} PUBLIC movableEntity)
target_include_directories(${PROJECT_NAME} PUBLIC item)

target_sources(${PROJECT_NAME} PRIVATE Entity.cpp)
