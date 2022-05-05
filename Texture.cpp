#include "Texture.h"


Texture::Texture() {
	GLuint textureID;
	glGenTextures(1, &textureID);
	textureId = textureID;
}

Texture::Texture(float _data[], int _width, int _height) {
	GLuint textureID;
	width =  _width;
	height = _height;
	data =   _data;
}

void Texture::BindTexture() {
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::updateData(float new_data[]) {
	data = new_data;
}

void Texture::applyTexture() {
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

