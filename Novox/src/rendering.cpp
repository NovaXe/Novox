#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <windows.h>
#include <fmt/core.h>
#include <memory>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


#include "novox/rendering.h"
#include "novox/data.h"


namespace rendering {

	std::unordered_map<std::string, Shader*> Shader::shadermap;

	GLuint Shader::compile(const std::string& vertexPath, const std::string& fragmentPath) {
		// Vertex and Fragment shader files
		std::string vertexCode, fragmentCode;
		std::ifstream vertexFile, fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Reads the code from the vertex and fragment shader files
		try {

			vertexFile.open(vertexPath, std::ios_base::in);;
			fragmentFile.open(fragmentPath, std::ios_base::in);;
			std::stringstream vStream, fStream;

			vStream << vertexFile.rdbuf();
			fStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexCode = vStream.str();
			fragmentCode = fStream.str();

		}
		catch (std::ifstream::failure e) {
			fmt::print("ERROR::SHADER::FILE_FAILED_READ");
		}

		// Code for Shader as c style string
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();


		GLuint programID, vertex, fragment;
		int success;
		char infoLog[512];

		// Compiles the vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
		}
		// Compiles the fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
		}

		programID = glCreateProgram();
		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		glLinkProgram(programID);
		//checks for linking errors
		glGetShaderiv(programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(programID, 512, NULL, infoLog);
			printf("ERROR::SHADER::PROGRAM::LINK_FAILED\n%s\n", infoLog);
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return programID;
	}

	
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
		this->ID = compile(vertexPath, fragmentPath);
	}
	Shader::Shader(GLuint ID) : ID(ID)
	{

	}

	void Shader::use() {
		glUseProgram(this->ID);
		//CurrentShader = std::make_unique<Shader>(this);

	}

	void Shader::setBool(const std::string& name, bool value) const {

	}
	void Shader::setInt(const std::string& name, int value) const {

	}
	void Shader::setFloat(const std::string& name, float vlaue) const {

	}

	void Shader::setVec3(const std::string& name, glm::vec3 vec) const {
		glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(vec));
	}

	void Shader::setMat4(const std::string& name, glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}




	//GLuint Texture::load()
	//{
	//	//static bool run_once = []()->bool {stbi_set_flip_vertically_on_load(true); return true; }();

	//	GLuint texid = 0;
	//	glGenTextures(1, &texid);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texid);
	//	

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//	int width, height, nrChannels;

	//	static unsigned char* imgData;
	//	imgData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	//	if (imgData != NULL) {
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	//		glGenerateMipmap(GL_TEXTURE_2D);
	//		fmt::print("Succesfully loaded Texture\n");
	//	}
	//	else {
	//		fmt::print("Failed to load Texture\n");
	//	}
	//	stbi_image_free(imgData);
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	return texid;
	////}

	Texture::Texture(GLuint ID) : ID(ID)
	{

	}

	void Texture::bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}

	Renderer::Renderer(Shader& shader) : shader(shader) {}
	Renderer::~Renderer() {
		glDeleteVertexArrays(1, &this->VAO);
	}




	void SpriteRenderer::initRenderData()
	{
		GLuint VBO;
		float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &this->quadVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(this->quadVAO);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}

	SpriteRenderer::SpriteRenderer(Shader& shader) : shader(shader)
	{
		initRenderData();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		glDeleteVertexArrays(1, &this->quadVAO);
	}

	void SpriteRenderer::drawSprite(std::shared_ptr<Texture> texture, const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec3& color) 
	{
		this->shader.use();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(pos, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		
		model = glm::scale(model, glm::vec3(size, 1.0f));

		this->shader.setMat4("model", model);
		this->shader.setVec3("spriteColor", color);

		glActiveTexture(GL_TEXTURE0);
		texture->bind();


		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);



	}



	LineRenderer::LineRenderer(Shader& shader) : shader(shader)
	{
		initRenderData();
	}

	LineRenderer::~LineRenderer()
	{
		glDeleteVertexArrays(1, &this->lineVAO);
	}

	void LineRenderer::initRenderData()
	{
		GLuint VBO;
		GLfloat vertices[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(1, &this->lineVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(this->lineVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void LineRenderer::drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color)
	{
		this->shader.use();
		glm::vec3 dif = end - start;

		glm::mat4 model(1.0f);



		model = glm::translate(model, start);
		model = glm::scale(model, dif);
		this->shader.setMat4("model", model);
		this->shader.setVec3("color", color);

		glBindVertexArray(this->lineVAO);
		glLineWidth(10);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

	}




}