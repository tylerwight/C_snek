#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h> 
#include <cglm/cam.h> 
#include <cglm/affine.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char* vert_shader_quads_source;
extern const char* vert_shader_text_source;
extern const char* frag_shader_quads_source;
extern const char* frag_shader_text_source;

typedef struct {
    GLuint TextureID;
    int Size[2];
    int Bearing[2];
    unsigned int Advance;
} Character;



GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source);
void setup_quad_vertx_data(GLuint VBO, GLuint VAO, float vertices[], int vertex_count);
const char* load_shader_source(const char* file_path);
void load_fonts(Character *Characters, int Characters_length, const char *font_name);
GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) );
void setup_text_vertx_data(GLuint VBO, GLuint VAO);
void RenderText(GLuint shaderProgram, const char* text, float x, float y, float scale, float color[3], int resolution_x, int resolution_y, Character Characters[], GLuint VAO, GLuint VBO);

#endif