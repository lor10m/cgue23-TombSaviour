cmake_minimum_required(VERSION 3.22)

function(find_additional_cmake_file outputVariableName filename)
    if(NOT DEFINED outputVariableName OR NOT DEFINED filename)
        message(FATAL_ERROR "Wrong usage of find_and_include_additional_cmake_file(). Need both arguments.")
    endif()
    find_file(${outputVariableName} ${filename}
            PATHS "../cmake/" "../cmake_gl/" "./cmake/"
            NO_PACKAGE_ROOT_PATH NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
    if(NOT ${outputVariableName})
        message(FATAL_ERROR "Could not find ${filename}")
    endif()
endfunction()

find_additional_cmake_file(GLOBAL_SETTINGS_CMAKE GlobalSettings.cmake)
include(${GLOBAL_SETTINGS_CMAKE})
find_additional_cmake_file(SOURCES_LIST_CMAKE SourcesList.cmake)
include(${SOURCES_LIST_CMAKE})

if(NOT All_Sources)
    message(FATAL_ERROR "Even though included SourcesList.cmake, the expected variable seems to be empty.")
endif()

# --log-context is needed in the cmake commandline options to make use of this little helper in the log output (optional)
list(APPEND CMAKE_MESSAGE_CONTEXT ECG_Solution)

project(ECG_Solution VERSION 0.1.0)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# OpenGL
find_package(OpenGL REQUIRED)

################################################################################
# Target
################################################################################
# All_Sources is imported from SourcesList.cmake
message(STATUS "All_Sources: ${All_Sources}")
add_executable(${PROJECT_NAME} ${All_Sources} ../assets/settings.ini "src/PhysxMain.cpp" "src/Drawables/PhysxObject.cpp" "src/Drawables/PhysxObject.h" "src/GlobalVariables.cpp" "src/GlobalVariables.h")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(ROOT_NAMESPACE ${PROJECT_NAME})

file(COPY "physx/PhysX_64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    
file(COPY "physx/PhysXFoundation_64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    
file(COPY "physx/PhysXCommon_64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})
    
file(COPY "physx/PhysXCooking_64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})
   
file(COPY "physx/PhysXDevice64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})

file(COPY "physx/PhysXGpu_64.dll"
    DESTINATION ${EXECUTABLE_OUTPUT_PATH})

    

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)

################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>/"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<OR:$<CONFIG:Release>,$<CONFIG:ReleaseExe>>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>,$<CONFIG:ReleaseExe>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE
    "../external/include"
)
target_include_directories(${PROJECT_NAME} PRIVATE
    "../external/include/physx"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:ReleaseExe>:
        USE_INTERN_SHADERS
    >
    GLEW_STATIC
    _CRT_SECURE_NO_WARNINGS
    UNICODE
    _UNICODE
)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32
    )
endif()

if(DEFINED ENV{TOOLCHAIN_ACTIVE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        NDEBUG
    )
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            _DEBUG
        >
        $<$<CONFIG:Release>:
            NDEBUG
        >
        $<$<CONFIG:ReleaseExe>:
            NDEBUG
        >
    )
endif()


################################################################################
# Compile and link options
################################################################################
# The compiler flags are defined in the CMakePresets.json!
# Link options are not needed here.
# Overwrites the settings from the presets by intention, because the presets disable debugging on purpose for the
# toolchain! Note also that the presets configuration applies to the whole project at once while these settings below
# are applied target-wise. Therefore, we need to disable debugging first by the presets for the whole project so
# the toolchain can build the lib without debugging flags, but we enable it here in the CMakeLists for the target where
# it can be allowed.
if(MSVC)
    # https://docs.microsoft.com/en-us/previous-versions/958x11bc(v=vs.140)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /ZI
        >
        $<$<OR:$<CONFIG:Release>,$<CONFIG:ReleaseExe>>:
            /Zi
        >
    )
else()
    # https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
    # https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html#Debugging-Options
    # -O0: "Optimize debugging experience. -Og should be the optimization level of choice for the standard
    #       edit-compile-debug cycle, offering a reasonable level of optimization while maintaining fast compilation
    #       and a good debugging experience. It is a better choice than -O0 for producing debuggable code because some
    #       compiler passes that collect debug information are disabled at -O0."
    target_compile_options(${PROJECT_NAME} PRIVATE
        -g3 -Og
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "ECG_Library_Debug"
    ">"
    "$<$<CONFIG:Release>:"
        "ECG_Library_Release"
    ">"
    "$<$<CONFIG:ReleaseExe>:"
        "ECG_Library_ReleaseExe"
    ">"
)
if(WIN32)
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES
        "opengl32"
        "glew32s"
        "PhysXCommon_64.lib"
        "PhysX_64.lib"
        "PhysXFoundation_64.lib"
        "PhysXExtensions_static_64.lib"
        "PhysXPvdSDK_static_64.lib"
    )
else()
    set(GLEW_USE_STATIC_LIBS TRUE)
    find_package(GLEW REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
    )

    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES
        ${GLEW_STATIC_LIBRARIES} # ATTENTION: order is important, this must come before GL
        ${OPENGL_LIBRARIES}
    )
endif()

if(WIN32)
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES "glfw3_mt")
else()
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES "glfw")
endif()

if (TARGET ECG_Library)
    # In addition to the specific library names per <CONFIG> stage above, add the target name of the libary here again to
    # declare a dependency from ECG_Solution onto ECG_Library, so the library will automatically be rebuilt when building
    # the solution project. This is equivalent to using add_dependencies, however, target_link_libraries should be
    # preferred over add_dependencies if possible. This is not needed in the delivered student assignment code as the
    # dependency has already been built and exists in ../external/lib, in contrast, it will lead to an error if
    # "ECG_Library" is not a target name, because CMake will interpret it then as a library filename and will not find
    # a library named "ECG_Library.lib". Therefore we need to check if it is a defined target or not.
    list(APPEND ADDITIONAL_LIBRARY_DEPENDENCIES ECG_Library)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "../external/lib"
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_directories(${PROJECT_NAME} PRIVATE "../external/lib/PhysxDebug")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
    target_link_directories(${PROJECT_NAME} PRIVATE "../external/lib/PhysxRelease")
endif()

################################################################################
# Assets handling
################################################################################
file(COPY "../assets" DESTINATION .) # Copy assets to current binary directory via "."