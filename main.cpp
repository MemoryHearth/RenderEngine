#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum class CompileType;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void checkCompile(unsigned int ID, CompileType type);

const char* vertexShaderSourceCode = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* fragmentShaderSourceCode = R"(
#version 330 core
vec4 FragColor;
void main() {
	FragColor = vec4(0.8, 0.9, 0.3, 1.0);
}
)";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 480, "My window", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Window couldn't be created" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "GLAD failed to be loaded." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int shaderProgramID = glCreateProgram();

	glShaderSource(vertexShaderID, 1, &vertexShaderSourceCode, nullptr);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSourceCode, nullptr);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);

	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	float vertices[] = { -0.75f, 0.75f, 0.0f,
		-0.75f, -0.75f, 0.0f,
		0.0f, -0.75f, 0.0f 
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgramID);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

enum class CompileType {VERTEX, FRAGMENT, SHADER};

void checkCompile(unsigned int ID, CompileType type) {
	int success;
	char infolog[512];
	switch (type) {
	case CompileType::SHADER:
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		break;
	default:
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		break;
	}

	if (!success) {
		switch (type) {
		case CompileType::SHADER:
			glGetProgramInfoLog(ID, 512, nullptr, infolog);
			std::cout << "Program link error!" << std::endl;
			break;
		default:
			glGetShaderInfoLog(ID, 512, nullptr, infolog);
			std::cout << ((int) type == 0 ? "Vertex" : "Fragment") << " compile error!" << std::endl;
			break;
		}
	}
}