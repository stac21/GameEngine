#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
	vec3 fragPos;
	vec3 normal;
	vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 vertexPos = vec4(aPos, 1.0f);
	mat3 normalMatrix = transpose(mat3(model));

	vs_out.fragPos = vec3(model * vertexPos);
	vs_out.normal = normalize(normalMatrix * aNormal);
	vs_out.texCoords = aTexCoords;

	gl_Position = projection * view * model * vertexPos;
}