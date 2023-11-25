#pragma once
#include <stdio.h>
#include <string.h>
#include<GL/glew.h>
#include<glfw/glfw3.h>
#include<string>
#include<SOIL2.h>
#include"stb_image.h"
class LoadTexture
{
public:
	LoadTexture();

	void LoadLoadTexture(const char* fileLoc);
	void UseTexture();
	void ClearTexture();
	~LoadTexture();
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

