#include <stdlib.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

#include "kazmath/kazmath/vec2.h"
#include "kazmath/kazmath/vec3.h"
#include "kazmath/kazmath/vec4.h"
#include "kazmath/kazmath/mat4.h"
#include "kazmath/kazmath/utility.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

const GLuint WIN_WIDTH = 800, WIN_HEIGHT = 600;
#define SHADER_SOURCE_SIZE  1024

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void set_tex_parameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void bind_vetex_attributes(GLuint *pVAO, GLuint *pVBO, GLuint *pEBO)
{
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
    	//position        //color             //texture coords
         0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// top right  
         0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right 
         -0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left   
         -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left
    };

	unsigned int indices[] = {  
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

    glGenVertexArrays(1, pVAO);
    glGenBuffers(1, pVBO);
    glGenBuffers(1, pEBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(*pVAO);

    glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int stride_size = 8;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride_size * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride_size * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
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


	//set the require callback functions
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	//viewport
	int viewPortWidth, viewPortHeight;
	glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);
	glViewport(0, 0, viewPortWidth, viewPortHeight);


	struct shader shader;

	char fragmentShaderSource[SHADER_SOURCE_SIZE];
	char vertexShaderSource[SHADER_SOURCE_SIZE];

	shader_source("5.2.shader.fs", fragmentShaderSource, SHADER_SOURCE_SIZE);
	shader_source("5.2.shader.vs", vertexShaderSource, SHADER_SOURCE_SIZE);

	shader_create(&shader, fragmentShaderSource, vertexShaderSource);


    GLuint VBO, VAO, EBO;
    bind_vetex_attributes(&VAO, &VBO, &EBO);

	int textures_len = 2;
	unsigned int textures[2] = {0, 0};
	load_image_bind_textures(textures);

	shader_use(&shader);

	glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader.id, "texture2"), 1);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		for(int i=0; i<textures_len; i++) 
		{
			//bind texture
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}

		shader_use(&shader);

		//rotate
		kmMat4 model;
		kmMat4Identity(&model);
		kmMat4 tmp; //变换矩阵
		kmMat4Translation(&tmp, 0.5f, -0.5f, 0.0);
		kmMat4Multiply(&model, &model, &tmp);
		kmMat4RotationZ(&tmp, (glfwGetTime()*kmDegreesToRadians(30)));
		kmMat4Multiply(&model, &model, &tmp);
		glUniformMatrix4fv(glGetUniformLocation(shader.id, "transform"), 1, GL_FALSE, &model.mat[0]);

        glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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

