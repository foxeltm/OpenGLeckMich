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
#include "src/VertexBufferLayout.h"
#include "src/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    window = glfwCreateWindow(960, 540, "Square", NULL, NULL);
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
        float positions[16] = {
            100.0f, 100.0f, 0.0f, 0.0f,//0
            200.0f, 100.0f, 1.0f, 0.0f,//1
            200.0f, 200.0f, 1.0f, 1.0f,//2
            100.0f, 200.0f, 0.0f, 1.0f//3
        };

        u32 indicies[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		

		VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));

        VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
        //cool

        IndexBuffer ib(indicies, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f , -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;

        Shader shader("../OpenGLeckMich/res/shaders/Basic.Shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.616f, 0.0f, 1.0f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/terraria.png");
		texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
		//papa ist hier

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
        shader.Unbind();

		Renderer renderer;

        //red
        float r = 0.0f;
        float incrementR = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
			renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 1.0f, 1.0f);

			renderer.Draw(va, ib, shader);
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