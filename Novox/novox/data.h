#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include "rendering.h"
#include "world.h"

namespace novox::data {

	typedef rendering::Shader Shader;
	typedef rendering::Texture Texture;

	

	
	extern std::map<std::string, std::shared_ptr<Shader>> shaderMap;
	extern std::map<std::string, std::shared_ptr<Texture>> textureMap;

	std::shared_ptr<Shader> loadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& name);
	std::shared_ptr<Texture> loadTexture(const std::string& textureFile, const std::string& name);

	std::shared_ptr<Shader> getShader(const std::string& name);
	std::shared_ptr<Texture> getTexture(const std::string& name);





	world::Chunk& loadChunk(world::World& world, const glm::ivec3& chunkPosition);
	


}