#pragma once

#include <string>
#include <map>
#include "rendering.h"


namespace novox::data {

	typedef rendering::Shader Shader;
	typedef rendering::Texture Texture;

	class ResourceManager {
	private:

		

	public:
		static std::map<std::string, rendering::Shader> shaderMap;
		static std::map<std::string, rendering::Texture> textureMap;

		static Shader getShader(const std::string& name);

		static Shader loadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& name);

		static Texture loadTexture(const std::string& textureFile, const std::string& name);
	};



}