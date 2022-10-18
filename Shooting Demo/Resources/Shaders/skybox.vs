#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vs_out.texCoords = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
}