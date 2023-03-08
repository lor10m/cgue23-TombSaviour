cmake_minimum_required(VERSION 3.22)

find_package(glfw3 3.3 REQUIRED)

find_path(glslang_INCLUDE_DIR NAMES glslang/build_info.h HINTS "$ENV{VULKAN_SDK}/include")
find_library(glslang_LIBRARY            NAMES glslang            HINTS "$ENV{VULKAN_SDK}/lib")
find_library(GenericCodeGen_LIBRARY     NAMES GenericCodeGen     HINTS "$ENV{VULKAN_SDK}/lib")
find_library(MachineIndependent_LIBRARY NAMES MachineIndependent HINTS "$ENV{VULKAN_SDK}/lib")
find_library(OGLCompiler_LIBRARY        NAMES OGLCompiler        HINTS "$ENV{VULKAN_SDK}/lib")
find_library(OSDependent_LIBRARY        NAMES OSDependent        HINTS "$ENV{VULKAN_SDK}/lib")
find_library(SPIRV_LIBRARY              NAMES SPIRV              HINTS "$ENV{VULKAN_SDK}/lib")
find_library(SPIRV-Tools-static_LIBRARY NAMES SPIRV-Tools        HINTS "$ENV{VULKAN_SDK}/lib")
find_library(SPIRV-Tools-opt_LIBRARY    NAMES SPIRV-Tools-opt    HINTS "$ENV{VULKAN_SDK}/lib")
find_library(SPIRV-Tools-link_LIBRARY   NAMES SPIRV-Tools-link   HINTS "$ENV{VULKAN_SDK}/lib")
find_library(SPIRV-Tools-reduce_LIBRARY NAMES SPIRV-Tools-reduce HINTS "$ENV{VULKAN_SDK}/lib")

set(glslang_INCLUDE_DIRS         ${glslang_INCLUDE_DIR})
set(glslang_LIBRARIES            ${glslang_LIBRARY})
set(GenericCodeGen_LIBRARIES     ${GenericCodeGen_LIBRARY})
set(MachineIndependent_LIBRARIES ${MachineIndependent_LIBRARY})
set(OGLCompiler_LIBRARIES        ${OGLCompiler_LIBRARY})
set(OSDependent_LIBRARIES        ${OSDependent_LIBRARY})
set(SPIRV_LIBRARIES              ${SPIRV_LIBRARY})
set(SPIRV-Tools-static_LIBRARIES ${SPIRV-Tools-static_LIBRARY})
set(SPIRV-Tools-opt_LIBRARIES    ${SPIRV-Tools-opt_LIBRARY})
set(SPIRV-Tools-link_LIBRARIES   ${SPIRV-Tools-link_LIBRARY})
set(SPIRV-Tools-reduce_LIBRARIES ${SPIRV-Tools-reduce_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(glslang            DEFAULT_MSG glslang_LIBRARY            glslang_INCLUDE_DIR)
find_package_handle_standard_args(GenericCodeGen     DEFAULT_MSG GenericCodeGen_LIBRARY     glslang_INCLUDE_DIR)
find_package_handle_standard_args(MachineIndependent DEFAULT_MSG MachineIndependent_LIBRARY glslang_INCLUDE_DIR)
find_package_handle_standard_args(OGLCompiler        DEFAULT_MSG OGLCompiler_LIBRARY        glslang_INCLUDE_DIR)
find_package_handle_standard_args(OSDependent        DEFAULT_MSG OSDependent_LIBRARY        glslang_INCLUDE_DIR)
find_package_handle_standard_args(SPIRV              DEFAULT_MSG SPIRV_LIBRARY              glslang_INCLUDE_DIR)
find_package_handle_standard_args(SPIRV-Tools-static DEFAULT_MSG SPIRV-Tools-static_LIBRARY glslang_INCLUDE_DIR)
find_package_handle_standard_args(SPIRV-Tools-opt    DEFAULT_MSG SPIRV-Tools-static_LIBRARY glslang_INCLUDE_DIR)
find_package_handle_standard_args(SPIRV-Tools-link   DEFAULT_MSG SPIRV-Tools-static_LIBRARY glslang_INCLUDE_DIR)
find_package_handle_standard_args(SPIRV-Tools-reduce DEFAULT_MSG SPIRV-Tools-static_LIBRARY glslang_INCLUDE_DIR)

mark_as_advanced(glslang_INCLUDE_DIR glslang_LIBRARY)

if(glslang_FOUND AND NOT TARGET glslang)
    add_library(glslang UNKNOWN IMPORTED)
    set_target_properties(glslang PROPERTIES
            IMPORTED_LOCATION "${glslang_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(GenericCodeGen_FOUND AND NOT TARGET GenericCodeGen)
    add_library(GenericCodeGen UNKNOWN IMPORTED)
    set_target_properties(GenericCodeGen PROPERTIES
            IMPORTED_LOCATION "${GenericCodeGen_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(MachineIndependent_FOUND AND NOT TARGET MachineIndependent)
    add_library(MachineIndependent UNKNOWN IMPORTED)
    set_target_properties(MachineIndependent PROPERTIES
            IMPORTED_LOCATION "${MachineIndependent_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(OGLCompiler_FOUND AND NOT TARGET OGLCompiler)
    add_library(OGLCompiler UNKNOWN IMPORTED)
    set_target_properties(OGLCompiler PROPERTIES
            IMPORTED_LOCATION "${OGLCompiler_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(OSDependent_FOUND AND NOT TARGET OSDependent)
    add_library(OSDependent UNKNOWN IMPORTED)
    set_target_properties(OSDependent PROPERTIES
            IMPORTED_LOCATION "${OSDependent_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(SPIRV_FOUND AND NOT TARGET SPIRV)
    add_library(SPIRV UNKNOWN IMPORTED)
    set_target_properties(SPIRV PROPERTIES
            IMPORTED_LOCATION "${SPIRV_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(SPIRV-Tools-static_FOUND AND NOT TARGET SPIRV-Tools-static)
    add_library(SPIRV-Tools-static UNKNOWN IMPORTED)
    set_target_properties(SPIRV-Tools-static PROPERTIES
            IMPORTED_LOCATION "${SPIRV-Tools-static_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(SPIRV-Tools-opt_FOUND AND NOT TARGET SPIRV-Tools-opt)
    add_library(SPIRV-Tools-opt UNKNOWN IMPORTED)
    set_target_properties(SPIRV-Tools-opt PROPERTIES
            IMPORTED_LOCATION "${SPIRV-Tools-opt_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(SPIRV-Tools-link_FOUND AND NOT TARGET SPIRV-Tools-link)
    add_library(SPIRV-Tools-link UNKNOWN IMPORTED)
    set_target_properties(SPIRV-Tools-link PROPERTIES
            IMPORTED_LOCATION "${SPIRV-Tools-link_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if(SPIRV-Tools-reduce_FOUND AND NOT TARGET SPIRV-Tools-reduce)
    add_library(SPIRV-Tools-reduce UNKNOWN IMPORTED)
    set_target_properties(SPIRV-Tools-reduce PROPERTIES
            IMPORTED_LOCATION "${SPIRV-Tools-reduce_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${glslang_INCLUDE_DIRS}")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    find_library(LIBRT rt)
    if(LIBRT)
        target_link_libraries(SPIRV-Tools-static INTERFACE ${LIBRT})
    endif()
endif()

target_link_libraries(SPIRV-Tools-opt    INTERFACE SPIRV-Tools-static)
target_link_libraries(SPIRV-Tools-reduce INTERFACE SPIRV-Tools-static SPIRV-Tools-opt)
target_link_libraries(SPIRV-Tools-link   INTERFACE SPIRV-Tools-opt)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
target_link_libraries(OSDependent        INTERFACE Threads::Threads)
target_link_libraries(MachineIndependent INTERFACE OGLCompiler OSDependent GenericCodeGen)
target_link_libraries(glslang            INTERFACE OGLCompiler OSDependent MachineIndependent)

target_link_libraries(SPIRV              INTERFACE MachineIndependent SPIRV-Tools-opt)