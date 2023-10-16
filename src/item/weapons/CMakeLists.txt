add_subdirectory(staffs)
add_subdirectory(general)
target_include_directories(${PROJECT_NAME} PUBLIC staffs general)
target_sources(${PROJECT_NAME} PRIVATE Weapon.cpp)