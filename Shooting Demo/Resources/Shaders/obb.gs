#version 330 core

layout (points) in;
layout (line_strip) out;

in VS_OUT {
	vec3 u;
	vec3 w;
	vec3 v;
} gs_in[];

out vec3 fColor;

const float MAGNITUDE = 5.0f;

void draw_lines() {
	// draw u (red)
	fColor = vec3(1.0f, 0.0f, 0.0f);

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = vec4(gs_in[0].u, 1.0f) * MAGNITUDE;
	EmitVertex();

	EndPrimitive();

	// draw w (green)
	fColor = vec3(0.0f, 1.0f, 0.0f);

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = vec4(gs_in[0].w, 1.0f) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();

	// draw v (blue)
	fColor = vec3(0.0f, 0.0f, 1.0f);

	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = vec4(gs_in[0].v, 1.0f) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void main() {
	draw_lines();
}