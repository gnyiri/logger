cmake_minimum_required(VERSION 2.6)

project(LOGGER)

include_directories($ {LOGGER_SOURCE_DIR})

set(LOGGER_MAIN_SRCS main.cxx)

add_library(logger SHARED $ {LOGGER_MAIN_SRCS})
