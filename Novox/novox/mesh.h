#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <novox/shader.h>

namespace novox::mesh{

	enum class Vertexattrib {
		pos = 0,
		norm = 1,
		uv = 2,
		color = 3

	};


	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 color;
	};

	class ChunkMesh {
	private:
		unsigned int VAO;
		unsigned int VBO;
		std::vector<Vertex> vertices;
		int vertexCount;

	public:
		ChunkMesh();
		void update();
		void draw(shader::Shader& shader, const glm::vec3& loc);
		void addVertex(Vertex& vertex);

		bool needsRegen;
	};
	
}