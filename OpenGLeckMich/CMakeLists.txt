# CMakeList.txt : CMake project for OpenGLeckMich, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (OpenGLeckMich 
"OpenGLeckMich.cpp"   
"src/renderer.h" 
"src/renderer.cpp" 
"src/VertexBuffer.h" 
"src/VertexBuffer.cpp" 
"src/IndexBuffer.h" 
"src/IndexBuffer.cpp"
"src/VertexBufferLayout.h"
"src/VertexArray.h"
"src/VertexArray.cpp"
 "src/Shader.h" "src/Shader.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLeckMich PROPERTY CXX_STANDARD 20)
endif()


target_include_directories(OpenGLeckMich PRIVATE 
    "${CMAKE_SOURCE_DIR}/vendor/glfw/include"
    "${CMAKE_SOURCE_DIR}/vendor/glew-cmake/include"
)

target_link_libraries(OpenGLeckMich PRIVATE glfw libglew_static)
# TODO: Add tests and install targets if needed.
