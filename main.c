#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"


#define TARGET_FPS 60

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

enum render_objects {PLAYER, FOOD, TEXT};

// int resolution_x;
// int resolution_y;
float resolution_ratio;
int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_movement(snake *player, food *food_item, float speed, float resolution_ratio, int *score);
void update_quad_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b);
int check_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size);
void randomize_food_coords(food *food, snake *player);



int main(void){
    GLFWwindow* window;
    snake player;
    food food;
    Character Characters[128];
    const char* fragment_shader_quads_source = load_shader_source("fragment_shader_quads.glsl");
    const char* vertex_shader_quads_source = load_shader_source("vertex_shader_quads.glsl");
    const char* fragment_shader_text_source = load_shader_source("fragment_shader_text.glsl");
    const char* vertex_shader_text_source = load_shader_source("vertex_shader_text.glsl");
    srand(time(NULL));
    int resolution_x = 1024;
    int resolution_y = 768;
    resolution_ratio = (float)resolution_x / (float)resolution_y;
    player.pos_x = 0.0f;
    player.pos_y = 0.0f;
    player.width = 0.05f;
    player.snake_length = 2;
    food.pos_x = 0.0f;
    food.pos_y = 0.0f;
    food.width = 0.01f;
    int score = 0;
    randomize_food_coords(&food, &player);
    
    update_quad_vertices(player.vertices, player.pos_x, player.pos_y, player.width, 0.0, 1.0 , 0.0);
    update_quad_vertices(food.vertices, food.pos_x, food.pos_y, food.width, 1.0, 0.0 , 0.0);
    window = setup_opengl(resolution_x, resolution_y, key_callback);
    load_fonts(Characters, 128, "Montserrat.ttf");
    
    int objects_count = 3;
    GLuint VBO[objects_count], VAO[objects_count];
    
    glGenBuffers(objects_count, VBO);
    glGenVertexArrays(objects_count, VAO);

    setup_quad_vertx_data(VBO[0], VAO[0], player.vertices, sizeof(player.vertices)/sizeof(float));
    setup_quad_vertx_data(VBO[1], VAO[1], food.vertices, sizeof(food.vertices)/sizeof(float));
    setup_text_vertx_data(VBO[2], VAO[2]);

    GLuint shader_program_quads = createShaderProgram(vertex_shader_quads_source, fragment_shader_quads_source);
    GLuint shader_program_text = createShaderProgram(vertex_shader_text_source, fragment_shader_text_source);

    double lasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)){
        process_movement(&player, &food, 0.01, resolution_ratio, &score);
        update_quad_vertices(player.vertices, player.pos_x, player.pos_y, player.width, 0.0, 1.0 , 0.0);
        update_quad_vertices(food.vertices, food.pos_x, food.pos_y, food.width, 1.0, 0.0 , 0.0);
        char score_text[10];
        sprintf(score_text, "%d", score);

        glUseProgram(shader_program_quads);
        glClear(GL_COLOR_BUFFER_BIT);

        //player
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(player.vertices), player.vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        //food
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(food.vertices), food.vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        //text
        // char playx[20];
        // char playy[20];
        // sprintf(playx, "%f", player.pos_x);
        // sprintf(playy, "%f", player.pos_y);
        //RenderText(shader_program_text, playx, 200.0f, 125.0f, 1.0f, color, resolution_x, resolution_y, Characters, VAO[2], VBO[2]);
        //RenderText(shader_program_text, playy, 450.0f, 125.0f, 1.0f, color, resolution_x, resolution_y, Characters, VAO[2], VBO[2]);
        float color[3] = {0.0f, 1.0f, 1.0f};
        RenderText(shader_program_text, "Score: ", 25.0f, 725.0f, 1.0f, color, resolution_x, resolution_y, Characters, VAO[2], VBO[2]);
        RenderText(shader_program_text, score_text, 200.0f, 725.0f, 1.0f, color, resolution_x, resolution_y, Characters, VAO[2], VBO[2]);

        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {
            //
        }
        lasttime += 1.0/TARGET_FPS;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shader_program_quads);

    glfwTerminate();
    return 0;
}


void randomize_food_coords(food *food, snake *player){
    float random_number = (float)rand() / RAND_MAX;
    while (check_collision(player->pos_x, player->pos_y, player->width, food->pos_x, food->pos_y, food->width * resolution_ratio)){
        food->pos_x = -0.7f + random_number * (0.7f - (-0.7f));
        random_number = (float)rand() / RAND_MAX;
        food->pos_y = -0.7f + random_number * (0.7f - (-0.7f));
    }
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      int value;
      if (action == GLFW_PRESS) {
          value = 1;
      } else {
          value = 0;
      }

        switch (key) {
            case GLFW_KEY_UP:
                upPressed = value;
                break;
            case GLFW_KEY_DOWN:
                downPressed = value;
                break;
            case GLFW_KEY_LEFT:
                leftPressed = value;
                break;
            case GLFW_KEY_RIGHT:
                rightPressed = value;
                break;
            case GLFW_KEY_ESCAPE:
                exit(0);
                break;
        }
    }
}

void process_movement(snake *player, food *food_item, float speed, float resolution_ratio, int *score) {
    
    if (upPressed && ((player->pos_y + player->width) * resolution_ratio) < 1.0f) {
        if (!check_collision(player->pos_x, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
        //if (!check_collision(player->pos_x, player->pos_y + speed, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_y += speed;
        } else{
            randomize_food_coords(food_item, player);
            *score += 1;
        }
    }
    if (downPressed && ((player->pos_y - player->width) * resolution_ratio) > -1.0f) {
        if (!check_collision(player->pos_x, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
        //if (!check_collision(player->pos_x, player->pos_y - speed, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_y -= speed;
        } else{
            randomize_food_coords(food_item, player);
            *score += 1;
        }
    }
    if (leftPressed && (player->pos_x - player->width) > -1.0f) {
        if (!check_collision(player->pos_x, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
        //if (!check_collision(player->pos_x - speed, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_x -= speed;
        } else{
            randomize_food_coords(food_item, player);
            *score += 1;
        }
    }
    if (rightPressed && (player->pos_x + player->width) < 1.0f) {
        if (!check_collision(player->pos_x, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
        //if (!check_collision(player->pos_x + speed, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_x += speed;
        } else{
            randomize_food_coords(food_item, player);
            *score += 1;
        }
    }
}



void update_quad_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b) {
    // Vertex 1 (bottom-left)
    vertices[0] = posX - size; vertices[1] = (posY - size) * resolution_ratio; vertices[2] = 0.0f;
    vertices[3] = r; vertices[4] = g; vertices[5] = b;

    // Vertex 2 (bottom-right)
    vertices[6] = posX + size; vertices[7] = (posY - size) * resolution_ratio; vertices[8] = 0.0f;
    vertices[9] = r; vertices[10] = g; vertices[11] = b;

    // Vertex 3 (top-right)
    vertices[12] = posX + size; vertices[13] = (posY + size) * resolution_ratio; vertices[14] = 0.0f;
    vertices[15] = r; vertices[16] = g; vertices[17] = b;

    // Vertex 4 (top-right)
    vertices[18] = posX + size; vertices[19] = (posY + size) * resolution_ratio; vertices[20] = 0.0f;
    vertices[21] = r; vertices[22] = g; vertices[23] = b;

    // Vertex 5 (top-left)
    vertices[24] = posX - size; vertices[25] = (posY + size) * resolution_ratio; vertices[26] = 0.0f;
    vertices[27] = r; vertices[28] = g; vertices[29] = b;

    // Vertex 6 (bottom-left)
    vertices[30] = posX - size; vertices[31] = (posY - size) * resolution_ratio; vertices[32] = 0.0f;
    vertices[33] = r; vertices[34] = g; vertices[35] = b;
}



int check_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size) {
    if (player_x + player_size < food_x - food_size || food_x + food_size < player_x - player_size) {
        return 0;
    }
    if (player_y + player_size < food_y - food_size || food_y + food_size < player_y - player_size) {
        return 0;
    }

    return 1;
}