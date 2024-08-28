#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "game.h"

int main(){
    game_loop();
}
