#include "novox/data.h"
#include <fstream>
#include <sstream>
#include <stb_image.h>

namespace novox::data {
	

	std::map<std::string, std::shared_ptr<Shader>> shaderMap;
	std::map<std::string, std::shared_ptr<Texture>> textureMap;


	std::shared_ptr<Shader> loadShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& name)
	{
		std::string vertexCode, fragmentCode;
		std::ifstream vertexFile, fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Reads the code from the vertex and fragment shader files
		try {

			vertexFile.open(vertexShaderFile, std::ios_base::in);;
			fragmentFile.open(fragmentShaderFile, std::ios_base::in);;
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


		
		shaderMap[name] = std::make_shared<Shader>(new Shader(programID));
		return shaderMap[name];

	}

	std::shared_ptr<Texture>  loadTexture(const std::string& textureFile, const std::string& name)
	{
		//static bool run_once = []()->bool {stbi_set_flip_vertically_on_load(true); return true; }();

		GLuint texID = 0;
		glGenTextures(1, &texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;

		static unsigned char* imgData;
		imgData = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

		if (imgData != NULL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);
			fmt::print("Succesfully loaded Texture\n");
		}
		else {
			fmt::print("Failed to load Texture\n");
		}
		stbi_image_free(imgData);
		glBindTexture(GL_TEXTURE_2D, 0);


		textureMap[name] = std::make_shared<Texture>(new Texture(texID));
		return textureMap[name];
	}

	std::shared_ptr<Shader> getShader(const std::string& name)
	{

	}

	std::shared_ptr<Texture> getTexture(const std::string& name)
	{

	}
}
