#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

#include <vector>
#include "novox/player.h"
#include "novox/world.h"
#include "novox/world.h"


namespace novox {
	class Game {
	private:
		Player* player;
		world::World* world;
		static std::unordered_map<unsigned int, Game*> instances;
		
	public:
		GLFWwindow* window;
		unsigned int id;

		Game();
		~Game();

		void render();
		void tick();

		static Game* getInstance(unsigned int id);

		void processInput();
		void mouseMoved(double xpos, double ypos);
		void mouseClicked(int button, int action, int mods);
		void mouseScrolled(double xoffset, double yoffset);

	};

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	//void processInput(GLFWwindow* window);
}

