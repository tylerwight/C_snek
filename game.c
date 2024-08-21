#include "game.h"
#include "graphics.h"

#define TARGET_FPS 60
#define OBJECT_COUNT 3
#define PLAYER 0
#define FOOD 1
#define TEXT 2

int resolution_x;
int resolution_y;
float resolution_ratio;
enum directional_keys key_pressed = STOP;
int move = 0;
int score = 0;

int game_loop(){
    GLFWwindow* window;
    snake player;
    food food;
    Character Characters[128];
    game game;
    setup_game(&player, &food, &game);
    window = setup_opengl(resolution_x, resolution_y, key_callback);
    load_fonts(Characters, 128, "Montserrat.ttf");

    GLuint VBO[OBJECT_COUNT], VAO[OBJECT_COUNT];
    glGenBuffers(OBJECT_COUNT, VBO);
    glGenVertexArrays(OBJECT_COUNT, VAO);
    setup_quad_vertx_data(VBO[PLAYER], VAO[PLAYER], player.vertices, sizeof(player.vertices)/sizeof(float));
    setup_quad_vertx_data(VBO[FOOD], VAO[FOOD], food.vertices, sizeof(food.vertices)/sizeof(float));
    setup_text_vertx_data(VBO[TEXT], VAO[TEXT]);
    GLuint shader_program_quads = make_shader_program("vertex_shader_quads.glsl","fragment_shader_quads.glsl");
    GLuint shader_program_text = make_shader_program("vertex_shader_text.glsl","fragment_shader_text.glsl");

    game.last_time = glfwGetTime();
    game.tick_counter = 0;
    while (!glfwWindowShouldClose(window)){ // game loop
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        if (game.tick_counter % 10 == 0){
            process_movement(&player, &food, 0.1, resolution_ratio, &score);
            //print_snake(&player);
        }
        if (game.tick_counter % 100 == 0){
            // printf("\n\n");
            // printf("snake length = %d\n", player.snake_length);
            // print_snake(&player);
            // printf("\n\n");
        }

        //update_quad_vertices(player.vertices, player.pos_x, player.pos_y, player.width, 0.0, 1.0 , 0.0);
        update_snake_verticies(&player, 0.0, 1.0 , 0.0);
        update_quad_vertices(food.vertices, food.pos_x, food.pos_y, food.width, 1.0, 0.0 , 0.0);
        

        char score_text[10];
        sprintf(score_text, "%d", score);
        
        draw_player(VBO[PLAYER], VAO[PLAYER], &player, shader_program_quads);
        draw_food(VBO[FOOD], VAO[FOOD], &food, shader_program_quads);
        draw_debug_text(VBO[TEXT], VAO[TEXT], &player, &food, &game, shader_program_text, Characters);
        float text_color[3] = {0.0f, 1.0f, 1.0f};
        render_text(shader_program_text, "Score: ", 25.0f, 725.0f, 1.0f, text_color, resolution_x, resolution_y, Characters, VAO[TEXT], VBO[TEXT]);
        render_text(shader_program_text, score_text, 200.0f, 725.0f, 1.0f, text_color, resolution_x, resolution_y, Characters, VAO[TEXT], VBO[TEXT]);

        while (glfwGetTime() < game.last_time + 1.0/TARGET_FPS) {
            //do nothing if faster than 60fps until it is FPS
            
        }
        game.last_time += 1.0/TARGET_FPS;
        game.tick_counter++;
        if (game.tick_counter > 1000){
            game.tick_counter = 0;
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shader_program_quads);

    glfwTerminate();
    return 0;
}







void setup_game(snake *player, food *food, game *game){
    srand(time(NULL));
    resolution_x = 1024;
    resolution_y = 768;
    resolution_ratio = (float)resolution_x / (float)resolution_y;
    game->score = 0;

    player->pos_x = 0.05f;
    player->pos_y = 0.0f;
    player->width = 0.05f;
    player->snake_length = 1;
    player->next = NULL;
    food->pos_x = 0.0f;
    food->pos_y = 0.0f;
    food->width = 0.01f;
    randomize_food_coords(food, player);

}

void randomize_food_coords(food *food, snake *player){
    float random_number = (float)rand() / RAND_MAX;
    while (check_snake_collision(player, food)){
        food->pos_x = -0.7f + random_number * (0.7f - (-0.7f));
        random_number = (float)rand() / RAND_MAX;
        food->pos_y = -0.7f + random_number * (0.7f - (-0.7f));
    }
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
      if (action == GLFW_PRESS) {
          move = 1;
      } else {
          move = 0;
      }

        switch (key) {
            case GLFW_KEY_UP:
                if (key_pressed != DOWN){ key_pressed = UP;}
                //key_pressed = UP;
                break;
            case GLFW_KEY_DOWN:
            if (key_pressed != UP){ key_pressed = DOWN;}
                //key_pressed = DOWN;
                break;
            case GLFW_KEY_LEFT:
                if (key_pressed != RIGHT){ key_pressed = LEFT;}
                //key_pressed = LEFT;
                break;
            case GLFW_KEY_RIGHT:
                if (key_pressed != LEFT){ key_pressed = RIGHT;}
                //key_pressed = RIGHT;
                break;
            case GLFW_KEY_ESCAPE:
                exit(0);
                break;
        }
    }
}

void process_movement(snake *player, food *food_item, float speed, float resolution_ratio, int *score) {
    float player_ymax = (player->pos_y + player->width) * resolution_ratio;
    float player_ymin = (player->pos_y - player->width) * resolution_ratio;
    float player_xmax = (player->pos_x + player->width);
    float player_xmin = (player->pos_x - player->width);

    static int wall_buffer = 0;
    static int last_buffer = 0;
    static int count = 0;

    if (check_snake_self_collision(player)){
            player->pos_y = 0;
            player->pos_x = 0.05;
        clear_snake(player);
    }

    if (key_pressed == UP && (player_ymax < 1.0f)) {
        //player->pos_y += speed;
        move_snake(player, 0.0f, speed);
        if (check_snake_collision(player, food_item)) {
            randomize_food_coords(food_item, player);
            *score += 1;  
            player->snake_length += 1;   
            add_to_snake(player);     
        }
    }
    if (key_pressed == DOWN && (player_ymin > -1.0f)) {
        //player->pos_y -= speed;
        move_snake(player, 0.0f, -speed);
        if (check_snake_collision(player, food_item)) {
            randomize_food_coords(food_item, player);
            *score += 1;
            player->snake_length += 1;   
            add_to_snake(player);   
        }
    }
    if (key_pressed == LEFT && (player_xmin > -1.0f)) {
        //player->pos_x -= speed;
        move_snake(player, -speed, 0.0f);
        if (check_snake_collision(player, food_item)) {
            randomize_food_coords(food_item, player);
            *score += 1;
            player->snake_length += 1;   
            add_to_snake(player);   
        }
    }
    if (key_pressed == RIGHT && (player_xmax < 1.0f)) {
        move_snake(player, +speed, 0.0f);
        if (check_snake_collision(player, food_item)) {
            randomize_food_coords(food_item, player);
            *score += 1;
            player->snake_length += 1;   
            add_to_snake(player);   
        }
    }



    if ((player->pos_y > 0.69 && player->pos_y < 0.71) && key_pressed == UP){
        if (wall_buffer >= 1){
            player->pos_y = 0;
            player->pos_x = 0.05;
            wall_buffer = 0;
            clear_snake(player);
        }
        wall_buffer += 1;
        
    }
    if ((player->pos_y < -0.69 && player->pos_y > -0.71) && key_pressed == DOWN){
        if (wall_buffer >= 1){
            player->pos_y = 0;
            player->pos_x = 0.05;
            wall_buffer = 0;
            clear_snake(player);
        }
        wall_buffer += 1;
        
    }
    if ((player->pos_x > 0.94 && player->pos_x < 0.969) && key_pressed == RIGHT){
        if (wall_buffer >= 1){
            player->pos_y = 0;
            player->pos_x = 0.05;
            wall_buffer = 0;
            clear_snake(player);
        }
        wall_buffer += 1;
        
    }
    if ((player->pos_x < -0.94 && player->pos_x > -0.969) && key_pressed == LEFT){
        if (wall_buffer >= 1){
            player->pos_y = 0;
            player->pos_x = 0.05;
            wall_buffer = 0;
            clear_snake(player);
        }
        wall_buffer += 1;
        
    }
    
    if (last_buffer == wall_buffer){
        if (count >= 3){
            wall_buffer = 0;
            count = 0;
        }
        count++;
    }
    last_buffer = wall_buffer;
}

void update_snake_verticies(snake *player, float r, float g, float b){
    if (player == NULL){return;}
    snake *tmp = player;
    while (tmp != NULL){
        update_quad_vertices(tmp->vertices, tmp->pos_x, tmp->pos_y, tmp->width, r, g, b);
        tmp = tmp->next;
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



int check_quad_collision(float player_x, float player_y, float player_size, float food_x, float food_y, float food_size) {
    if (player_x + player_size < food_x - food_size || food_x + food_size < player_x - player_size) {
        return 0;
    }
    if (player_y + player_size < food_y - food_size || food_y + food_size < player_y - player_size) {
        return 0;
    }

    return 1;
}


int check_snake_collision(snake *player, food *food) {
    if (player == NULL){return -1;}

    snake *tmp = player;
    while (tmp != NULL){
        if (check_quad_collision(tmp->pos_x, tmp->pos_y, tmp->width, food->pos_x, food->pos_y, food->width)){
            return 1;
        }
        tmp = tmp->next;
    }

    return 0;
}

int check_snake_self_collision(snake *player) {
    if (player == NULL || player->next == NULL || player->next->next == NULL){return 0;}
    snake *head = player;
    snake *body = player->next->next;
    while (body != NULL){
        if (check_quad_collision(body->pos_x, body->pos_y, body->width-0.01, head->pos_x, head->pos_y, head->width-0.01)){
            return 1;
        }
        body = body->next;
    }

    return 0;
}

snake* make_snake_node(float X, float Y){
    snake *tmp = malloc(sizeof(snake));

    switch (key_pressed) {
        case UP:
            tmp->pos_y = Y - 0.1f;
            tmp->pos_x = X;
            break;
        case DOWN:
            tmp->pos_y = Y + 0.1f;
            tmp->pos_x = X;
            break;
        case LEFT:
            tmp->pos_x = X + 0.1f;
            tmp->pos_y = Y;
            break;
        case RIGHT:
            tmp->pos_x = X - 0.1f;
            tmp->pos_y = Y;
            break;
        default:
            tmp->pos_x = X - 0.1f;
            tmp->pos_y = Y - 0.1f;
            break;

    }

    tmp->width = 0.05f;
    tmp->next = NULL;
    return tmp;
}
void clear_snake(snake *player){
    if (player == NULL){return;}
    player->snake_length = 1;
    key_pressed = STOP;
    score = 0;
    if (player->next == NULL){return;}

    
    snake *tmp = player->next;// set tmp to 2nd segment of snake to keep track of it
    player->next = NULL;// set head of snake to point to nothing (making it a 1 segment snake)
    while (tmp !=NULL){ // free the memory for whole snake from 2nd segmant forawrd
        snake *tmp_forward = tmp->next;
        free(tmp);
        tmp = tmp_forward;
    }
}

void add_to_snake(snake *player){
    snake *tmp = player;
    while (tmp->next != NULL){
        tmp = tmp->next;
    }

    snake *newnode = make_snake_node(tmp->pos_x, tmp->pos_y);
    tmp->next = newnode;
}


void print_snake(snake *player){
    snake *tmp = player;
    int count = 1;
    if (player == NULL){
        return;
    }
    while (tmp != NULL){
        // printf("SNAKE #%d\n", count);
        // printf("X %f\n Y %f\n W %f\n", tmp->pos_x, tmp->pos_y, tmp->width);
        count++;
        tmp = tmp->next;
    }
}

void move_snake(snake *player, float x_speed, float y_speed){
    if (player == NULL){return;}
    float prev_x = player->pos_x;
    float prev_y = player->pos_y;

    player->pos_x += x_speed;
    player->pos_y += y_speed;
    snake *tmp = player->next;

    while (tmp != NULL){
        float tmp_x = tmp->pos_x;
        float tmp_y = tmp->pos_y;
        tmp->pos_x = prev_x;
        tmp->pos_y = prev_y;
        prev_x = tmp_x;
        prev_y = tmp_y;
        tmp = tmp->next;
    }

}