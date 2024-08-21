#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;




GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram();
void setup_vertx_data(GLuint VBO, GLuint VAO, float vertices[], int vertex_count);

#endif