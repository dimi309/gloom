file(GLOB SOURCE_FILES *.*pp)

find_package(small3d REQUIRED)

if(WIN32)
  add_executable(gloom ${SOURCE_FILES} resources.rc)
else()
  add_executable(gloom ${SOURCE_FILES})
endif()

target_include_directories(gloom PRIVATE ${CMAKE_INCLUDE_PATH})

target_link_libraries(gloom PRIVATE
  ${small3d_LIBRARIES}
)

if(MSVC)
  set_target_properties(gloom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}/bin")
endif()
