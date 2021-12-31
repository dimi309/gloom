file(GLOB SOURCE_FILES *.*pp)

if(WIN32)
  add_executable(gloom ${SOURCE_FILES} resources.rc)
else()
  add_executable(gloom ${SOURCE_FILES})
endif()

target_link_libraries(gloom PUBLIC ${CONAN_LIBS})

if(MSVC)
  set_target_properties(gloom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}/bin")
endif()
