add_executable(${PROJECT_NAME} MazeGame.cpp
                               RandomGen.cpp
                               Log.cpp
                               Camera.cpp
                               Application.cpp
                               Event.cpp)

target_precompile_headers(${PROJECT_NAME} PUBLIC
                          headers/Core.h
                          headers/glfwGlew.h
                          headers/GLM.h
                          headers/ImGui.h
                          headers/LogHeaders.h
                          headers/MazeHeaders.h)


add_subdirectory(maze)
add_subdirectory(rendering)
add_subdirectory(vendor/imgui)
add_subdirectory(layers)
add_subdirectory(entity)
add_subdirectory(levels)

target_include_directories(${PROJECT_NAME} PUBLIC
                            maze
                            rendering
                            headers
                            vendor
                            entity
                            layers
                            levels)