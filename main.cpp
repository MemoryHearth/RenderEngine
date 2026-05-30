#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <iostream>

#include "stb_image.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

	Shader shader("/shaders/basic.vert", "/shaders/basic.frag");

	float vertices[] = {
		-0.75f, 0.75f, 0.0f, 1.0f, 0.75f, 0.25f, 1.0f, 0.0f, // top left
		-0.75f, -0.75f, 0.0f, 0.5f, 0.55f, 0.75f, 0.0f, 0.0f, // bottom left
		0.0f, -0.75f, 0.0f, 0.5f, 0.55f, 0.75f, 0.0f, 1.0f, // bottom right
		0.0f, 0.75f, 0.0f, 1.0f, 0.75f, 0.25f, 1.0f, 1.0f // top right
	};
	
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channel_number;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load((std::filesystem::current_path().string() + "/textures/container.jpg").c_str(), &width, &height, &channel_number, 0);

	if (data) {
		GLenum format = channel_number == 4 ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Texture couldn't be loaded. stbi: " << stbi_failure_reason() << std::endl;
	}

	stbi_image_free(data);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}