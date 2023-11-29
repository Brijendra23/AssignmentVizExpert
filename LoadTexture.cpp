#include "LoadTexture.h"

LoadTexture::LoadTexture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation =NULL;
}


void LoadTexture::LoadLoadTexture(const char* fileLoc)
{
	fileLocation = fileLoc;
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData)
	{
		
		printf("failed to find: %s\n", fileLocation);
		return;
	}
	//Generating and binding the texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	//adjusting the parameters to contorl the wrraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	//Generating mip map and usuage for the data
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, height, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(texData);
}

void LoadTexture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void LoadTexture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0; height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}



LoadTexture::~LoadTexture()
{
	ClearTexture();
}
