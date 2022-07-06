#pragma once
#ifndef GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

namespace game {
	void init();

	void render();

	void tick();
	
	void close();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void processInput(GLFWwindow* window);
}

#endif // GAME