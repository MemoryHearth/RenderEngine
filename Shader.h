#pragma once

#include <string>
#include <glm/glm.hpp>

enum class ComponentType : uint8_t { VERTEX, FRAGMENT, PROGRAM };

class Shader {
public:
	unsigned int ID = 0;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void use() const;

	void setInt(const std::string& name, int value) const;
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	std::string readFile(const std::string& path);

	void checkComponent(unsigned int ID, ComponentType type);
};
