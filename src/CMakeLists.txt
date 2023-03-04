file(GLOB SOURCE_FILES *.*pp)

find_package(small3d REQUIRED)
if(SMALL3D_OPENGL)
  find_package(glew REQUIRED)
  if(WIN32)
    find_package(opengl REQUIRED)
  else()
    find_package(OpenGL REQUIRED)
  endif()
else()
  find_package(vulkan REQUIRED)
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

if(SMALL3D_OPENGL)
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
else()
  target_link_libraries(gloom PUBLIC
  ${small3d_LIBRARIES}
  ${vulkan_LIBRARIES}
  ${glfw3_LIBRARIES}
  ${libpng_LIBRARIES_RELEASE}
  ${vorbis_LIBRARIES_RELEASE}
  ${ogg_LIBRARIES_RELEASE}
  ${portaudio_LIBRARIES}
  ${freetype_LIBRARIES}
  )
endif()

if(MSVC)
  set_target_properties(gloom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${PROJECT_BINARY_DIR}/bin")
endif()
