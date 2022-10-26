#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define NOMINMAX
#include <windows.h>
#include <fmt/core.h>

#include <array>
#include <vector>
//#include <cmath>
#include <stdexcept>

#include "novox/world.h"
#include "novox/block.h"
#include "novox/util.h"
#include "novox/rendering.h"

namespace novox::world {
	

	World::World(int x_size, int y_size, int z_size) : chunks(x_size, y_size, z_size), x_size(x_size), y_size(y_size), z_size(z_size) {
		for (int x = 0; x < x_size; x++) {
			for (int y = 0; y < y_size; y++) {
				for (int z = 0; z < z_size; z++) {
					Chunk& chunk = this->chunks.at(x, y, z);
					chunk.setLocation(glm::ivec3(x, y, z));
					fmt::print("chunk loc {},{},{}\n", x, y, z);
					chunk.populate();
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

	void World::updateChunkMesh(Chunk& chunk, const glm::ivec3& position) {
		glm::ivec3 chunkPos = chunk.getLocation() * 16;
		glm::ivec3 offset = position - chunkPos;
		

		auto checkAndUpdate = [this](glm::ivec3 npos) mutable{
			if (this->checkBounds(npos)) {
				this->getChunkAt(npos.x, npos.y, npos.z).tagForRegen();
			}
			else {
				return;
			}
		};


		chunk.tagForRegen();

		if (offset.x == 0) {
			glm::ivec3 npos = glm::ivec3(position.x - 1, position.y, position.z);
			checkAndUpdate(npos);
		}
		else if (offset.x == 15) {
			glm::ivec3 npos = glm::ivec3(position.x + 1, position.y, position.z);
			checkAndUpdate(npos);
		}

		if (offset.y == 0) {
			glm::ivec3 npos = glm::ivec3(position.x, position.y - 1, position.z);
			checkAndUpdate(npos);
		}
		else if (offset.y == 15) {
			glm::ivec3 npos = glm::ivec3(position.x, position.y + 1, position.z);
			checkAndUpdate(npos);
		}

		if (offset.z == 0) {
			glm::ivec3 npos = glm::ivec3(position.x, position.y, position.z - 1);
			checkAndUpdate(npos);
		}
		else if (offset.z == 15) {
			glm::ivec3 npos = glm::ivec3(position.x, position.y, position.z + 1);
			checkAndUpdate(npos);
		}


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
			return chunks.inRange(std::floor(pos.x / 16), std::floor(pos.y / 16), std::floor(pos.z / 16));
		}
	}

	std::vector<std::pair<glm::ivec3, WorldVoxel>> World::castVoxelRay(const glm::vec3& start, const glm::vec3& end) {
		
		float voxel_size = 1.0;
		//glm::ivec3 endBound
		std::vector<std::pair<glm::ivec3, WorldVoxel>> intersectedVoxels;

		auto appendVoxel = [&](glm::ivec3 pos) {
			if (this->checkBounds(pos)) {
				auto pair = std::make_pair(pos, this->getVoxel(pos));
				intersectedVoxels.push_back(pair);
			}
			else {
				auto pair = std::make_pair(pos, WorldVoxel(Block::defaultBlocks[0]));
				intersectedVoxels.push_back(pair);
			}
		};

		auto ray = end - start;
		auto rayDirection = glm::normalize(ray);



		
		glm::ivec3 currentVoxel(0);

		//start = start + glm::vec3(0.5);
		
		currentVoxel = glm::floor(start);

		/*if (start.x >= 0) {
			currentVoxel.x = glm::floor(start.x);
		}
		else {
			currentVoxel.x = glm::ceil(start.x);
		}
		if (start.y >= 0) {
			currentVoxel.y = glm::floor(start.y);
		}
		else {
			currentVoxel.y = glm::ceil(start.y);
		}
		if (start.z >= 0) {
			currentVoxel.z = glm::floor(start.z);
		}
		else {
			currentVoxel.z = glm::ceil(start.z);
		}*/

		auto firstVoxel = currentVoxel;
		auto lastVoxel = glm::ivec3(glm::floor(end));

		appendVoxel(firstVoxel);


		


		int stepX = (ray.x < 0) ? -1 : (ray.x == 0) ? 0 : 1 * voxel_size;
		int stepY = (ray.y < 0) ? -1 : (ray.y == 0) ? 0 : 1 * voxel_size;
		int stepZ = (ray.z < 0) ? -1 : (ray.z == 0) ? 0 : 1 * voxel_size;


		//float tMaxX = (rayDirection.x != 0) ? (std::max(0.0f, glm::ceil(start.x)) - start.x) / rayDirection.x : FLT_MAX;
		//float tMaxY = (rayDirection.y != 0) ? (std::max(0.0f, glm::ceil(start.y)) - start.y) / rayDirection.y : FLT_MAX;
		//float tMaxZ = (rayDirection.z != 0) ? (std::max(0.0f, glm::ceil(start.z)) - start.z) / rayDirection.z : FLT_MAX;


		//float tMaxX = (rayDirection.x != 0) ? (currentVoxel.x + stepX - start.x) / (end - start).x : FLT_MAX;
		//float tMaxY = (rayDirection.y != 0) ? (currentVoxel.y + stepY - start.y) / (end - start).y : FLT_MAX;
		//float tMaxZ = (rayDirection.z != 0) ? (currentVoxel.z + stepZ - start.z) / (end - start).z : FLT_MAX;
		// 
		// 
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))
		// kudos to stack overflow dude "ProjectPhysX"
		float tDeltaX = (stepX != 0) ? glm::min(stepX / (ray.x), FLT_MAX) : FLT_MAX;
		float tMaxX = (stepX > 0) ? tDeltaX * FRAC1(start.x) : tDeltaX * FRAC0(start.x);

		float tDeltaY = (stepY != 0) ? glm::min(stepY / (ray.y), FLT_MAX) : FLT_MAX;
		float tMaxY = (stepY > 0) ? tDeltaY * FRAC1(start.y) : tDeltaY * FRAC0(start.y);

		float tDeltaZ = (stepZ != 0) ? glm::min(stepZ / (ray.z), FLT_MAX) : FLT_MAX;
		float tMaxZ = (stepZ > 0) ? tDeltaZ * FRAC1(start.z) : tDeltaZ * FRAC0(start.z);

		/*float tDeltaX = (rayDirection.x != 0) ? 1 / rayDirection.x * voxel_size: FLT_MAX;
		float tDeltaY = (rayDirection.y != 0) ? 1 / rayDirection.y * voxel_size: FLT_MAX;
		float tDeltaZ = (rayDirection.z != 0) ? 1 / rayDirection.z * voxel_size: FLT_MAX;
		*/
		/*float tMaxX = tDeltaX * (1.0 - glm::fract(start.x));
		float tMaxY = tDeltaY * (1.0 - glm::fract(start.y));
		float tMaxZ = tDeltaZ * (1.0 - glm::fract(start.z));*/


		int count = 0;

		do {
			if (tMaxX < tMaxY) {
				if (tMaxX < tMaxZ) {
					currentVoxel.x += stepX;
					//if (currentVoxel.x == this->x_size * 16 || currentVoxel.x < 0) break; /* outside grid */
					tMaxX += tDeltaX;
				}
				else {
					currentVoxel.z += stepZ;
					//if (currentVoxel.z == this->z_size * 16 || currentVoxel.z < 0) break;
					tMaxZ += tDeltaZ;
				}
			}
			else {
				if (tMaxY < tMaxZ) {
					currentVoxel.y += stepY;
					//if (currentVoxel.y == this->y_size * 16 || currentVoxel.y < 0) break;
					tMaxY += tDeltaY;
				}
				else {
					currentVoxel.z += stepZ;
					//if (currentVoxel.z == this->z_size * 16 || currentVoxel.z < 0) break;
					tMaxZ += tDeltaZ;
				}
			}
			appendVoxel(currentVoxel);
			
		} while (currentVoxel != lastVoxel);
		//currentVoxel != lastVoxel
		appendVoxel(lastVoxel);

		return intersectedVoxels;

	}
	
	

	Chunk::Chunk() : blocks(16, 16, 16) {
		//fmt::print("Generated chunk of size {}x{}x{}\n", 16, 16, 16);

		this->mesh = new mesh::ChunkMesh();
		this->location = glm::ivec3(0);
	}
	void Chunk::populate() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				for (int k = 0; k < 16; k++) {
					auto& voxel = this->blocks.at(i, j, k);
					auto blockPos = this->location * 16 + glm::ivec3(i, j, k);
					if (blockPos.y < 16) {
						voxel.block = Block::defaultBlocks.at(1);
					}
					else if (blockPos.y < 24) {
						voxel.block = Block::defaultBlocks.at(2);
					}
					


				}
			}
		}
	}
	
	void Chunk::setLocation(const glm::ivec3& location)
	{
		this->location = location;
	}
	const glm::ivec3& Chunk::getLocation()
	{
		return this->location;
	}
	WorldVoxel& Chunk::getVoxel(int x_loc, int y_loc, int z_loc)
	{
		WorldVoxel& voxel = blocks.at(x_loc, y_loc, z_loc);
		return voxel;
	}
	


	void Chunk::draw(std::shared_ptr<rendering::Shader> shader)
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

	void Chunk::meshAddCube(World* world, const WorldVoxel& voxel, const glm::ivec3& loc)
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

				int uOffset = blockFace[i + 3] * TEXTURE_WIDTH;
				int vOffset = blockFace[i + 4] * TEXTURE_WIDTH;

				float texU = ((voxel.block->block_id - 1 % TEXTURE_ATLAS_WIDTH) + (float)blockFace[i + 3]) / TEXTURE_ATLAS_WIDTH;
				float texV = ((floor((voxel.block->block_id - 1) / TEXTURE_ATLAS_WIDTH)) + (float)blockFace[i + 4]) / TEXTURE_ATLAS_WIDTH;

				
				// sets the vertex data
				vertex.position = vertexPos + glm::vec3(loc);
				vertex.normal = Blockface::normals[currentFace];
				vertex.uv = glm::vec2(texU, texV);
				vertex.color = glm::vec3(1.0, 1.0, 1.0);

				this->mesh->addVertex(vertex);
			}
		}
	
	}

	std::array<bool, 6> Chunk::meshCheckOcclusion(World* world, const glm::ivec3& loc)
	{
		glm::ivec3 adjacentLoc;
		glm::ivec3 adjacentPos;
		glm::ivec3 normal;

		std::array<bool, 6> occlude = {};

		for (int currentFace = 0; currentFace < 6; currentFace++) {

			adjacentLoc = loc;								// uses the initial blockposition
			normal = Blockface::normals[currentFace];		// gets the normal of the current face
			adjacentLoc += normal;							// adds the normal to the face to get the adjacent block
			adjacentPos = this->location * 16 + adjacentLoc;
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