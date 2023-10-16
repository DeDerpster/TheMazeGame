add_subdirectory(level)

target_sources(${PROJECT_NAME} PRIVATE Layer.cpp
                                       MessageManager.cpp
                                       GUILayer.cpp
                                       GUIStack.cpp)