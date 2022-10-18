#include "shader.h"

/*
1. retrieve the vertex/fragment source code from filepath

2. compile the shaders
*/
Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path) {
	unsigned int vertex_shader = this->create_shader(vertex_path, GL_VERTEX_SHADER);
	unsigned int fragment_shader = this->create_shader(fragment_path, GL_FRAGMENT_SHADER);

	// shader program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex_shader);
	glAttachShader(this->ID, fragment_shader);
	glLinkProgram(this->ID);

	int success;
	char info_log[512];

	// print linking errors if they exist
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->ID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	// the shaders are already linked to our program and are no longer necessary so we delete them
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path, const GLchar* geometry_path) {
	unsigned int vertex_shader = this->create_shader(vertex_path, GL_VERTEX_SHADER);
	unsigned int fragment_shader = this->create_shader(fragment_path, GL_FRAGMENT_SHADER);
	unsigned int geometry_shader = this->create_shader(geometry_path, GL_GEOMETRY_SHADER);

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex_shader);
	glAttachShader(this->ID, fragment_shader);
	glAttachShader(this->ID, geometry_shader);
	glLinkProgram(this->ID);

	int success;
	char info_log[512];

	// print linking errors, if they exist
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	// the shaders are already linked to our program and are no longer necessary so we delete them
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(geometry_shader);
}

unsigned int Shader::create_shader(const GLchar* path, GLenum type) {
	unsigned int reference;
	std::string code;
	std::ifstream file;

	// ensure the ifstream object can throw exceptions
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open file
		file.open(path);

		// read file's buffer content into stream
		std::stringstream stream;

		stream << file.rdbuf();

		// close file handler
		file.close();

		// convert the file's content into a string
		code = stream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << path << std::endl;
	}

	const char* c_code = code.c_str();

	// check for compile errors
	int success;
	char info_log[512];

	reference = glCreateShader(type);
	glShaderSource(reference, 1, &c_code, NULL);
	glCompileShader(reference);

	glGetShaderiv(reference, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::string str;

		switch (type) {
		case GL_VERTEX_SHADER:
			str = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			str = "FRAGMENT";
			break;
		case GL_GEOMETRY_SHADER:
			str = "GEOMETRY";
			break;
		}

		glGetShaderInfoLog(reference, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::" << str << "::COMPILATION_FAILED\n" << path << '\n' << info_log << std::endl;
	}

	return reference;
}

void Shader::use() const {
	glUseProgram(ID);
}

void Shader::set_bool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_float(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::set_mat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::set_dir_light(const DirectionalLight& dir_light) const {
	std::string uniform_name = dir_light.get_uniform_name();

	this->set_vec3(uniform_name + ".ambient", dir_light.get_ambient());
	this->set_vec3(uniform_name + ".diffuse", dir_light.get_diffuse());
	this->set_vec3(uniform_name + ".specular", dir_light.get_specular());
	this->set_vec3(uniform_name + ".direction", dir_light.get_direction());
}

void Shader::set_spot_light(const SpotLight& spot_light) const {
	std::string uniform_name = spot_light.get_uniform_name();

	this->set_vec3(uniform_name + ".ambient", spot_light.get_ambient());
	this->set_vec3(uniform_name + ".diffuse", spot_light.get_diffuse());
	this->set_vec3(uniform_name + ".specular", spot_light.get_specular());
	this->set_vec3(uniform_name + ".pos", spot_light.get_pos());
	this->set_vec3(uniform_name + ".direction", spot_light.get_direction());
	this->set_float(uniform_name + ".cutOff", spot_light.get_cut_off());
	this->set_float(uniform_name + ".outerCutOff", spot_light.get_outer_cut_off());
	this->set_float(uniform_name + ".constant", spot_light.get_constant());
	this->set_float(uniform_name + ".linear", spot_light.get_linear());
	this->set_float(uniform_name + ".quadratic", spot_light.get_quadratic());
}

void Shader::set_point_light(const PointLight& point_light) const {
	std::string uniform_name = point_light.get_uniform_name();

	this->set_vec3(uniform_name + ".ambient", point_light.get_ambient());
	this->set_vec3(uniform_name + ".diffuse", point_light.get_diffuse());
	this->set_vec3(uniform_name + ".specular", point_light.get_specular());
	this->set_vec3(uniform_name + ".pos", point_light.get_pos());
	this->set_float(uniform_name + ".constant", point_light.get_constant());
	this->set_float(uniform_name + ".linear", point_light.get_linear());
	this->set_float(uniform_name + ".quadratic", point_light.get_quadratic());
}