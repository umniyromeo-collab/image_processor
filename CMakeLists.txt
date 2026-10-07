cmake_minimum_required(VERSION 3.14)
project(image_processor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

option(IMAGE_PROCESSOR_BUILD_TESTS "Build unit tests" ON)

# Header-only core: BMP I/O, image model, filters, exceptions
add_library(image_processor_core INTERFACE)
target_include_directories(image_processor_core INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Main
        ${CMAKE_CURRENT_SOURCE_DIR}/BMP
        ${CMAKE_CURRENT_SOURCE_DIR}/Exceptions
        ${CMAKE_CURRENT_SOURCE_DIR}/Abstract_Filters
        ${CMAKE_CURRENT_SOURCE_DIR}/Final_Filters
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(image_processor_core INTERFACE -Wall -Wextra -Wimplicit-fallthrough)
elseif (MSVC)
    target_compile_options(image_processor_core INTERFACE /W4 /utf-8)
endif ()

add_executable(image_processor Main/image_processor.cpp)
target_link_libraries(image_processor PRIVATE image_processor_core)

# MinGW: link the runtime statically so the .exe runs without extra DLLs
if (MINGW)
    target_link_options(image_processor PRIVATE -static -static-libgcc -static-libstdc++)
endif ()

if (IMAGE_PROCESSOR_BUILD_TESTS)
    enable_testing()

    add_library(catch_main STATIC third_party/catch/catch_main.cpp)
    target_include_directories(catch_main PUBLIC third_party/catch)

    add_executable(unit_tests tests/unit_tests.cpp)
    target_link_libraries(unit_tests PRIVATE image_processor_core catch_main)
    add_test(NAME unit_tests COMMAND unit_tests)

    find_package(Python3 COMPONENTS Interpreter)
    if (Python3_FOUND)
        add_test(NAME filter_tests
                COMMAND ${Python3_EXECUTABLE} test_script/test_image_processor.py $<TARGET_FILE:image_processor>
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif ()
endif ()
