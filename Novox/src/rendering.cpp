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

	Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
		this->ID = compile(vertexPath, fragmentPath);
		shadermap[name] = this;
	}
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
		this->ID = compile(vertexPath, fragmentPath);
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

	GLuint Texture::load()
	{
		//static bool run_once = []()->bool {stbi_set_flip_vertically_on_load(true); return true; }();

		GLuint texid = 0;
		glGenTextures(1, &texid);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;

		static unsigned char* imgData;
		imgData = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

		if (imgData != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
			fmt::print("Succesfully loaded Texture\n");
		}
		else {
			fmt::print("Failed to load Texture\n");
		}
		stbi_image_free(imgData);
		glBindTexture(GL_TEXTURE_2D, texid);

		return texid;
	}

	Texture::Texture(const std::string& texturePath)
	{
		this->path = texturePath;
		this->ID = load();
	}

	void Texture::bind()
	{
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, this->ID);
	}

}