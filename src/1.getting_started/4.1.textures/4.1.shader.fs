const GLchar* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"//FragColor = texture(ourTexture, TexCoord);\n" //origin textures
	"FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n" //get fancy colors on texture
	"}\n\0";
