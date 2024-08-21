#ifndef GAME_H
#define GAME_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#define GAME_OBJECT_COUNT 3

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
};
typedef struct snake snake;

struct food{
    float pos_x;
    float pos_y;
    float width;
    float vertices[36];
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
};
typedef struct game game;

enum directional_keys {UP, DOWN, LEFT, RIGHT, STOP};


snake* make_snake_node(float X, float Y);
void add_to_snake(snake *player);
void print_snake(snake *player);
void move_snake(snake *player, float x_speed, float y_speed);
void clear_snake(snake *player);

int game_loop();
void setup_game(snake *player, food *food, game *game);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void process_movement(snake *player, food *food_item, float speed, float resolution_ratio, int *score);
void update_quad_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b);
void update_snake_verticies(snake *player, float r, float g, float b);
int check_quad_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size);
int check_snake_collision(snake *player, food *food);
void randomize_food_coords(food *food, snake *player);


#endif