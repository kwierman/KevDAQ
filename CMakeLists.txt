cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project (kevdaq)
set(kevdaq_MAJOR_VERSION 0)
set(kevdaq_MINOR_VERSION 1)
set(kevdaq_PATCH_VERSION 0)
set(kevdaq_VERSION
  ${kevdaq_MAJOR_VERSION}.${kevdaq_MINOR_VERSION}.${kevdaq_PATCH_VERSION})

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS ${PROJECT_NAME} " requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

find_package(Boost 1.58.0 REQUIRED COMPONENTS python thread)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
endif()
###########################################################################################
include(FindPackageHandleStandardArgs)
if (PYTHON_EXECUTABLE)
    get_filename_component(PYTHON_EXECUTABLE "${PYTHON_EXECUTABLE}" REALPATH)
endif ()
if (PYTHON_EXECUTABLE AND EXISTS ${PYTHON_EXECUTABLE}-config)
    set(PYTHON_CONFIG ${PYTHON_EXECUTABLE}-config CACHE PATH "" FORCE)
else ()
    find_program(PYTHON_CONFIG
        NAMES python-config python-config2.7 python-config2.6 python-config2.6
              python-config2.4 python-config2.3)
endif ()
if (PYTHON_CONFIG AND NOT ${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
    execute_process(COMMAND "${PYTHON_CONFIG}" --ldflags
                    OUTPUT_VARIABLE PYTHON_LIBRARIES
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    string(STRIP "${PYTHON_LIBRARIES}" PYTHON_LIBRARIES)

    execute_process(COMMAND "${PYTHON_CONFIG}" --includes
                    OUTPUT_VARIABLE PYTHON_INCLUDE_DIR
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)

    string(REGEX REPLACE "^[-I]" "" PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
    string(REGEX REPLACE "[ ]-I" " " PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
    separate_arguments(PYTHON_INCLUDE_DIR)

    find_package_handle_standard_args(PythonDev DEFAULT_MSG
        PYTHON_CONFIG
        PYTHON_INCLUDE_DIR
        PYTHON_LIBRARIES
    )
else ()
    find_package(PythonLibs)
    if (PYTHON_INCLUDE_PATH AND NOT PYTHON_INCLUDE_DIR)
        set(PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_PATH}")
    endif ()
    find_package_handle_standard_args(PythonDev DEFAULT_MSG
        PYTHON_INCLUDE_DIR
        PYTHON_LIBRARIES
    )
endif ()
include_directories(${PYTHON_INCLUDE_DIR})
###########################################################################################


file (GLOB srcs src/*.cc)
add_library (kevdaq SHARED ${srcs})
target_include_directories( kevdaq PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries( kevdaq  ${Boost_LIBRARIES} )
install(TARGETS kevdaq DESTINATION lib)

option(BUILD_TESTS "Build Unit Tests" OFF)
mark_as_advanced(BUILD_TESTS)
if(BUILD_TESTS)
  add_subdirectory(googletest)
  file (GLOB testSources test/*.cc)
  add_executable (kevdaqtest ${testSources} )
  target_include_directories (kevdaqtest PRIVATE ${gtest_SOURCE_DIR}/include)
  target_include_directories (kevdaqtest PRIVATE ${gmock_SOURCE_DIR}/include)
  target_include_directories (kevdaqtest PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_link_libraries (kevdaqtest kevdaq gmock gmock_main gtest ${PYTHON_LIBRARIES})

  enable_testing()

  add_test(Unit kevdaqtest)

  add_custom_target(unit COMMAND ${CMAKE_BINARY_DIR}/kevdaqtest --gtest_color=true)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall 
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_custom_target(setup
  COMMAND export PATH=$PATH:${CMAKE_INSTALL_PREFIX}/bin
  COMMAND export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/lib
  COMMAND export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/lib
  COMMAND export PYTHONPATH=$PYTHONPATH:${CMAKE_INSTALL_PREFIX}/lib
  )
