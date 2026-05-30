#version 330 core
in vec3 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture;

void main() {
	FragColor = texture(texture, texCoord) * vec4(vertexColor, 1.0);
}