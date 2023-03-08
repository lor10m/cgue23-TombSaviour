# Always choose latest possible version if the project supports it without the need of too complex refactorings.
cmake_minimum_required(VERSION 3.22)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

# include CMake utility macros like cmake_print_variables()
include(CMakePrintHelpers)

# See 15.5. Advanced Linking Relationships
# Use this flag to require any linked libraries to be also targets which are defined by this repo (and not external library names)
# External libraries can still be linked, but this flag enhances error messages if some mistakes happen.
# FIXME set(CMAKE_LINK_LIBRARIES_ONLY_TARGETS TRUE)

get_property(isMultiConfig GLOBAL
    PROPERTY GENERATOR_IS_MULTI_CONFIG
)
if(isMultiConfig)
    message(FATAL_ERROR "Multi-config is not supported by this project. Use a non-multi-config generator instead.")
endif()


################################################################################
# Global configuration types
################################################################################
if(NOT "Debug" IN_LIST CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES Debug)
endif()
if(NOT "Release" IN_LIST CMAKE_CONFIGURATION_TYPES)
    list(APPEND CMAKE_CONFIGURATION_TYPES Release)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS "${CMAKE_CONFIGURATION_TYPES}"
)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
elseif(NOT CMAKE_BUILD_TYPE IN_LIST CMAKE_CONFIGURATION_TYPES)
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

# DEBUG_CONFIGURATIONS should not be set. Instead, generator expressions should be used.

# TODO: maybe those methods can be overwritten by custom guards which throw an error instead, and put those guards into
#  a separate Util.cmake file?
# include_directories should not be used because they are discouraged.      Use target_include_directories instead.
# add_definitions:                                                          use add_compile_definitions instead.
# add_dependencies() and link_libraries():                                  use target_link_libraries instead.
# link_directories:                                                         use target_link_directories instead.
# get_filename_component() and file():                                      use cmake_path() instead.

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} architecture is not supported!")
endif()

################################################################################
# Global compiler options
################################################################################
# See 16.1. Setting The Language Standard Directly
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS        FALSE)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)