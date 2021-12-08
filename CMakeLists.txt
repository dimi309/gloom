cmake_minimum_required(VERSION 3.0.2)

project(game)

include(conanbuildinfo.cmake)
conan_basic_setup()

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "bin")

subdirs(src)
