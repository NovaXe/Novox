#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

#include "novox/util.h"
#include "novox/block.h"
#include "novox/mesh.h"
#include "novox/rendering.h"

namespace novox::world {
	class World;
	class Chunk;


	class World {
	private:
		util::Array3D<Chunk> chunks;
	public:
		World(int x_size, int y_size, int z_size);
		int x_size;
		int y_size;
		int z_size;

		Chunk& getChunkAt(int x_coord, int y_coord, int z_coord);
		Chunk& getChunk(int x_loc, int y_loc, int z_loc);

		void updateChunkMesh(Chunk& chunk, const glm::ivec3& position);

		WorldVoxel& getVoxel(int x_coord, int y_coord, int z_coord);
		WorldVoxel& getVoxel(const glm::vec3& pos);
		bool checkBounds(const glm::vec3& pos);
		std::vector<std::pair<glm::ivec3, WorldVoxel>> castVoxelRay(const glm::vec3& start, const glm::vec3& end);
	};




	class Chunk {
	private:
		util::Array3D<WorldVoxel> blocks;
		mesh::ChunkMesh* mesh;
		glm::ivec3 location;
		
		void meshAddCube(World* world, const WorldVoxel& voxel, const glm::ivec3& loc);
		std::array<bool, 6> meshCheckOcclusion(World* world, const glm::ivec3& pos);

	public:
		Chunk();
		void draw(rendering::Shader& shader);
		void tagForRegen();
		void setLocation(const glm::ivec3& location);
		const glm::ivec3& getLocation();
		WorldVoxel& getVoxel(int x_loc, int y_loc, int z_loc);
		void generateMesh(World* world);
	};



}