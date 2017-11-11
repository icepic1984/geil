cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(guilecpp VERSION 0.1 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Include Boost as an imported target
find_package(Guile REQUIRED)
add_executable(guilecpp main.cpp)
target_include_directories(guilecpp PRIVATE ${GUILE_INCLUDE_DIRS})
target_link_libraries(guilecpp ${GUILE_LIBRARIES})


