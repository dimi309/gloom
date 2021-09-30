# Unfortunately we need this hack for portaudio on Linux, because
# its conan package does not work there.
if(UNIX AND NOT MACOS)
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
  find_package(Portaudio REQUIRED)
endif()

add_executable(gloom main.cpp Game.cpp Map.cpp Enemy.cpp KeyInput.hpp Game.hpp Map.hpp Enemy.hpp)
target_link_libraries(gloom PUBLIC ${CONAN_LIBS} ${PORTAUDIO_LIBRARIES})
