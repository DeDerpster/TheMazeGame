add_subdirectory(sprite)
add_subdirectory(glInterface)

target_sources(${PROJECT_NAME} PRIVATE Renderer.cpp RenderObject.cpp)