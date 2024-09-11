#ifndef GAME_H
#define GAME_H
#define GLEW_STATIC

#include "stb_image.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

 #define GAME_OBJECT_COUNT 3


enum direction {UP, DOWN, LEFT, RIGHT, STOP};


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
    struct snake *next;
    GLuint texture_head;
    GLuint texture_body1;
    GLuint texture_body2;
    enum direction facing;
};
typedef struct snake snake;

struct food{
    float pos_x;
    float pos_y;
    float width;
    float vertices[36];
    GLuint texture;
};
typedef struct food food;



struct game{
    int resolution_x;
    int resolution_y;
    float resolution_ratio;
    int score;
    int tick_counter;
    double last_time;
    GLuint VBO[GAME_OBJECT_COUNT];
    GLuint VAO[GAME_OBJECT_COUNT];
    Character Characters[128];
    GLFWwindow* window;
    GLuint shader_program_text;
    GLuint shader_program_quads;
    enum direction key_pressed;
    ALuint song;
    ALuint eat_sound;
    ALuint die_sound;
    
};
typedef struct game game;




snake* make_snake_node(float X, float Y, game *game);
void add_to_snake(snake *player, game *game);
void print_snake(snake *player);
void move_snake(snake *player, float x_speed, float y_speed);
void clear_snake(snake *player, game *game);

int game_loop();
void setup_game(snake *player, food *food, game *game);
void load_textures(snake *player, food *food, game *game);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void process_movement(snake *player, food *food_item, float speed, game *game);
void update_quad_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b, game *game);
void update_snake_verticies(snake *player, float r, float g, float b, game *game);
void rotate_snake_texture(snake *player, game *game);
void update_food_verticies(food *food, float r, float g, float b, game *game);

int check_quad_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size);
int check_snake_collision(snake *player, food *food);
int check_snake_self_collision(snake *player);
void randomize_food_coords(food *food, snake *player);


init_audio();
ALuint create_audio(char *filename);


#endif