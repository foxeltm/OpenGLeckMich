#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "src/renderer.h"
#include "src/VertexBuffer.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"



typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static u32 CompileShader(const std::string& source, u32 type)
{
  GLCall(u32 id = glCreateShader(type));
  GLCall(const char* src = source.c_str());
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE)
   {
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char* message = (char*)alloca(length * sizeof(char));
       glGetShaderInfoLog(id, length, &length, message);
       std::cout << "Failed to compile " <<
           (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!!" << std::endl;
       std::cout << message << std::endl;
       glDeleteShader(id);
       return 0;
   }

   return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(u32 program = glCreateProgram());
    u32 vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    u32 fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(3);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR! " << std::endl;
    {
        float positions[8] = {
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.5f,  0.5f, //2
            -0.5f,  0.5f  //3
        };

        u32 indicies[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        u32 vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

		VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

        IndexBuffer ib(indicies, 6);

        ShaderProgramSources source = ParseShader("../OpenGLeckMich/res/shaders/Basic.Shader");
        u32 shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1)
            GLCall(glUniform4f(location, 0.616f, 0.0f, 1.0f, 1.0f));

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //cool oder??
        //red
        float r = 0.0f;
        float incrementR = 0.05f;

        //green
        float g = 0.0f;
        float incrementG = 0.05f;

        //blue
        float b = 0.0f;
        float incrementB = 0.05f;


        //ich bin so cool
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, g, b, 1.0f));

			va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //red
            if (r > 1.0f)
                incrementR = -0.05f;
            else if (r < 0.0f)
                incrementR = 0.06f;

            r += incrementR;

            //green
            if (g > 1.0f)
                incrementG = -0.04f;
            else if (g < 0.0f)
                incrementG = 0.07f;

            g += incrementG;

            //blue
            if (b > 1.0f)
                incrementB = -0.01f;
            else if (b < 0.0f)
                incrementB = 0.02f;

            b += incrementB;




            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}