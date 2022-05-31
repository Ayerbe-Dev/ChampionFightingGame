#pragma once
#include <iostream>
#include <glew/glew.h>

unsigned int loadGLTexture(std::string file_path);
unsigned int loadGLTexture(std::string file_path, int* width, int* height);
unsigned int loadGLTexture(std::string file_path, int* width, int* height, int* num_components);