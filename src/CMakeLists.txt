add_executable(${PROJECT_NAME} MazeGame.cpp
                               Application.cpp)

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
add_subdirectory(layer)
add_subdirectory(entity)
add_subdirectory(level)
add_subdirectory(utils)
add_subdirectory(item)
add_subdirectory(gui)
add_subdirectory(particle)
add_subdirectory(event)

target_include_directories(${PROJECT_NAME} PUBLIC
                            headers
                            vendor
                            utils)