#include "novox/block.h"
#include "novox/util.h"
#include <cstdlib>
#include <time.h>
#include <fmt/CORE.h>

namespace novox::world{
	const std::array<glm::vec3, 6> Blockface::normals = {
		glm::vec3( 0,  1,  0),
		glm::vec3( 0, -1,  0),
		glm::vec3( 1,  0,  0),
		glm::vec3(-1,  0,  0),
		glm::vec3( 0,  0,  1),
		glm::vec3( 0,  0, -1),
	};
	const std::array<std::array<int, 30>, 6> Blockface::vertices = { {
			// +y
			{
			0,1,0,  0,1,		// top-left
			1,1,1,  1,0,		// bottom-right
			1,1,0,  1,1,		// top-right

			1,1,1,  1,0,		// bottom-right
			0,1,0,  0,1,		// top-left
			0,1,1,  0,0,		// bottom-left        
			},

			// -y
			{
			0,0,0,  0,1,		// top-left
			1,0,0,  0,1,		// top-left
			1,0,1,  0,0,		// bottom-left

			1,0,1,  0,0,		// bottom-left
			0,0,1,  1,0,		// bottom-right
			0,0,0,  1,1,		// top-right 
			},

			// +x
			{
			1,1,1,  0,1,		// top-left
			1,0,0,  1,0,		// bottom-right
			1,1,0,  1,1,		// top-right

			1,0,0,  1,0,		// bottom-right
			1,1,1,  0,1,		// top-left
			1,0,1,  0,0,		// bottom-left   
			},
			// -x
			{
			0,1,1,  1,1,		// top-right
			0,1,0,  0,1,		// top-left
			0,0,0,  0,0,		// bottom-left

			0,0,0,  0,0,		// bottom-left
			0,0,1,  1,0,		// bottom-right
			0,1,1,  1,1,		// top-right
			},

			// +z
			{
			0,0,1,  0,0,		// bottom-left
			1,0,1,  1,0,		// bottom-right
			1,1,1,  1,1,		// top-right

			1,1,1,  1,1,		// top-right
			0,1,1,  0,1,		// top-left
			0,0,1,  0,0			// bottom-left
			},
		// -z
		{
		0,0,0,  1,0,		// Bottom-left
		1,1,0,  0,1,		// top-right
		1,0,0,  0,0,		// bottom-right         

		1,1,0,  0,1,		// top-right
		0,0,0,  1,0,		// bottom-left
		0,1,0,  1,1,		// top-left
		}

	} };

	void initializeBlocks() {
		new Block("air");
		new Block("stone");
		new Block("dirt");

		srand(time(0));
	}

	WorldVoxel::WorldVoxel(const Block* block, blockdata_t data) : block(block) {
		this->data = data;
	}
	WorldVoxel::WorldVoxel(const Block* block) : block(block) {
		this->data = {};
	}
	WorldVoxel::WorldVoxel() : block(Block::defaultBlocks.at(rand() % 2)) {
		this->data = {};
		std::string name = this->block->name;
		//fmt::print("created worldvoxel {}\n", name);
	}


	std::vector<const Block*> Block::defaultBlocks;

	Block::Block(const std::string name) : name(name)
	{
		this->block_id = Block::defaultBlocks.size();
		Block::defaultBlocks.push_back(this);
		fmt::print("created block {}\n", name);
	}

	const Block* Block::operator[](unsigned int ID) {
		if (0 <= ID && ID < Block::defaultBlocks.size()) {
			return Block::defaultBlocks[ID];
		}
		else {
			return Block::defaultBlocks[0];
		}
	}



}