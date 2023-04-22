#include "level.h"

const std::string Level::WALL_CHAR = "x";
const std::string Level::ENEMY_CHAR = "e";
const std::string Level::LIGHT_CHAR = "l";

Level::Level(const std::string& file_path) {
	this->load(file_path);
}

void Level::load(const std::string& file_path) {
	std::ifstream file;

	try {
		file.open(file_path);



		file.close();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::BUTTON_MAPPINGS::FILE_NOT_SUCCESSFULLY_READ\n" << file_path << std::endl;
	}
}