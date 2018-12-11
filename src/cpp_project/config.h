#pragma once

#include "ProjectType.h"

#include <filesystem>
#include <string>

namespace mba {

struct Names {
	std::string project;
	std::string target;
	std::string ns;
	std::string cmake_ns;
	std::string component_name;
	std::string cmake_link_target;
};

struct Config {
	ProjectType           prj_type;
	Names                 names;
	std::filesystem::path template_dir;
	std::filesystem::path project_dir;
	bool                  create_git;
};

std::string to_string( const Config& cfg );

auto get_template_directory() -> std::filesystem::path;
auto create_default_names( const std::string& project_name ) -> Names;
auto parse_config( int argc, char** argv ) -> Config;

} // namespace mba
