add_subdirectory(sprites)
add_subdirectory(glInterface)

target_include_directories(${PROJECT_NAME} PUBLIC sprites
                                                  glInterface)

target_sources(${PROJECT_NAME} PRIVATE Renderer.cpp)