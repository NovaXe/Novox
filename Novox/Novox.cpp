#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

#include "novox/game.h"


int main()
{
    fmt::print("Novox has started\n");

    novox::Game* game = new novox::Game();
    
    

    // TODO: Leave all openGL stuff inside the game namespace and handle the window myself

    //GLFWwindow* gameWindow = glfwGetCurrentContext();

    while (!glfwWindowShouldClose(game->window)) {

        game->tick();
        game->render();

    }

    return 0;
}


