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
#include "src/shader.h"



typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

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
    window = glfwCreateWindow(640, 490, "Square", NULL, NULL);
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


        Shader shader("../OpenGLeckMich/res/shaders/Basic.Shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.616f, 0.0f, 1.0f, 1.0f);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
        shader.Unbind();

        //cool oder??
        //red
        float r = 0.0f;
        float incrementR = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 1.0f, 1.0f);

			va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            //red
            if (r > 1.0f)
                incrementR = -0.05f;
            else if (r < 0.0f)
                incrementR = 0.06f;

            r += incrementR;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}