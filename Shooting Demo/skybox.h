#pragma once

#include <string>
#include <vector>

#include "texture.h"

class Skybox {
private:
	std::vector<std::string> faces;
	Texture* cubemap;
	unsigned int vao, vbo;

public:
	Skybox(std::vector<std::string> faces);
	virtual ~Skybox();

	enum FACES {
		RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK
	};

	// faces are meant to be in the order right, left, top, bottom, front, back in order to display properly
	void set_faces(std::vector<std::string> faces);
	void set_face(FACES face, const char* path);
	void render_skybox();
	void bind();
};