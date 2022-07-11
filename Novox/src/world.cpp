#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <windows.h>
#include <fmt/core.h>

#include <array>
//#include <cmath>
#include <stdexcept>

#include "novox/world.h"
#include "novox/block.h"
#include "novox/util.h"
#include "novox/shader.h"

namespace novox::world {
	

	World::World(int x_size, int y_size, int z_size) : chunks(x_size, y_size, z_size) {
		for (int x = 0; x < x_size; x++) {
			for (int y = 0; y < y_size; y++) {
				for (int z = 0; z < z_size; z++) {
					Chunk& chunk = this->chunks.at(x, y, z);
					chunk.setLocation(glm::ivec3(x, y, z));
					//fmt::print("chunk location set: {},{},{}\t\t", x, y, z);
					//auto newloc = chunk.getLocation();
					//fmt::print("chunk new location: {},{},{}\n", newloc.x, newloc.y, newloc.z);

				}
			}
		}

		fmt::print("Generated world of size {}x{}x{}\n", x_size, y_size, z_size);
	}

	Chunk& World::getChunkAt(int x_coord, int y_coord, int z_coord)
	{
		Chunk& chunk = this->chunks.at(floor(x_coord / 16), floor(y_coord / 16), floor(z_coord / 16));
		return chunk;
	}

	Chunk& World::getChunk(int x_loc, int y_loc, int z_loc)
	{
		Chunk& chunk = this->chunks.at(x_loc, y_loc, z_loc);
		return chunk;
	}




	WorldVoxel& World::getVoxel(int x_coord, int y_coord, int z_coord)
	{
		Chunk& chunk = getChunkAt(x_coord, y_coord, z_coord);
 		WorldVoxel& voxel = chunk.getVoxel(x_coord % 16, y_coord % 16, z_coord % 16);
		return voxel;
	}
	WorldVoxel& World::getVoxel(const glm::vec3& pos)
	{
		return getVoxel(pos.x, pos.y, pos.z);
	}


	bool World::checkBounds(const glm::vec3& pos)
	{
		if (pos.x < 0 || pos.y < 0 || pos.z < 0) {
			return false;
		}
		else {
			return chunks.inRange(pos.x / 16, pos.y / 16, pos.z / 16);
		}
	}

	
	

	Chunk::Chunk() : blocks(16, 16, 16) {
		//fmt::print("Generated chunk of size {}x{}x{}\n", 16, 16, 16);
		this->mesh = new mesh::ChunkMesh();
		this->location = glm::ivec3(0);
	}
	
	void Chunk::setLocation(const glm::vec3& location)
	{
		this->location = location;
	}
	const glm::vec3& Chunk::getLocation()
	{
		return this->location;
	}
	WorldVoxel& Chunk::getVoxel(int x_loc, int y_loc, int z_loc)
	{
		WorldVoxel& voxel = blocks.at(x_loc, y_loc, z_loc);
		return voxel;
	}
	


	void Chunk::draw(shader::Shader& shader)
	{
		this->mesh->draw(shader, this->location);
	}

	void Chunk::tagForRegen() {
		this->mesh->needsRegen = true;
	}

	void Chunk::generateMesh(World* world)
	{
		if (this->mesh->needsRegen) {
			for (int x = 0; x < 16; x++) {
				for (int y = 0; y < 16; y++) {
					for (int z = 0; z < 16; z++) {
						WorldVoxel& voxel = this->blocks.at(x, y, z);

						meshAddCube(world, voxel, glm::ivec3(x, y, z));
					}
				}
			}
			this->mesh->update();
			this->mesh->needsRegen = false;
		}

		
	}

	void Chunk::meshAddCube(World* world, const WorldVoxel& voxel, const glm::vec3& loc)
	{
		
		if (voxel.block->block_id == util::as_int(BLOCK::air)) return;

		std::array<bool, 6> occludeData = meshCheckOcclusion(world, loc);
		/*fmt::print("occludeData: ");
		for(auto& b : occludeData) fmt::print(stderr, "{},", b);
		fmt::print("\n");*/

		//std::array<bool, 6> occludeData = { 0,0,0,0,0,0 };

		for (int currentFace = 0; currentFace < 6; currentFace++) {
			if (occludeData[currentFace]) {
				//fmt::print(stderr, "occluded face\n");
				continue;
			}
			std::array<int, 30> blockFace = Blockface::vertices[currentFace];

			// translates and adds each vertex for each triangle to the vertices array
			for (int i = 0; i < 30; i = i + 5) {
				mesh::Vertex vertex = {};
				glm::vec3 vertexPos(blockFace[i + 0], blockFace[i + 1], blockFace[i + 2]);

				/*int uOffset = blockFace[i + 3] * TEXTURE_WIDTH;
				int vOffset = blockFace[i + 4] * TEXTURE_WIDTH;

				float texU = ((block->blockID - 1 % ATLAS_WIDTH) + (float)blockFace[i + 3]) / ATLAS_WIDTH;
				float texV = ((floor((block->blockID - 1) / ATLAS_WIDTH)) + (float)blockFace[i + 4]) / ATLAS_WIDTH;*/


					



				//vec2 textureCoord = { texU, texV };								REWRITE THIS 
				//vec2 textureCoord = { blockFace[i + 3], blockFace[i + 4] };
				// sets the vertex position
				vertex.position = vertexPos + loc;
				// sets the vertex normal
				vertex.normal = Blockface::normals[currentFace];
				// sets the vertex texture coords
				//glm_vec2_copy(textureCoord, vertex.texcoord);						REWRITE THIS
				vertex.uv = glm::vec2(0.0, 0.0);
				//vertex.color[0] = block->color[0];
				// Sets the vertex color
				vertex.color = glm::vec3(1.0, 1.0, 1.0);

				this->mesh->addVertex(vertex);
			}
		}
	
	}

	std::array<bool, 6> Chunk::meshCheckOcclusion(World* world, const glm::vec3& loc)
	{
		glm::vec3 adjacentLoc;
		glm::vec3 adjacentPos;
		glm::vec3 normal;

		std::array<bool, 6> occlude = {};

		for (int currentFace = 0; currentFace < 6; currentFace++) {

			adjacentLoc = loc;								// uses the initial blockposition
			normal = Blockface::normals[currentFace];		// gets the normal of the current face
			adjacentLoc += normal;							// adds the normal to the face to get the adjacent block
			adjacentPos = this->location * 16.0f + adjacentLoc;
			//fmt::print("chunk location: {}, {}, {}", this->location.x, this->location.y, this->location.z);
			if(world->checkBounds(adjacentPos)) {
				WorldVoxel& voxel = world->getVoxel(adjacentPos.x, adjacentPos.y, adjacentPos.z);
				if (voxel.block->block_id == 0) {
					occlude[currentFace] = false;
				}
				else {
					occlude[currentFace] = true;
					//fmt::print(stderr, "occluded face\n");
				}
			}
			else {
				occlude[currentFace] = false;
			}
		}

		return occlude;
	}

}