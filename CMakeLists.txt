cmake_minimum_required(VERSION 3.18)
project(MazeGame VERSION 0.1.0)

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Release mode")
    set(CURRENT_MODE RELEASE)
else()
    message(STATUS "Debug mode")
    set(CURRENT_MODE DEBUG)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME} - ${CURRENT_MODE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME} - ${CURRENT_MODE}")

set(CMAKE_CXX_STANDARD 17) # This is because windows doesn't support c++20 fully yet - quite buggy
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CURRENT_MODE MATCHES "DEBUG")
    set(CMAKE_CXX_FLAGS "/DEBUG[:FULL]")
endif()


include(CTest)
enable_testing()

message(STATUS "Running on ${CMAKE_SYSTEM_NAME}")

add_subdirectory(src)

find_package (Threads)
if(Threads_FOUND)
    target_link_libraries (${PROJECT_NAME} PUBLIC Threads::Threads)
else()
    target_link_libraries(${PROJECt_NAME} PUBLIC dependencies/pthreadWin32)
endif()

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
endif()

# add_subdirectory(dependencies/FreeType)
if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(${FREETYPE_INCLUDE_DIRS} dependencies/FreeType/include)
    add_subdirectory(dependencies/freetype)
    target_link_libraries(${PROJECT_NAME} PUBLIC freetype)
else()
find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ${FREETYPE_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE dependencies/FreeType/include)
endif()


add_subdirectory(dependencies/glfw)
include_directories(dependencies/glfw/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

target_include_directories(${PROJECT_NAME} PRIVATE dependencies/glew/include)

# pre-processor definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC GLEW_STATIC)

if(CURRENT_MODE MATCHES DEBUG)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_compile_definitions(${PROJECT_NAME} PUBLIC IS_ON_LINUX)
    add_library(glew STATIC IMPORTED GLOBAL)

    set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/glew/lib/linux/libGLEW.a )
    set_target_properties(glew PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/dependencies/glew/include )

    target_link_libraries(${PROJECT_NAME} PUBLIC glew)

else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC IS_ON_WINDOWS)

    add_library(glew STATIC IMPORTED GLOBAL)

    set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/dependencies/glew/lib/windows/glew32s.lib )
    set_target_properties(glew PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/dependencies/glew/include )

    target_link_libraries(${PROJECT_NAME} PUBLIC glew)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
                            ${PROJECT_BINARY_DIR}
                            src)

include(CPack)
