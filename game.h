#ifndef GAME_H
#define GAME_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLuint TextureID;
    int Size[2];
    int Bearing[2];
    unsigned int Advance;
} Character;

struct snake{
    float pos_x;
    float pos_y;
    float width;
    int snake_length;
    float vertices[36];

};
typedef struct snake snake;

struct food{
    float pos_x;
    float pos_y;
    float width;
    float vertices[36];
};
typedef struct food food;


#endif