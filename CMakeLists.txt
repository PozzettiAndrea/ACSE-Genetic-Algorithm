cmake_minimum_required(VERSION 3.1)

project(Genetic_Algorithm
        LANGUAGES CXX)

# Add definition for relative path into project
add_definitions(-DPROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

# include(${CMAKE_SOURCE_DIR}/includes)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")

# Compiler specific settings
if (MSVC)
    message("Add definition _CRT_SECURE_NO_WARNINGS flag for MSVC compiler.")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> CONFIGURATIONS "Debug" "RelWithDebInfo" DESTINATION "${CMAKE_INSTALL_BINDIR}")
else ()
    set(CONVOLUTION_WARNING_FLAGS "-Wall -Wextra -pedantic-errors -Wconversion -Wformat")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -pg -O0 ${CONVOLUTION_WARNING_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG ${CONVOLUTION_WARNING_FLAGS}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message(STATUS "Using Link Time Optimization (-flto) on GNU platforms.")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto")
    endif ()
    if (${CMAKE_GENERATOR} MATCHES "MinGW Makefiles")
        # To prevent a redefinition conflict
        message(STATUS "Add definition _TIMESPEC_DEFINED")
        add_definitions(-D_TIMESPEC_DEFINED)
    endif ()
endif ()

# CMake cross compilation to mingw
if (${CROSS_COMPILE_MINGW})
    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)

    # cross compilers to use for C and C++
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

    # target environment on the build host system
    #   set 1st to dir with the cross compiler's C/C++ headers/libs
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

    # modify default behavior of FIND_XXX() commands to
    # search for headers/libs in the target environment and
    # search for programs in the build host environment
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif ()

set(CMAKE_CXX_EXTENSIONS OFF)

# Setting CXX standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the include directories to use <> instead of ""
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
)

# Add the files
file(
    GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/includes/*.h
)
add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/bin)

file(
    GLOB TEST1_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/test1.cpp
    ${CMAKE_SOURCE_DIR}/includes/*.h
)
list(REMOVE_ITEM TEST1_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(test1 ${TEST1_SOURCES})

file(
    GLOB TEST2_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/test2.cpp
    ${CMAKE_SOURCE_DIR}/includes/*.h
)
list(REMOVE_ITEM TEST2_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(test2 ${TEST2_SOURCES})

file(
    GLOB TEST3_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/tests/test3.cpp
    ${CMAKE_SOURCE_DIR}/includes/*.h
)
list(REMOVE_ITEM TEST3_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(test3 ${TEST3_SOURCES})

add_custom_target(
    runtests ALL
    python ${CMAKE_SOURCE_DIR}/run_tests.py ${CMAKE_SOURCE_DIR}
    DEPENDS test1 test2 test3
)
