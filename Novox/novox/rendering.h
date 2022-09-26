#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <unordered_map>

#define TEXTURE_ATLAS_WIDTH 16
#define TEXTURE_WIDTH 16

namespace rendering {
	class Shader;
	class Texture;
	class Renderer;




	class Shader{
	private:
		GLuint compile(const std::string& vertexPath, const std::string& fragmentPath);
		GLuint ID;
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

	class Texture {
	private:
		GLuint ID;
		std::string path;
		GLuint load();
	public:
		Texture(const std::string& texturePath);
		void bind();
	};

	


	class SpriteRenderer {
	private:
		Shader& shader;
		void initRenderData();
		GLuint quadVAO;


	public:
		SpriteRenderer(Shader& shader);
		~SpriteRenderer();

		void drawSprite(Texture& texture, const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec3& color);

	};

	class LineRenderer {
	private:
		Shader& shader;
		void initRenderData();
		GLuint lineVAO;
		
	public:
		LineRenderer(Shader& shader);
		~LineRenderer();

		void drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);

	};

	class Renderer {
	private:
		Shader& shader;
		virtual void initRenderData() = 0;
		GLuint VAO;
	public:
		Renderer(Shader& shader);
		~Renderer();
		virtual void renderToScreen() = 0;
		
	};

}