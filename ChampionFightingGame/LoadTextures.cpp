#include "LoadTextures.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "EasyGifReader.h"
#include <glew/glew.h>
#include "ResourceManager.h"

unsigned int loadGLTexture(std::string file_path) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width;
	int height;
	int num_components;
	unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &num_components, 0);
	if (data) {
		GLenum internal_format;
		GLenum format;
		switch (num_components) {
			case 3: {
				if (ResourceManager::get_instance()->is_srgb()) {
					internal_format = GL_SRGB;
				}
				else {
					internal_format = GL_RGB;
				}
				format = GL_RGB;
			} break;
			case 4: {
				if (ResourceManager::get_instance()->is_srgb()) {
					internal_format = GL_SRGB8_ALPHA8;
				}
				else {
					internal_format = GL_RGBA16F;
				}
				format = GL_RGBA;
			} break;
			default: {
				internal_format = GL_RED;
				format = GL_RED;
			} break;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture at " << file_path << "\n";
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
		GLenum internal_format;
		GLenum format;
		switch (num_components) {
			case 3: {
				if (ResourceManager::get_instance()->is_srgb()) {
					internal_format = GL_SRGB;
				}
				else {
					internal_format = GL_RGB;
				}
				format = GL_RGB;
			} break;
			case 4: {
				if (ResourceManager::get_instance()->is_srgb()) {
					internal_format = GL_SRGB8_ALPHA8;
				}
				else {
					internal_format = GL_RGBA16F;
				}
				format = GL_RGBA;
			} break;
			default: {
				internal_format = GL_RED;
				format = GL_RED;
			} break;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, *width, *height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

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

std::vector<unsigned int> loadGLTextures(std::string file_path) {
	std::vector<unsigned int> ret;
	EasyGifReader loader = EasyGifReader::openFile(file_path.c_str());
	ret.reserve(loader.frameCount());
	if (ret.capacity()) {
		int width = loader.width();
		int height = loader.height();
		for (const EasyGifReader::Frame& frame : loader) {
			const std::uint32_t* data = frame.pixels();
			ret.push_back(0);
			glGenTextures(1, &ret.back());
			glBindTexture(GL_TEXTURE_2D, ret.back());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
	else {
		std::cout << "Texture failed to load at path: " << file_path << "\n";
		ret.push_back(0);
	}

	return ret;
}

std::vector<unsigned int> loadGLTextures(std::string file_path, int* width, int* height) {
	std::vector<unsigned int> ret;
	EasyGifReader loader = EasyGifReader::openFile(file_path.c_str());
	ret.reserve(loader.frameCount());
	if (ret.capacity()) {
		*width = loader.width();
		*height = loader.height();
		for (const EasyGifReader::Frame& frame : loader) {
			const std::uint32_t* data = frame.pixels();
			ret.push_back(0);
			glGenTextures(1, &ret.back());
			glBindTexture(GL_TEXTURE_2D, ret.back());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}
	else {
		std::cout << "Texture failed to load at path: " << file_path << "\n";
		ret.push_back(0);
	}

	return ret;
}