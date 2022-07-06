#define FMT_HEADER_ONLY

#include "novox/game.h"
#include "novox/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fmt/core.h>


namespace game {
	using namespace shader;

	int window_width = 800;
	int window_height = 600;
	


	void init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Novox", NULL, NULL);
		if (window == NULL) {
			fmt::print("Failed to create GLFW window");
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		//glfwSetKeyCallback(window, key_callback);


		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Failed to initialize GLAD");
		}

		Shader lightingShader("shaders/LightingShader.vs.glsl", "shaders/LightingShader.fs.glsl");
		Shader lightSourceShader("shaders/LightSource.vs.glsl", "shaders/LightSource.fs.glsl");



	}

	void render() {

	}

	void tick() {

	}

	void close() {

	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		window_width = width;
		window_height = height;
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow* window)
	{
		//Uses the Esc key to close the window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)		//forward
		{

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)		//backwward
		{

		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)		//left
		{

		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)		//right
		{

		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)		// up
		{

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)		// down
		{

		}


	}


}
