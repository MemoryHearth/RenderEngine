#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	glEnable(GL_DEPTH_TEST);

	Shader shader("/shaders/basic.vert", "/shaders/basic.frag");

	float vertices[] = {
		// konum (x,y,z)        // texture (s,t)
		// Arka yüz
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,

		// Ön yüz
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

		// Sol yüz
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

		// Sağ yüz
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

		 // Alt yüz
		 -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

		 // Üst yüz
		 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,    0.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::vec3 cubeLocations[] = {
			glm::vec3(0.0f,  0.0f,  -4.5f),
			glm::vec3(3.0f,  3.0f, -4.5f),
			glm::vec3(3.0f, 3.0f, -4.5f),
			glm::vec3(3.0f, 3.0f, -4.5f),
			glm::vec3(3.0f, 3.0f, -4.5f),
			glm::vec3(3.0f, 3.0f, -4.5f),
			glm::vec3(3.0f, 3.0f, -4.5f)
		};

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 600.0f / 480.0f, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		float time = (float)glfwGetTime();

		for (int i = 0; i < 7; i++) {
			
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(cubeLocations[i].x * sin(time + i), cubeLocations[i].y * cos(time + i), cubeLocations[i].z * ((sin(time) + 1.0f))));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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