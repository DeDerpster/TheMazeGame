add_subdirectory(sprites)
add_subdirectory(text)
add_subdirectory(glInterface)

target_include_directories(${PROJECT_NAME} PUBLIC sprites
                                                  glInterface
                                                  text
                                                  effect)

target_sources(${PROJECT_NAME} PRIVATE Renderer.cpp
                                       SmartBuffer.cpp)