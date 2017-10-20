/*
 * =====================================================================================
 *
 *       Filename:  shader.c
 *
 *    Description:  测试shader，将shader封装成单独的模块
 *
 *        Version:  1.0
 *        Created:  10/20/2017 16:49:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingmingxin (dmx), dingmingxin20@gmail.com
 *   Organization:  coding
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <stdlib.h>
#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "shader.h"

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


const GLchar* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 position; \n"
	"layout (location = 1) in vec3 color; \n"
	"out vec3 ourColor; \n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0); \n"
	"ourColor = color;\n"
	"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(ourColor, 1.0);\n"
	"}\n\0";

int 
main()
{
	//init glfw
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
#endif
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	//create a glfwwindow object that we can use for glfw`s functions
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);


	//set the require callback functions
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	//viewport
	int viewPortWidth, viewPortHeight;
	glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);
	glViewport(0, 0, viewPortWidth, viewPortHeight);

	struct shader shader;
	shader_create(&shader, fragmentShaderSource, vertexShaderSource);

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
    	//position
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left  
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Right 
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top   
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO 


    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float ourColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/2.0f) + 0.5f;

		int loc = shader_uniform_location(&shader, "ourColor");
        ourColor[1] = greenValue;
        shader_use(&shader);
		shader_set_uniform(&shader, loc, UNIFORM_FLOAT3, ourColor);

        glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

