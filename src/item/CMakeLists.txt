add_subdirectory(weapons)
add_subdirectory(potions)
target_include_directories(${PROJECT_NAME} PUBLIC weapons)
target_include_directories(${PROJECT_NAME} PUBLIC potions)

target_sources(${PROJECT_NAME} PRIVATE Item.cpp)