#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 130\n"
    "in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 130\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0, 1.0, 0.2, 1.0);\n"
    "}\0";

// Function to compile a shader
GLuint compileShader(GLenum type, const char* source){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return shader;
}

// Function to create a shader program
GLuint createShaderProgram(){
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Variables to store the quad's position
float quadPosX = 0.0f;
float quadPosY = 0.0f;
// Variables to track key states
int upPressed = 0;
int downPressed = 0;
int leftPressed = 0;
int rightPressed = 0;

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
// Function to update vertex data
void updateVertices(float vertices[], float posX, float posY){
    float size = 0.2f; // Half size of the quad
    vertices[0] = posX - size; vertices[1] = posY - size; vertices[2] = 0.0f;
    vertices[3] = posX + size; vertices[4] = posY - size; vertices[5] = 0.0f;
    vertices[6] = posX + size; vertices[7] = posY + size; vertices[8] = 0.0f;
    vertices[9] = posX + size; vertices[10] = posY + size; vertices[11] = 0.0f;
    vertices[12] = posX - size; vertices[13] = posY + size; vertices[14] = 0.0f;
    vertices[15] = posX - size; vertices[16] = posY - size; vertices[17] = 0.0f;
}

// Function to process input
void processInput(GLFWwindow* window, float speed){
    //float speed = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        quadPosY += speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        quadPosY -= speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        quadPosX -= speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        quadPosX += speed;
}

void processMovement(){
    float speed = 0.001f;
    if (upPressed){ quadPosY += speed;} 
    if (downPressed){ quadPosY -= speed;}
    if (leftPressed){ quadPosX -= speed;}
    if (rightPressed){ quadPosX += speed;}
}

int main(void){
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
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
    updateVertices(vertices, quadPosX, quadPosY);

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
        processMovement();

        updateVertices(vertices, quadPosX, quadPosY);
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
