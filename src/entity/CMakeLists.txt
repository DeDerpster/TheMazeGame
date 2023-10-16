add_subdirectory(movableEntity)
add_subdirectory(objects)
target_include_directories(${PROJECT_NAME} PUBLIC movableEntity)
target_include_directories(${PROJECT_NAME} PUBLIC objects)

target_sources(${PROJECT_NAME} PRIVATE Entity.cpp)
