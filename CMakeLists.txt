cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(guilecpp VERSION 0.1 LANGUAGES CXX)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Include Boost as an imported target
find_package(Guile REQUIRED)
find_package(CallableTraits REQUIRED)

add_library(call INTERFACE IMPORTED)
set_property(TARGET call PROPERTY
                    INTERFACE_INCLUDE_DIRECTORIES ${CallableTraits_INCLUDE_DIR})

add_executable(guilecpp main.cpp)
set_target_properties(guilecpp PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
)
target_include_directories(guilecpp PRIVATE ${GUILE_INCLUDE_DIRS})
target_link_libraries(guilecpp ${GUILE_LIBRARIES} call)

add_library(module SHARED module.cpp)
set_target_properties(module PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
)
target_include_directories(module PRIVATE ${GUILE_INCLUDE_DIRS})
target_link_libraries(module ${GUILE_LIBRARIES})

