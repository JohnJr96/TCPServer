cmake_minimum_required(VERSION 3.10)
project(KeyValueServerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TCPserver
    main.cpp
    src/KeyValueServer.cpp
)

# Include directories
target_include_directories(TCPserver PRIVATE src)

# Link libraries
target_link_libraries(TCPserver PRIVATE pthread)


add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/TCPserver
    COMMENT "Cleaning all build artifacts"
)