file(GLOB SOURCE_FILES *.*pp)

find_package(small3d REQUIRED)
find_package(glew REQUIRED)
if(WIN32)
  find_package(opengl REQUIRED)
else()
  find_package(OpenGL REQUIRED)
endif()
find_package(glfw3 REQUIRED)
if(WIN32)
  find_package(png REQUIRED)
  find_package(vorbis REQUIRED)
else()
  find_package(PNG REQUIRED)
  find_package(Vorbis REQUIRED)
endif()
find_package(Ogg REQUIRED)
find_package(Portaudio REQUIRED)
find_package(freetype REQUIRED)

if(WIN32)
  add_executable(gloom ${SOURCE_FILES} resources.rc)
else()
  add_executable(gloom ${SOURCE_FILES})
endif()

target_include_directories(gloom PUBLIC ${CMAKE_INCLUDE_PATH})

MESSAGE(STATUS "Linking to: "
  ${small3d_LIBRARIES} ", "
  ${glew_LIBRARIES} ", "
  ${opengl_system_LIBRARIES} ", "
  ${glfw3_LIBRARIES} ", "
  ${libpng_LIBRARIES_RELEASE} ", "
  ${vorbis_LIBRARIES_RELEASE} ", "
  ${ogg_LIBRARIES_RELEASE} ", "
  ${portaudio_LIBRARIES} ", "
  ${freetype_LIBRARIES}
  )
  
target_link_libraries(gloom PUBLIC
 ${small3d_LIBRARIES}
  ${glew_LIBRARIES}
  ${opengl_system_LIBRARIES}
  ${glfw3_LIBRARIES}
  ${libpng_LIBRARIES_RELEASE}
  ${vorbis_LIBRARIES_RELEASE}
  ${ogg_LIBRARIES_RELEASE}
  ${portaudio_LIBRARIES}
  ${freetype_LIBRARIES}
  )

if(MSVC)
  set_target_properties(gloom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}/bin")
endif()
