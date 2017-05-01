cmake_minimum_required(VERSION 2.6)
project(untyped)

add_executable(untyped main.cpp Term.cpp Parser.cpp Printer.cpp)

install(TARGETS untyped RUNTIME DESTINATION bin)
