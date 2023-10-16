add_subdirectory(stats)

target_sources(${PROJECT_NAME} PRIVATE Mob.cpp
                                       Player.cpp
                                       NPC.cpp)