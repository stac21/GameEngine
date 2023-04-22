#include "button_mappings.h"

const std::string ButtonMappings::FILE_PATH = "Resources\\Configs\\button_mappings.txt";
std::unordered_map<std::string, int> ButtonMappings::mappings;

int ButtonMappings::get_mapping(const std::string& action) {
	return mappings[action];
}

std::string ButtonMappings::edit_mapping(const std::string& action, int new_mapping) {
	// check to see if new_mapping is already bound to an action and remove that binding if so
	std::string edited_action = action;
	auto it = mappings.begin();

	while (it != mappings.end()) {
		if (it->second == new_mapping) {
			mappings[action] = -1;
			edited_action = it->first;
			it = mappings.end();
		} else {
			it++;
		}
	}

	mappings[action] = new_mapping;

	return edited_action;
}

void ButtonMappings::load_mappings() {
	std::ifstream file;

	try {
		// open file
		file.open(FILE_PATH);

		// read file's content into the mappings unordered_map
		std::string line;
		std::string action;
		int button;

		while (std::getline(file, line)) {
			action = line.substr(0, line.find(' '));
			button = std::atoi(line.substr(line.find(' ')).c_str());

			mappings.insert({ action, button });
		}

		// close file handler
		file.close();
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::BUTTON_MAPPINGS::FILE_NOT_SUCCESSFULLY_READ\n" << FILE_PATH << std::endl;
	}
}

void ButtonMappings::save_mappings() {
	std::ofstream file;

	try {
		file.open(FILE_PATH);

		// clear the file in case that actions have been removed
		file.clear();

		auto it = mappings.begin();

		while (it != mappings.end()) {
			file << it->first << " " << it->second << "\n";

			it++;
		}
	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::BUTTON_MAPPINGS::FILE_NOT_SUCCESSFULLY_READ\n" << FILE_PATH << std::endl;
	}
}

void ButtonMappings::print_mappings() {
	auto it = mappings.begin();

	while (it != mappings.end()) {
		std::cout << it->first << " " << it->second << "\n";
		
		it++;
	}

	std::cout << "\n";
}