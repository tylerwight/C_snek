#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

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

int resolution_x;
int resolution_y;
float resolution_ratio;
int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_movement(snake *player_snake, food *food_item, float speed, float resolution_ratio);
void update_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b);
int check_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size);
void randomize_food_coords(food *food, snake *player);

GLFWwindow* setup_opengl();


int main(void){
    GLFWwindow* window;
    snake player;
    food food;
    srand(time(NULL));
    resolution_x = 1024;
    resolution_y = 768;
    resolution_ratio = (float)resolution_x / (float)resolution_y;
    player.pos_x = 0.0f;
    player.pos_y = 0.0f;
    player.width = 0.05f;
    food.pos_x = 0.0f;
    food.pos_y = 0.0f;
    food.width = 0.01f;
    randomize_food_coords(&food, &player);
    printf(" food_x: %f, food_y: %f\n", food.pos_x, food.pos_y);
    
    update_vertices(player.vertices, player.pos_x, player.pos_y, player.width, 0.0, 1.0 , 0.0);
    update_vertices(food.vertices, food.pos_x, food.pos_y, food.width, 1.0, 0.0 , 0.0);


    window = setup_opengl();

    int objects_count = 2;
    GLuint VBO[objects_count], VAO[objects_count];
    glGenBuffers(objects_count, VBO);
    glGenVertexArrays(objects_count, VAO);

    setup_vertx_data(VBO[0], VAO[0], player.vertices, sizeof(player.vertices)/sizeof(float));
    setup_vertx_data(VBO[1], VAO[1], food.vertices, sizeof(food.vertices)/sizeof(float));

    GLuint shaderProgram = createShaderProgram();

    while (!glfwWindowShouldClose(window)){
        process_movement(&player, &food, 0.002, resolution_ratio);

        update_vertices(player.vertices, player.pos_x, player.pos_y, player.width, 0.0, 1.0 , 0.0);
        update_vertices(food.vertices, food.pos_x, food.pos_y, food.width, 1.0, 0.0 , 0.0);

        glUseProgram(shaderProgram);
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



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

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

void process_movement(snake *player, food *food_item, float speed, float resolution_ratio) {
    
    if (upPressed && ((player->pos_y + player->width) * resolution_ratio) < 1.0f) {
        if (!check_collision(player->pos_x, player->pos_y + speed, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_y += speed;
        } else{
            randomize_food_coords(food_item, player);
        }
    }
    if (downPressed && ((player->pos_y - player->width) * resolution_ratio) > -1.0f) {
        if (!check_collision(player->pos_x, player->pos_y - speed, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_y -= speed;
        } else{
            randomize_food_coords(food_item, player);
        }
    }
    if (leftPressed && (player->pos_x - player->width) > -1.0f) {
        if (!check_collision(player->pos_x - speed, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_x -= speed;
        } else{
            randomize_food_coords(food_item, player);
        }
    }
    if (rightPressed && (player->pos_x + player->width) < 1.0f) {
        if (!check_collision(player->pos_x + speed, player->pos_y, player->width, food_item->pos_x, food_item->pos_y, food_item->width)) {
            player->pos_x += speed;
        } else{
            randomize_food_coords(food_item, player);
        }
    }
}



void update_vertices(float vertices[], float posX, float posY, float size, float r, float g, float b) {
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




GLFWwindow* setup_opengl(){
    GLFWwindow* window;
    if (!glfwInit()){exit(-1);}

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(resolution_x, resolution_y, "Snek", NULL, NULL);
        if (!window){
            glfwTerminate();
            exit(-1);
        }

    glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
        {
            printf("Failed to initialize GLEW\n");
            exit(-1);
        }

    glfwSetKeyCallback(window, key_callback);

    return window;
    
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