#include "GLEW Helpers.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int loadGLTexture(std::string file_path) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width;
	int height;
	int num_components;
	unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &num_components, 0);
	if (data) {
		GLenum format;
		if (num_components == 3) {
			format = GL_RGB;
		}
		else if (num_components == 4) {
			format = GL_RGBA;
		}
		else {
			format = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		stbi_image_free(data);
	}

	return texture_id;
}

unsigned int loadGLTexture(std::string file_path, int* width, int* height) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int num_components;

	unsigned char* data = stbi_load(file_path.c_str(), width, height, &num_components, 0);
	if (data) {
		GLenum format;
		if (num_components == 3) {
			format = GL_RGB;
		}
		else if (num_components == 4) {
			format = GL_RGBA;
		}
		else {
			format = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << file_path << "\n";
		stbi_image_free(data);
	}

	return texture_id;
}

unsigned int loadGLTexture(std::string file_path, int* width, int* height, int* num_components) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	unsigned char* data = stbi_load(file_path.c_str(), width, height, num_components, 0);
	if (data) {
		GLenum format;
		if (*num_components == 3) {
			format = GL_RGB;
		}
		else if (*num_components == 4) {
			format = GL_RGBA;
		}
		else {
			format = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << file_path << "\n";
		stbi_image_free(data);
	}

	return texture_id;
}