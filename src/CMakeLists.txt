add_executable(gloom main.cpp Game.cpp Map.cpp Enemy.cpp KeyInput.hpp Game.hpp Map.hpp Enemy.hpp)
target_link_libraries(gloom PUBLIC ${CONAN_LIBS})
