cmake_minimum_required(VERSION 3.20)
project(mymod LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CRITICAL FIX: Force -fPIC globally for all targets, static libs, and subprojects
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# ---------------------------------------------------------------
# MODULAR SCANNER: Automatically check every folder inside libs/
# ---------------------------------------------------------------
file(GLOB lib_children RELATIVE ${CMAKE_SOURCE_DIR}/libs ${CMAKE_SOURCE_DIR}/libs/*)
foreach(child IN LISTS lib_children)
  set(lib_dir "${CMAKE_SOURCE_DIR}/libs/${child}")
  if(IS_DIRECTORY "${lib_dir}" AND EXISTS "${lib_dir}/CMakeLists.txt")
    message(STATUS "Auto-detected subproject in libs/: ${child}")
    
    set(${child}_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(${child}_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    
    add_subdirectory(libs/${child})
  endif()
endforeach()

# ---------------------------------------------------------------
# Your own code
# ---------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# ---------------------------------------------------------------
# The mod itself (shared library)
# ---------------------------------------------------------------
add_library(mymod SHARED ${SOURCES})

# Output file is exactly "mifihook.so" (no "lib" prefix) in build/<type>/
set_target_properties(mymod PROPERTIES
  PREFIX ""
  OUTPUT_NAME "mifihook"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET mymod POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$ENV{HOME}/inject/tf2"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:mymod>" "$ENV{HOME}/inject/tf2/a.so"
  COMMENT "Copying mifihook to ~/inject/tf2/a.so"
)

# Your headers
target_include_directories(mymod PRIVATE src include)

# Link threads, dynamic loader library, and funchook-static
find_package(Threads REQUIRED)
target_link_libraries(mymod PRIVATE 
  Threads::Threads 
  ${CMAKE_DL_LIBS}
  funchook-static
)

target_compile_options(mymod PRIVATE
  -Wall -Wextra -pedantic
  -Wconversion -Wsign-conversion
  $<$<CONFIG:Debug>:-Werror>
  -fno-gnu-unique
)