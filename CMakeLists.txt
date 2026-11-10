#what is there better than a cmake file cooked up by claude? i dont understand shit : )
cmake_minimum_required(VERSION 3.20)
project(mymod LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# ---------------------------------------------------------------
# Folders that contain code that is NOT yours.
# Their headers are silenced, their .cpp files get no strict flags.
# Add more folder names here if you need them.
# ---------------------------------------------------------------
set(THIRD_PARTY_DIRS libs)

# ---------------------------------------------------------------
# Your own code
# ---------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# ---------------------------------------------------------------
# Third-party code: collect .cpp files and header folders
# ---------------------------------------------------------------
set(THIRD_PARTY_SOURCES "")
set(THIRD_PARTY_INCLUDES "")

foreach(dir IN LISTS THIRD_PARTY_DIRS)
  # every .cpp inside the folder, at any depth
  file(GLOB_RECURSE found CONFIGURE_DEPENDS ${dir}/*.cpp)
  list(APPEND THIRD_PARTY_SOURCES ${found})

  # the folder itself, plus each direct subfolder (libs/imgui, libs/plthook, ...)
  list(APPEND THIRD_PARTY_INCLUDES ${CMAKE_SOURCE_DIR}/${dir})
  file(GLOB children LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/${dir}/*)
  foreach(child IN LISTS children)
    if(IS_DIRECTORY ${child})
      list(APPEND THIRD_PARTY_INCLUDES ${child})
    endif()
  endforeach()
endforeach()

# Third-party .cpp files are compiled separately, with no strict warnings
if(THIRD_PARTY_SOURCES)
  add_library(thirdparty OBJECT ${THIRD_PARTY_SOURCES})
  set_property(TARGET thirdparty PROPERTY POSITION_INDEPENDENT_CODE ON)
  target_include_directories(thirdparty SYSTEM PRIVATE ${THIRD_PARTY_INCLUDES})
  target_compile_options(thirdparty PRIVATE -w -fno-gnu-unique)
endif()

# ---------------------------------------------------------------
# The mod itself (shared library)
# ---------------------------------------------------------------
add_library(mymod SHARED ${SOURCES})

if(THIRD_PARTY_SOURCES)
  target_sources(mymod PRIVATE $<TARGET_OBJECTS:thirdparty>)
endif()

# Output file is exactly "loader.so" (no "lib" prefix) in build/<type>/
set_target_properties(mymod PROPERTIES
  PREFIX ""
  OUTPUT_NAME "loader"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Your headers: normal, warnings on
target_include_directories(mymod PRIVATE src include)

# Third-party headers: SYSTEM = no warnings from them
target_include_directories(mymod SYSTEM PRIVATE ${THIRD_PARTY_INCLUDES})

find_package(Threads REQUIRED)
target_link_libraries(mymod PRIVATE Threads::Threads ${CMAKE_DL_LIBS})

target_compile_options(mymod PRIVATE
  -Wall -Wextra -pedantic
  -Wconversion -Wsign-conversion
  $<$<CONFIG:Debug>:-Werror>
  -fno-gnu-unique
)