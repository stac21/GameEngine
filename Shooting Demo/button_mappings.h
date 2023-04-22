#pragma once

#include <glfw3.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <set>

#include "actions.h"

class ButtonMappings {
private:
	static const std::string FILE_PATH;
	static std::unordered_map<std::string, int> mappings;

	ButtonMappings();

public:
	// get the mapping that corresponds to a particular action
	static int get_mapping(const std::string& action);
	/* 
	* edit the mapping that corresponds to a particular action.
	* Returns action if new_mapping was not bound to another action,
	* and the other action if it was bound to another action.
	*/
	static std::string edit_mapping(const std::string& action, int new_mapping);

	// load the button mappings
	static void load_mappings();
	static void save_mappings();

	static void print_mappings();
};