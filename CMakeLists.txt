cmake_minimum_required(VERSION 3.0.2)

project(game)

include(conanbuildinfo.cmake)
conan_basic_setup()

file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

if(MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
endif()

subdirs(src)
