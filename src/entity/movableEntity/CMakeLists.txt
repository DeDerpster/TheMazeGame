target_sources(${PROJECT_NAME} PRIVATE MovableEntity.cpp)

add_subdirectory(mob)
add_subdirectory(projectile)
target_include_directories(${PROJECT_NAME} PUBLIC mob)
target_include_directories(${PROJECT_NAME} PUBLIC projectile)