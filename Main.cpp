#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include<GL/glew.h>
#include<glfw/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include"ReadFileCoord.h"
#include"LoadTexture.h"
#include"Window.h"
#include"Camera.h"

// Window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;
//window
Window mainwindow;
Camera camera;
GLuint VBO, VAO, shader;
LoadTexture texture;



GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;



// Vertex Shader code
static const char* vShader = "                                                  \n\
#version 330                                                                    \n\
                                                                                \n\
layout (location = 0) in vec3 pos;											    \n\
layout (location = 1) in vec2 tex;											    \n\
																				\n\
out vec2 TexCoord;															    \n\
																				\n\
uniform mat4 model;                                                                      \n\
uniform mat4 projection;                                                                                \n\
uniform mat4 view;                                                                                \n\
                                                                                \n\
																				\n\
void main()                                                                     \n\
{                                                                               \n\
    gl_Position = projection*view*model* vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);				    \n\
	TexCoord =tex;															    \n\																		}";	

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
	
	/*for (int i = 0; i < controlPoints.size(); i++)
	{
		std::cout << controlPoints[i].x << " " << controlPoints[i].y << std::endl;
	}*/
	//Calculation of the vertices of the extrude geometry
	std::vector<GLfloat> realVertex=cooord.calculateSplineCoords(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3],1000);
	for (int i = 0; i < realVertex.size(); i+=2)
	{
		std::cout << realVertex[i]<<","<< realVertex[i+1] << endl;
	}

	//generation and binding of VAO/VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,realVertex.size()*sizeof(float), realVertex.data(), GL_STATIC_DRAW);

	//How the each vertices are used for mesh and texture mapping
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(realVertex[0])*2, 0);
	glEnableVertexAttribArray(0);

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
	mainwindow = Window(800, 600);
	mainwindow.Initialise();
	

	CreateMesh();
	CompileShaders();

	camera= Camera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.50f, 0.1f);


	//loading txture image
	texture.LoadLoadTexture("Texture/brick.png");

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat) mainwindow.getBufferWidth() / mainwindow.getBufferHeight(), 0.1f, 100.0f);
	// Loop until window closed
	while (!mainwindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;
		// Get + Handle user input events
		glfwPollEvents();


		camera.keyControl(mainwindow.getsKeys(), deltaTime);
		camera.mouseControl(mainwindow.getXChange(), mainwindow.getYChange());



		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		
		glUseProgram(shader);
		uniformModel = glGetUniformLocation(shader, "model");
		uniformProjection = glGetUniformLocation(shader, "projection");
		uniformView = glGetUniformLocation(shader, "view");
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.010f, 0.010f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glBindVertexArray(VAO);
		texture.UseTexture();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
		glBindVertexArray(0);
		
		glUseProgram(0);
		mainwindow.swapBuffers();
	}

	return 0;
}