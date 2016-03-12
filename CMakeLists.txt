cmake_minimum_required (VERSION 2.6)

project (LOGGER)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# set up sources
set(logger_sources
  src/logger.cxx
  src/main.cxx
)

add_definitions ("-Wall -ansi -pedantic")

add_executable (logger_main ${logger_sources})
