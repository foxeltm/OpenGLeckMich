# CMakeList.txt : CMake project for OpenGLeckMich, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (OpenGLeckMich "OpenGLeckMich.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLeckMich PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries(OpenGLeckMich glfw)
# TODO: Add tests and install targets if needed.
