#pragma once
#include <iostream>
#include <glew/glew.h>

unsigned int loadGLTexture(char const* file_path);
unsigned int loadGLTexture(char const* file_path, int* width, int* height);
unsigned int loadGLTextureFromFile(const char* path, const std::string & directory, bool gamma = false);