add_subdirectory(staffs)
target_include_directories(${PROJECT_NAME} PUBLIC staffs)
target_sources(${PROJECT_NAME} PRIVATE Weapon.cpp)