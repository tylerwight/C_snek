#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"



struct snake{
    float pos_x;
    float pos_y;
    float width;
    int snake_length;

};
typedef struct snake snake;

struct food{
    float pos_x;
    float pos_y;
    float width;
};


typedef struct food food;

int resolution_x;
int resolution_y;
float resolution_ratio;
int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processMovement(float *X, float *Y, float speed, float size);
void updateVertices(float vertices[], float posX, float posY, float size);

void setup_opengl(){

    
}


int main(void){
    GLFWwindow* window;
    snake player;
    food food;
    resolution_x = 1024;
    resolution_y = 768;
    resolution_ratio = (float)resolution_x / (float)resolution_y;
    player.pos_x = 0.0f;
    player.pos_y = 0.0f;
    player.width = 0.05f;
    food.pos_x = 0.0f;
    food.pos_y = 0.0f;
    printf("resolution ratio= %f, resx = %d, resy = %d", resolution_ratio, resolution_x, resolution_y);


    if (!glfwInit()){return -1;}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(resolution_x, resolution_y, "Snek", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    float vertices[18];
    updateVertices(vertices, player.pos_x, player.pos_y, player.width);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram();

    while (!glfwWindowShouldClose(window))
    {
        processMovement(&player.pos_x, &player.pos_y, 0.001, player.width);

        updateVertices(vertices, player.pos_x, player.pos_y, player.width);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
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
        }
    }
}


void processMovement(float *X, float *Y, float speed, float size){    
    if (upPressed && ((*Y + size) * resolution_ratio) < 1.0f) {
        *Y += speed;
    }
    if (downPressed && ((*Y - size) * resolution_ratio) > -1.0f) {
        *Y -= speed;
    }
    if (leftPressed && (*X - size) > -1.0f) {
        *X -= speed;
    }
    if (rightPressed && (*X + size) < 1.0f) {
        *X += speed;
    }
}

void updateVertices(float vertices[], float posX, float posY, float size){
    //float size = 0.2f; // Half size of the quad
    vertices[0] = posX - size; vertices[1] = (posY - size) * resolution_ratio; vertices[2] = 0.0f;
    vertices[3] = posX + size; vertices[4] = (posY - size) * resolution_ratio; vertices[5] = 0.0f;
    vertices[6] = posX + size; vertices[7] = (posY + size) * resolution_ratio; vertices[8] = 0.0f;
    vertices[9] = posX + size; vertices[10] = (posY + size) * resolution_ratio; vertices[11] = 0.0f;
    vertices[12] = posX - size; vertices[13] = (posY + size) * resolution_ratio; vertices[14] = 0.0f;
    vertices[15] = posX - size; vertices[16] = (posY - size) * resolution_ratio; vertices[17] = 0.0f;
}