#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 ray;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float mouseX;
uniform float mouseY;

uniform int windowWidth;
uniform int windowHeight;

void main() {
	float x = (2.0f * mouseX) / windowWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / windowHeight;
	float z = 1.0f;

	vec3 ray_nds = vec3(x, y, z);

	vec4 ray_clip = vec4(ray_nds.xy, -1.0f, 1.0f);

	vec4 ray_eye = inverse(projection) * ray_clip;

	ray_eye = vec4(ray_eye.xy, -1.0f, 0.0f);

	vec3 ray_wor = normalize((inverse(view) * ray_eye).xyz);

	vs_out.ray = ray_wor;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}