#define FMT_HEADER_ONLY
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/core.h>
#include <windows.h>
#include <functional>

#include <stb_image.h>

#include "novox/game.h"
#include "novox/rendering.h"
#include "novox/player.h"
#include "novox/block.h"
#include "novox/world.h"
#include "novox/mesh.h"
#include "novox/util.h"

#define WORLD_SIZE 2


namespace novox {

	glm::vec3 rayStartPos(0.0f);
	glm::vec3 rayEndPos(0.0f);
	std::vector<glm::ivec3> rayIntersections;

	using namespace rendering;

	int window_width = 800;
	int window_height = 600;

	float deltaTime = 0.0;
	float lastFrame = 0.0;


	std::unordered_map<unsigned int, Game*> Game::instances;

	Game::Game() {
		static unsigned int id = 0;
		Game::instances[id] = this;
		this->id = id++;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->window = glfwCreateWindow(window_width, window_height, "Novox", NULL, NULL);
		if (window == NULL) {
			fmt::print("Failed to create GLFW window");
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		//glfwSetKeyCallback(window, key_callback);
		GLFWimage images[1];
		images[0].pixels = stbi_load("novox2.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("Failed to initialize GLAD");
		}


		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
			Game* game = Game::getInstance(0);
			game->mouseMoved(xpos, ypos);

		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			Game* game= Game::getInstance(0);
			game->mouseClicked(button, action, mods);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* widnow, double xoffset, double yoffset) {
			Game* game = Game::getInstance(0);
			game->mouseScrolled(xoffset, yoffset);
		});
		
		world::initializeBlocks();
		this->world = new world::World(WORLD_SIZE, WORLD_SIZE, WORLD_SIZE);
		this->player = new Player();
		


		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);


		

	}
	Game::~Game() {
		free(this->window);
		delete this->player;

	}

	Game* Game::getInstance(unsigned int id) {
		return instances[id];
	}


	void Game::render() {
		static Shader lightingShader("shaders/LightingShader.vert", "shaders/LightingShader.frag");
		static Shader lightSourceShader("shaders/LightSource.vert", "shaders/LightSource.frag");

		static float cube_vertices[] = {
			// vertices				// Texture Coords	// Normal Vectors
			// Back Face
			0, 0, 0,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// Bottom-left
			1, 1, 0,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
			1, 0, 0,	1.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-right       
			1, 1, 0,	1.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-right
			0, 0, 0,	0.0f, 0.0f,		0.0f,  0.0f, -1.0f,		// bottom-left
			0, 1, 0,	0.0f, 1.0f,		0.0f,  0.0f, -1.0f,		// top-left
			// Front face					   			  
			0, 0, 1,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
			1, 0, 1,	1.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-right
			1, 1, 1,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
			1, 1, 1,	1.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-right
			0, 1, 1,	0.0f, 1.0f,		0.0f,  0.0f,  1.0f,		// top-left
			0, 0, 1,	0.0f, 0.0f,		0.0f,  0.0f,  1.0f,		// bottom-left
			// Left face								  
			0, 1, 1,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
			0, 1, 0,	1.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// top-left
			0, 0, 0,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
			0, 0, 0,	0.0f, 1.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-left
			0, 0, 1,	0.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// bottom-right
			0, 1, 1,	1.0f, 0.0f,	   -1.0f,  0.0f,  0.0f,		// top-right
			// Right face	  		 	   			   	  
			1, 1, 1,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
			1, 0, 0,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
			1, 1, 0,	1.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// top-right         
			1, 0, 0,	0.0f, 1.0f,		1.0f,  0.0f,  0.0f,		// bottom-right
			1, 1, 1,	1.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// top-left
			1, 0, 1,	0.0f, 0.0f,		1.0f,  0.0f,  0.0f,		// bottom-left     
			// Bottom face	  		 	   			   	  
			0, 0, 0,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
			1, 0, 0,	1.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-left
			1, 0, 1,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
			1, 0, 1,	1.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-left
			0, 0, 1,	0.0f, 0.0f,		0.0f, -1.0f,  0.0f,		// bottom-right
			0, 0, 0,	0.0f, 1.0f,		0.0f, -1.0f,  0.0f,		// top-right
			// Top face				  		   		  
			0, 1, 0,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
			1, 1, 1,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
			1, 1, 0,	1.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-right     
			1, 1, 1,	1.0f, 0.0f,		0.0f,  1.0f,  0.0f,		// bottom-right
			0, 1, 0,	0.0f, 1.0f,		0.0f,  1.0f,  0.0f,		// top-left
			0, 1, 1,	0.0f, 0.0f,		0.0f,  1.0f,  0.0f 		// bottom-left        
		};

		static unsigned int selectionVAO = []()mutable ->unsigned int  {
			unsigned int selectionVAO, VBO, EBO;
			glGenVertexArrays(1, &selectionVAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(selectionVAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

			/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

			//vertex position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//vertex texture coordinates
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);

			//safe to unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//The VAO can also be unbound to prevent other VAO calls from modifying the VAO, doesn't really happen though
			//In order to modify another VAO you need to call glBIndVertexArray so there isn't much reason to unbind the VAO
			glBindVertexArray(0);
			return selectionVAO;
		}();
		
		

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		static int frame_count = 0;
		frame_count++;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		lightingShader.use();

		glm::mat4 view = this->player->getView();


		static auto do_once = [&]() -> bool {
			fmt::print("{}\n", glm::to_string(view));
			return true;
		}();



		glm::mat4 projection = glm::perspective(glm::radians(80.0f), ((float)window_width / (float)window_height), 0.1f, 500.0f);

		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		lightingShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
		lightingShader.setVec3("lightPos", glm::vec3(WORLD_SIZE * 16, WORLD_SIZE * 16 + 8, WORLD_SIZE * 16));
		lightingShader.setVec3("viewPos", glm::vec3(0, 0, 0));

		for (int x = 0; x < WORLD_SIZE; x++) {
			for (int y = 0; y < WORLD_SIZE; y++) {
				for (int z = 0; z < WORLD_SIZE; z++) {
					


					world::Chunk& chunk = this->world->getChunk(x, y, z);
					chunk.generateMesh(this->world);
					

					chunk.draw(lightingShader);

					


				}
			}
		}

		glm::mat4 model = glm::mat4(1.0f);
		auto sel = this->player->selectionPos;

		model = glm::translate(model, glm::vec3(sel));
		lightingShader.use();
		lightingShader.setMat4("model", model);
		lightingShader.setVec3("objectColor", glm::vec3(0.0));

		glDisable(GL_CULL_FACE);





		if (this->player->blockSelected) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(4);
			glBindVertexArray(selectionVAO);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			/*lightingShader.use();
			model = glm::translate(glm::mat4(1.0f), glm::floor(rayStartPos));
			lightingShader.setMat4("model", model);
			lightingShader.setVec3("objectColor", glm::vec3(0.0, 1.0, 0.0));
			glDrawArrays(GL_TRIANGLES, 0, 36);*/

			

			/*lightingShader.use();
			model = glm::translate(glm::mat4(1.0f), glm::floor(rayEndPos));
			lightingShader.setMat4("model", model);
			lightingShader.setVec3("objectColor", glm::vec3(1.0, 0.0, 0.0));
			glDrawArrays(GL_TRIANGLES, 0, 36);*/

			glLineWidth(1);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (auto const& pos : rayIntersections) {
			lightingShader.use();
			model = glm::translate(glm::mat4(1.0f), glm::vec3(pos));
			lightingShader.setMat4("model", model);
			lightingShader.setVec3("objectColor", glm::vec3(0.0, 0.0, 0.0));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glEnable(GL_CULL_FACE);
		//fmt::print(stderr, "rendered chunks\n");
		
		
		glfwSwapBuffers(this->window);
		glfwPollEvents();

		static float deltaSec = 0.0;
		//fmt::print(stderr, "lastsec {}\n", deltaSec);
		deltaSec += deltaTime;
		if (deltaSec >= 1.0) {
			glfwSetWindowTitle(this->window, fmt::format("Novox: {} FPS", frame_count).c_str());
			frame_count = 0;
			deltaSec = 0.0;
		}



	}

	void Game::tick() {
		processInput();
		
		// Player selection processing
		auto playerCameraPos = this->player->getCamera()->position;
		auto playerCameraDir = this->player->getCamera()->front;
		float maxSelectionDistance = 8;

		bool blockSelected = false;

		rayStartPos = playerCameraPos;
		rayEndPos = playerCameraPos + playerCameraDir * maxSelectionDistance;

		auto playerSelectedVoxels = this->world->castVoxelRay(playerCameraPos, playerCameraPos + playerCameraDir * maxSelectionDistance);
		//auto playerSelectedVoxels = this->world->castVoxelRay(glm::vec3(0.0, 0.0, 0.5), glm::vec3(32.0,32.0,0.5));
		rayIntersections.clear();
		for (const auto& [voxelPos, voxel] : playerSelectedVoxels) {
			rayIntersections.push_back(voxelPos);
		}

		for (const auto& [voxelPos, voxel] : playerSelectedVoxels) {
			
			fmt::print("<{:02}, {:02}, {:02}>\t:\t[ {} ]\t[{:02}]\n", voxelPos.x, voxelPos.y, voxelPos.z, voxel.block->block_id, playerSelectedVoxels.size());
			if (voxel.block->block_id != 0) {
				this->player->selectionPos = voxelPos;
				blockSelected = true;
				break;
			}
		}
		this->player->blockSelected = blockSelected;

	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		window_width = width;
		window_height = height;
		glViewport(0, 0, width, height);
	}

	void Game::processInput()
	{
		auto speed = this->player->speed() * deltaTime * 2;

		//Uses the Esc key to close the window
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)		//forward
		{
			this->player->displaceRelative(0, 0, speed);

		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)		//backwward
		{
			this->player->displaceRelative(180, 0, speed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)		//left
		{
			this->player->displaceRelative(-90, 0, speed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)		//right
		{
			this->player->displaceRelative(90, 0, speed);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)		// up
		{
			this->player->displaceRelative(0, 90, speed);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)		// down
		{
			this->player->displaceRelative(0, -90, speed);
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			auto sel = this->player->selectionPos;
			if (!this->world->checkBounds(sel)) return;
			auto chunkloc = this->world->getChunkAt(sel.x, sel.y, sel.z).getLocation();
			fmt::print("selection: {:02}, {:02}, {:02}\t chunkloc {:02},{:02},{:02}\r", sel.x, sel.y, sel.z, chunkloc.x, chunkloc.y, chunkloc.z);
		}

		//fmt::print("pos: {}, {}, {}\r", this->player->getPos().x, this->player->getPos().y, this->player->getPos().z);
		
	}

	void Game::mouseMoved(double xpos, double ypos) {
		static float lastX = 0.0;
		static float lastY = 0.0;
		const float sensitivity = 0.05f;


		float deltaX = xpos - lastX;
		float deltaY = -1 * (ypos - lastY);
		lastX = xpos;
		lastY = ypos;



		auto pitch = &this->player->getCamera()->pitch;
		auto yaw = &this->player->getCamera()->yaw;

		if (*pitch > 89.0f)
			*pitch = 89.0f;
		if (*pitch < -89.0f)
			*pitch = -89.0f;


		//glfwSetWindowTitle(this->window, fmt::format("Novox: {}, {}", *yaw, *pitch).c_str());

		this->player->lookTowards(deltaX * sensitivity, deltaY * sensitivity);

	}

	void Game::mouseClicked(int button, int action, int mods) {
		glm::ivec3 selpos = this->player->selectionPos;
		if (!this->world->checkBounds(selpos)){
			return;
		}



		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			world::WorldVoxel& voxel = this->world->getVoxel(selpos);
			voxel.block = world::Block::defaultBlocks[util::as_int(world::BLOCK::air)];

			world::Chunk& chunk = this->world->getChunkAt(selpos.x, selpos.y, selpos.z);
			this->world->updateChunkMesh(chunk, selpos);
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			world::WorldVoxel& voxel = this->world->getVoxel(selpos);
			voxel.block = world::Block::defaultBlocks[util::as_int(world::BLOCK::stone)];

			world::Chunk& chunk = this->world->getChunkAt(selpos.x, selpos.y, selpos.z);
			this->world->updateChunkMesh(chunk, selpos);
		}
	}

	void Game::mouseScrolled(double xoffset, double yoffset) {

	}
	

}
