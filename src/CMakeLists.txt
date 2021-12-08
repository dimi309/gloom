add_executable(gloom main.cpp Game.cpp Map.cpp Enemy.cpp KeyInput.hpp Game.hpp Map.hpp Enemy.hpp)
target_link_libraries(gloom PUBLIC ${CONAN_LIBS})

if(MSVC)
  set_target_properties(gloom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}/bin")
endif()
