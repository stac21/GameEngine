#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 u;
	vec3 w;
	vec3 v;
} vs_out;

uniform vec3 u;
uniform vec3 w;
uniform vec3 v;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vs_out.u = (projection * view * model * vec4(u, 1.0f)).xyz;
	vs_out.w = (projection * view * model * vec4(w, 1.0f)).xyz;
	vs_out.v = (projection * view * model * vec4(v, 1.0f)).xyz;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}