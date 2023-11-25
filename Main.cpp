#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include<GL/glew.h>
#include<glfw/glfw3.h>

#include"ReadFileCoord.h"
#include"Calculation.h"
#include"LoadTexture.h"


// Window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;

GLuint VBO, VAO, shader;
LoadTexture texture;
// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
layout (location = 1) in vec2 tex;											  \n\
																				\n\
out vec2 TexCoord;															\n\
																				\n\
																				\n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);				\n\
	TexCoord =tex;															\n\																		}";	

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
in vec2 TexCoord;															   \n\
																				\n\
																				\n\
out vec4 colour;                                                               \n\
uniform sampler2D theTexture;                                                  \n\
void main()                                                                   \n\
{                                                                             \n\
    colour = texture(theTexture,TexCoord);//vec4(0.2f,0.0f,1.0f,1.0);         \n\
}";

void CreateMesh()
{
	//Reading the control points from the ttext file
	ReadFileCoord cooord;
	std::vector<Point> controlPoints = cooord.getPointsFromFile("coordinate/AssignmentPoint.txt");
	Calculation vertexPoints;
	//Calculation of the vertices of the extrude geometry
	std::vector<GLfloat> realVertex=vertexPoints.calculateSplineCoords(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3],250);
	for (int i = 0; i < realVertex.size(); i++)
	{
		std::cout << realVertex[i] << endl;
	}

	//generation and binding of VAO/VBO
	glGenVertexArrays(1, &VAO);
	

	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,realVertex.size()*sizeof(float), realVertex.data(), GL_STATIC_DRAW);

	//How the each vertices are used for mesh and texture mapping
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(realVertex[0])*4, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(realVertex[0]) * 4, (void*)(sizeof(realVertex[0])*2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	//creating shader program
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	//connecting the code and compiling
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	//attaching the shader with the program
	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateMesh();
	CompileShaders();

	//loading txture image
	texture.LoadLoadTexture("Texture/wall.jpg");

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shader);
		
		glBindVertexArray(VAO);
		texture.UseTexture();
		glDrawArrays(GL_TRIANGLE_STRIP,0,3);
		glBindVertexArray(0);
		
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}