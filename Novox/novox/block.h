#pragma once
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace novox::world {
	class Block;

	enum class BLOCK {
		air,
		stone,
		dirt,
		grass,
	};

	struct Blockface {
		enum faces {
			top,		// +y
			bottom,		// -y
			east,		// +x
			west,		// -x
			north,		// +z
			south		// -z
		};
		
		static const std::array<glm::vec3, 6> normals;
		static const std::array<std::array<int, 30>, 6> vertices;
	};




	class Block {
	public:
		const std::string name;
		unsigned int block_id;
		//unsigned int texture_id;

		Block(const std::string name);
		Block() = default;

		static std::vector<const Block*> defaultBlocks;

		const Block* operator[] (unsigned int ID);

	};
	void initializeBlocks();

	struct blockdata_t {
		glm::vec3 color;

	};

	class WorldVoxel {
	public:
		const Block* block;
		blockdata_t data;

		WorldVoxel(const Block* block, blockdata_t data);
		WorldVoxel(const Block* block);
		WorldVoxel();


	};


}