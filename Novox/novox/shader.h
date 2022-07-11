#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>



namespace shader {
	class Shader;

	



	class Shader{
	private:
		unsigned int compile(const std::string& vertexPath, const std::string& fragmentPath);
		unsigned int ID;
	public:
		Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		Shader() = default;

		static std::unordered_map<std::string, Shader*> shadermap;

		void use();

		void setBool(const std::string& name, bool value) const;

		void setInt(const std::string& name, int value) const;

		void setFloat(const std::string& name, float vlaue) const;

		void setVec3(const std::string& name, glm::vec3 vec) const;

		void setMat4(const std::string& name, glm::mat4 mat) const;


	};
}