#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec2 texCoords;
} fs_in;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform float exposure;

void main() {
	const float gamma = 2.2f;

	vec3 hdrColor = texture(screenTexture, fs_in.texCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, fs_in.texCoords).rgb;
	hdrColor += bloomBlur;

	vec3 result = vec3(1.0f) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0f / gamma));
	FragColor = vec4(result, 1.0f);
}