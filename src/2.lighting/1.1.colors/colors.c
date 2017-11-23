/*
 * =====================================================================================
 *
 *       Filename:  colors.c
 *
 *    Description:  test opengl colors of light
 *
 *        Version:  1.0
 *        Created:  11/16/2017 18:30:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingmingxin (dmx), dingmingxin20@gmail.com
 *   Organization:  coding
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "kazmath/kazmath/vec2.h"
#include "kazmath/kazmath/vec3.h"
#include "kazmath/kazmath/vec4.h"
#include "kazmath/kazmath/mat4.h"
#include "kazmath/kazmath/GL/matrix.h"
#include "kazmath/kazmath/utility.h"

#include "shader.h"
#include "camera.h"

#define SHADER_SOURCE_SIZE 1024
#define MAX_FOV 75.0f
#define MIN_FOV 1.0f

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;

kmVec3 cameraPos = {0.0f, 0.0f, 3.0f};
kmVec3 cameraFront = {0, 0, -1.0f};
kmVec3 cameraUp	= {0.0f, 1.0f, 0.0f};
float cameraYaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float cameraPitch =  0.0f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov =  20.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void set_tex_parameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

struct camera camera;

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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

	//set the require callback functions
	glfwSetKeyCallback(window, key_callback);


	GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f, 
		-1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f, 
		1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
	}; 

	glewExperimental = GL_TRUE;
	glewInit();

	struct shader shader;
	char fragmentShaderSource[SHADER_SOURCE_SIZE];
	char vertexShaderSource[SHADER_SOURCE_SIZE];
	shader_source("1.1.colors.fs", fragmentShaderSource, SHADER_SOURCE_SIZE);
	shader_source("1.1.colors.vs", vertexShaderSource, SHADER_SOURCE_SIZE);
	shader_create(&shader, fragmentShaderSource, vertexShaderSource);

    GLuint VBO, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    


	shader_use(&shader);

	float cubePositions[10][3] = {
		{0.0f,  0.0f,  0.0f}, 
		{2.0f,  5.0f, -15.0f}, 
	};

	glEnable(GL_DEPTH_TEST);

	camera_init(&camera, cameraPos, cameraUp, cameraFront, cameraYaw, cameraPitch);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //计算时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		shader_use(&shader);

		kmMat4 view;
		camera_view_matrix(&camera, &view);

		kmMat4 projection;
		kmMat4PerspectiveProjection(&projection, fov, (float)WIN_WIDTH/(float)WIN_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, &projection.mat[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, &view.mat[0]);

        glBindVertexArray(lightVAO);
//		glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void 
mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraYaw += xoffset;
	cameraPitch += yoffset;


	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;

	camera_yaw(&camera, cameraYaw);
	camera_pitch(&camera, cameraPitch);
	camera_update_angle(&camera);

}

void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= MIN_FOV && fov <= MAX_FOV)
		fov -= yoffset;
    if (fov <= MIN_FOV)
        fov = MIN_FOV;
    if (fov >= MAX_FOV)
        fov = MAX_FOV;
}

// Is called whenever a key is pressed/released via GLFW
void 
key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	float offset = 2.5f * deltaTime;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
    	camera_move(&camera, CM_FORWARD, offset);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
    	camera_move(&camera, CM_BACKWARD, offset);
	}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
    	camera_move(&camera, CM_LEFT, offset);
	}

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		camera_move(&camera, CM_RIGHT, offset);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
