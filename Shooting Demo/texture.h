#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <vector>

#include "stb_image.h"

class Texture {
private:
	unsigned int id;
	int target;
	int type;
	std::string path;

public:
	Texture(int target, int type);
	void bind();
	void generate_texture(const char* image_path, int rgb_type);
	void generate_cubemap(const std::vector<std::string> faces, int rgb_type);

	void set_id(unsigned int id);
	void set_target(int target);
	void set_type(int type);
	void set_path(std::string& path);

	const unsigned int get_id();
	const int get_target();
	const int get_type();
	const std::string get_path();

	enum Types {
		DIFFUSE, SPECULAR, NORMAL, HEIGHT, REFLECTION
	};
};