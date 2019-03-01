#pragma once

#include "ProjectType.h"

#include <filesystem>
#include <string>
#include <optional>

namespace mba {

struct UserConfig {
	std::string project_name;
	std::optional<ProjectType> project_type;
	std::optional<std::string> target_base_name;
	std::optional<std::string> abbreviation;
	std::optional<std::string> cpp_namespace;
	std::optional<std::string> cmake_namespace;
	bool                       create_git;
};

std::string to_string( const UserConfig& cfg );

struct Config2 {
	UserConfig user_Config;
};

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


Config generate_full_config( const UserConfig& user_cfg );

auto get_template_directory() -> std::filesystem::path;
auto create_default_names( const std::string& project_name ) -> Names;

auto parse_config( int argc, char** argv ) -> Config;

auto parse_config_clara( int argc, char const * const* argv ) -> UserConfig;

} // namespace mba
