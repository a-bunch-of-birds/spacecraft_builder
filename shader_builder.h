#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

//#include "GLAD/include/glad/glad.h"
//#include <glfw3.h>
//#include <GLFW Library/include/GLFW/glfw3.h>
//#include "GLFW Library/include/GLFW/glfw3.h"
GLuint LoadShader(const char* vertex_path, const char* fragment_path);
GLuint BuildShader(GLenum eShaderType, const std::string& shaderText);
GLuint LoadShaderwgeom(const char* vertex_path, const char* fragment_path, const char* geometry_path);
GLuint BuildShaderProgram(std::string vertShaderStr, std::string fragShaderStr);
std::string readFile(const char* filePath);