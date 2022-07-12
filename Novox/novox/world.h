#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

#include "novox/util.h"
#include "novox/block.h"
#include "novox/mesh.h"
#include "novox/shader.h"

namespace novox::world {
	class World;
	class Chunk;


	class World {
	private:
		util::Array3D<Chunk> chunks;
	public:
		World(int x_size, int y_size, int z_size);

		Chunk& getChunkAt(int x_coord, int y_coord, int z_coord);
		Chunk& getChunk(int x_loc, int y_loc, int z_loc);

		void updateConnectedChunks(glm::vec3 position);

		WorldVoxel& getVoxel(int x_coord, int y_coord, int z_coord);
		WorldVoxel& getVoxel(const glm::vec3& pos);
		bool checkBounds(const glm::vec3& pos);
	};




	class Chunk {
	private:
		util::Array3D<WorldVoxel> blocks;
		mesh::ChunkMesh* mesh;
		glm::vec3 location;
		
		void meshAddCube(World* world, const WorldVoxel& voxel, const glm::vec3& loc);
		std::array<bool, 6> meshCheckOcclusion(World* world, const glm::vec3& pos);

	public:
		Chunk();
		void draw(shader::Shader& shader);
		void tagForRegen();
		void setLocation(const glm::vec3& location);
		const glm::vec3& getLocation();
		WorldVoxel& getVoxel(int x_loc, int y_loc, int z_loc);
		void generateMesh(World* world);
	};



}