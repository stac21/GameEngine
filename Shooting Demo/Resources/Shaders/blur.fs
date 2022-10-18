#version 330 core

out vec4 FragColor;

in VS_OUT {
	vec2 texCoords;
} fs_in;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main() {
	// gets size of single texel
	vec2 tex_offset = 1.0 / textureSize(image, 0);
	// current fragment's contribution
	vec3 result = texture(image, fs_in.texCoords).rgb * weight[0];

	if (horizontal) {
		for (int i = 1; i < 5; i++) {
			result += texture(image, fs_in.texCoords + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
			result += texture(image, fs_in.texCoords - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
		}
	} else {
		for (int i = 1; i < 5; i++) {
			result += texture(image, fs_in.texCoords + vec2(0.0f, tex_offset.y * i)).rbg * weight[i];
			result += texture(image, fs_in.texCoords - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
		}
	}

	FragColor = vec4(result, 1.0f);
}