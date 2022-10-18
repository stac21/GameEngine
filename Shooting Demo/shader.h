#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "directional_light.h"
#include "spotlight.h"
#include "point_light.h"

class Shader {
private:
	unsigned int create_shader(const GLchar* path, GLenum type);

public:
	// the program ID
	unsigned int ID;

	// constructor builds and rebuilds the shader
	Shader(const GLchar* vertex_path, const GLchar* fragment_path);
	Shader(const GLchar* vertex_path, const GLchar* fragment_path, const GLchar* geometry_path);
	// use/activate the shader
	void use() const;
	// utility uniform functions
	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set_mat4(const std::string& name, glm::mat4 value) const;
	void set_vec3(const std::string& name, glm::vec3 value) const;
	void set_dir_light(const DirectionalLight& dir_light) const;
	void set_spot_light(const SpotLight& spot_light) const;
	void set_point_light(const PointLight& point_light) const;
};