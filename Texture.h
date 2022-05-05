#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>

class Texture
{
	GLuint textureId;
	float *data;
	int width;
	int height;
	public:
		Texture();
		Texture(float[], int, int);
		void BindTexture();
		void updateData(float[]);
		void applyTexture();
};
#endif