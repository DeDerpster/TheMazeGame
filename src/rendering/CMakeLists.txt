add_subdirectory(sprites)
add_subdirectory(glInterface)
add_subdirectory(effect)

target_include_directories(${PROJECT_NAME} PUBLIC sprites
                                                  glInterface
                                                  effect)

target_sources(${PROJECT_NAME} PRIVATE Renderer.cpp)