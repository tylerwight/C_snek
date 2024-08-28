#ifndef GRAPHICS_H
#define GRAPHICS_H
#define GLEW_STATIC
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
#include "game.h"

#define DEBUG 0
extern int resolution_x;
extern int resolution_y;
extern float resolution_ratio;
//tmp


GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source);
void setup_quad_vertx_data(GLuint VBO, GLuint VAO, float vertices[], int vertex_count);
const char* load_shader_source(const char* file_path);
void load_fonts(Character *Characters, int Characters_length, const char *font_name);
GLFWwindow* setup_opengl(int resolution_x, int resolution_y, void (*key_callback)(GLFWwindow*, int, int, int, int) );
void setup_text_vertx_data(GLuint VBO, GLuint VAO);
//void render_text(GLuint shaderProgram, const char* text, float x, float y, float scale, float color[3], int resolution_x, int resolution_y, Character Characters[], GLuint VAO, GLuint VBO);
void render_text(const char* text, float x, float y, float scale, float color[3], game *game);
//void draw_player(GLuint VBO, GLuint VAO, snake *player, GLuint shaderProgram);
void draw_player(snake *player, game *game);
//void draw_food(GLuint VBO, GLuint VAO, food *food, GLuint shaderProgram);
void draw_food(food *food, game *game);
void draw_debug_text(snake *player, food *food, game *game);
GLuint make_shader_program(const char vertex_source[], const char fragment_source[]); 
#endif