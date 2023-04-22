#pragma once

#include <fstream>
#include <iostream>

#include "chunk.h"

class Level {
private:
	static const std::string WALL_CHAR;
	static const std::string ENEMY_CHAR;
	static const std::string LIGHT_CHAR;

	std::vector<Chunk> chunks;

public:
	Level(const std::string& file_path);

	void load(const std::string& file_path);
};