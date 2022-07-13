#include "novox/mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
//#include <glm/ext/matrix_transform.hpp>
#include "novox/rendering.h"
#include "novox/util.h"
#include <fmt/core.h>

namespace novox::mesh {
	ChunkMesh::ChunkMesh() {
		this->needsRegen = true;
		this->vertexCount = 0;
		
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, 128000 * sizeof(Vertex), this->vertices.data(), GL_DYNAMIC_DRAW);

		// position
		glVertexAttribPointer(util::as_int(Vertexattrib::pos), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(util::as_int(Vertexattrib::pos));
		// normal
		glVertexAttribPointer(util::as_int(Vertexattrib::norm), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(util::as_int(Vertexattrib::norm));
		// texture coord
		glVertexAttribPointer(util::as_int(Vertexattrib::uv), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(util::as_int(Vertexattrib::uv));
		// color data
		glVertexAttribPointer(util::as_int(Vertexattrib::color), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(util::as_int(Vertexattrib::color));

		

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void ChunkMesh::update()
	{
		//fmt::print(stderr, "mesh update: Vertex Count: {}\n", this->vertices.size());

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, sizeof(Vertex) * this->vertices.size(), this->vertices.data());
		//fmt::print(stderr, "mesh update: subdata updated\n");


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		this->vertexCount = this->vertices.size();
		this->vertices.clear();
	}

	void ChunkMesh::draw(rendering::Shader& shader, const glm::vec3& loc)
	{
		static rendering::Texture textureAtlas("textures/TextureAtlas.png");
		textureAtlas.bind();
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, loc * 16.0f);
		shader.setMat4("model", model);

		//fmt::print("chunk drawn at location: {},{},{}\n", loc.x, loc.y, loc.z);


		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ChunkMesh::addVertex(Vertex& vertex)
	{
		this->vertices.push_back(vertex);
	}

}