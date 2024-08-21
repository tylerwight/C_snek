#include "graphics.h"

const char* vertexShaderSource = "#version 130\n"
    "in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

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
