#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "kazmath/kazmath/vec2.h"
#include "kazmath/kazmath/vec3.h"
#include "kazmath/kazmath/vec4.h"
#include "kazmath/kazmath/mat4.h"
#include "kazmath/kazmath/GL/matrix.h"
#include "kazmath/kazmath/utility.h"

#include "shader.h"

#define SHADER_SOURCE_SIZE 1024

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;


kmVec3 cameraPos = {0.0f, 0.0f, 3.0f};
kmVec3 cameraFront = {0, 0, -1.0f};
kmVec3 cameraUp	= {0.0f, 1.0f, 0.0f};

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//shaders

void set_tex_parameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void bind_vetex_attributes(GLuint *pVAO, GLuint *pVBO)
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    glGenVertexArrays(1, pVAO);
    glGenBuffers(1, pVBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(*pVAO);

    glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    int stride_size = 5;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void load_image_bind_textures(unsigned int *textures)
{

	stbi_set_flip_vertically_on_load(1); 
	char *images[2] = {"res/wall.jpg", "res/awesomeface.png"};
	for(int i=0; i<2 ; i++) {
		glGenTextures(1, textures+i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		set_tex_parameters();
		int width, height, nrChannels;
		unsigned char * data = stbi_load(images[i], &width, &height, &nrChannels, 0);
		if(data) 
		{
			if(i==0) 
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else 
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else 
		{
			printf("Failed to load texture\n");
		}
		stbi_image_free(data);
	}
}

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


	//set the require callback functions
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	struct shader shader;
	char fragmentShaderSource[SHADER_SOURCE_SIZE];
	char vertexShaderSource[SHADER_SOURCE_SIZE];
	shader_source("7.2.shader.fs", fragmentShaderSource, SHADER_SOURCE_SIZE);
	shader_source("7.2.shader.vs", vertexShaderSource, SHADER_SOURCE_SIZE);
	shader_create(&shader, fragmentShaderSource, vertexShaderSource);

    GLuint VBO, VAO;
    bind_vetex_attributes(&VAO, &VBO);

	int textures_len = 2;
	unsigned int textures[2] = {0, 0};
	load_image_bind_textures(textures);

	shader_use(&shader);

	glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader.id, "texture2"), 1);

	float cubePositions[10][3] = {
		{0.0f,  0.0f,  0.0f}, 
		{2.0f,  5.0f, -15.0f}, 
		{1.5f, -2.2f, -2.5f},  
		{3.8f, -2.0f, -12.3f},  
		{-0.4f, -0.4f, -3.5f},  
		{-1.7f,  3.0f, -7.5f},  
		{-1.3f, -2.0f, -2.5f},  
		{-1.5f,  2.0f, -2.5f}, 
		{1.5f,  0.2f, -1.5f}, 
		{-1.3f,  1.0f, -1.5f}  
	};

	glEnable(GL_DEPTH_TEST);
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

		for(int i=0; i<textures_len; i++) 
		{
			//bind texture
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}

		shader_use(&shader);

		kmMat4 view;
		kmMat4LookAt(&view, &cameraPos, &cameraFront, &cameraUp);

		kmMat4 projection;
		kmMat4PerspectiveProjection(&projection, 45, (float)WIN_WIDTH/(float)WIN_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, &projection.mat[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, &view.mat[0]);

        glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++)
		{

			float angle = 1.0f;
			kmMat4 model, tmp;
			kmMat4Identity(&model);
			kmMat4Translation(&tmp, cubePositions[i][0], cubePositions[i][1], cubePositions[i][2]);
			kmMat4Multiply(&model, &model, &tmp);

			kmMat4Identity(&tmp);
			kmMat4RotationYawPitchRoll(&tmp, 1.0f*angle, 0.6f*angle, 0.5f*angle);
			kmMat4Multiply(&model, &model, &tmp);

			glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &model.mat[0]);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float cameraSpeed = 2.5f * deltaTime;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
    	kmVec3 tmp;
    	kmVec3Scale(&tmp, &cameraFront, cameraSpeed);
    	kmVec3Add(&cameraPos, &cameraPos, &tmp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
    	kmVec3 tmp;
    	float speed = -cameraSpeed;
    	kmVec3Scale(&tmp, &cameraFront, speed);
    	kmVec3Add(&cameraPos, &cameraPos, &tmp);
	}

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
    	kmVec3 tmp;
    	kmVec3Cross(&tmp, &cameraFront, &cameraUp);
    	kmVec3Normalize(&tmp, &tmp);
    	kmVec3Scale(&tmp, &tmp, -cameraSpeed);
//    	printf("A : %f %f %f\n", tmp.x, tmp.y, tmp.z);
    	kmVec3Add(&cameraPos, &cameraPos, &tmp);
    	// 叉乘, 标准化, scale, add
//		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
	{
    	kmVec3 tmp;
    	kmVec3Cross(&tmp, &cameraFront, &cameraUp);
    	kmVec3Normalize(&tmp, &tmp);
    	kmVec3Scale(&tmp, &tmp, cameraSpeed);
    	kmVec3Add(&cameraPos, &cameraPos, &tmp);
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
