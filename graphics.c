#include "graphics.h"

// const char* vertexShaderSource = "#version 130\n"
//     "in vec3 aPos;\n"
//     "in vec3 aColor;\n"
//     "out vec3 ourColor;\n"
//     "void main()\n"
//     "{\n"
//     "    gl_Position = vec4(aPos, 1.0);\n"
//     "    ourColor = aColor;\n"
//     "}\0";

// const char* fragmentShaderSource = "#version 130\n"
//     "in vec3 ourColor;\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "    FragColor = vec4(ourColor, 1.0);\n"
//     "}\0";
const char* vert_shader_quads_source;
const char* vert_shader_text_source;
const char* frag_shader_quads_source;
const char* frag_shader_text_source;


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

GLuint createShaderProgram(const char* vertex_shader_source, const char* fragment_shader_source){
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment_shader_source);

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


void setup_vertx_data(GLuint VBO, GLuint VAO, float vertices[], int vertex_count) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


const char* load_shader_source(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", file_path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "Could not allocate memory for file: %s\n", file_path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}


void load_fonts(Character *Characters, int Characters_length){
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init FreeType Library\n");
        exit(EXIT_FAILURE);
    }

    FT_Face face;
    if (FT_New_Face(ft, "Montserrat.ttf", 0, &face)) {
        fprintf(stderr, "Failed to load font\n");
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load Glyph\n");
            continue;
        }
        
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x
        };
        Characters[c] = character;
        //printf("glyh: %c bitmap width: %d, bitmap pitch: %d\n", c, face->glyph->bitmap.width, face->glyph->bitmap.pitch);
        //printf("loaded glyph %c, size: %d - %d, bearing: %d - %d, advance: %ld\n", c, face->glyph->bitmap.width, face->glyph->bitmap.rows,face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}