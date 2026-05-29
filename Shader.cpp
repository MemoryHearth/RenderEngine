#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	ID = glCreateProgram();

	std::string vertexShaderSourceCode = readFile(vertexPath);
	std::string fragmentShaderSourceCode = readFile(fragmentPath);

	const char* vertexSource = vertexShaderSourceCode.c_str();
	const char* fragmentSource = fragmentShaderSourceCode.c_str();

	glShaderSource(vertexShaderID, 1, &vertexSource, nullptr);
	glShaderSource(fragmentShaderID, 1, &fragmentSource, nullptr);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	checkComponent(vertexShaderID, ComponentType::VERTEX);
	checkComponent(fragmentShaderID, ComponentType::FRAGMENT);

	glAttachShader(ID, vertexShaderID);
	glAttachShader(ID, fragmentShaderID);

	glLinkProgram(ID);

	checkComponent(ID, ComponentType::PROGRAM);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::~Shader() {
	if (ID != 0) {
		glDeleteProgram(ID);
	}
}

void Shader::use() const {
	glUseProgram(ID);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::readFile(const std::string& path) {
	std::string aPath = std::filesystem::current_path().string() + path;
	std::ifstream file(aPath);

	if (!file.is_open()) {
		std::cout << "File couldn't be opened. (" << aPath << ")" << std::endl;
		return "";
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	return ss.str();
}

void Shader::checkComponent(unsigned int ID, ComponentType type) {
	int success = -1;
	char infolog[512];
	switch (type) {
	case ComponentType::PROGRAM:
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		break;
	case ComponentType::VERTEX:
	case ComponentType::FRAGMENT:
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		break;
	}

	if (!success) {
		switch (type) {
		case ComponentType::PROGRAM:
			glGetProgramInfoLog(ID, 512, nullptr, infolog);
			std::cout << "Program link error!" << std::endl;
			break;
		case ComponentType::VERTEX:
		case ComponentType::FRAGMENT:
			glGetShaderInfoLog(ID, 512, nullptr, infolog);
			std::cout << ((int)type == 0 ? "Vertex" : "Fragment") << " compile error!" << std::endl;
			break;	
		}
	} else {
		std::cout << "Component " << (int)type << " is OK." << std::endl;
	}
}