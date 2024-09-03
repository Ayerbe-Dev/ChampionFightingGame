#pragma once
#include <iostream>
#include <vector>

unsigned int loadGLTexture(std::string file_path);
unsigned int loadGLTexture(std::string file_path, int* width, int* height);
std::vector<unsigned int> loadGLTextures(std::string file_path);
std::vector<unsigned int> loadGLTextures(std::string file_path, int* width, int* height);