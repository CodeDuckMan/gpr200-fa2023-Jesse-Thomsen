#pragma once
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

unsigned int createVAO(float* vertexData, int arrayLength, int numVertices);

unsigned int createShader(GLenum shaderType, const char* sourceCode);

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);