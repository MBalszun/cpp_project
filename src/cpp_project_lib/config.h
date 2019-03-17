#pragma once

#include "ProjectType.h"

#include <filesystem>
#include <optional>
#include <regex>
#include <string>
#include <string_view>

namespace mba {

inline std::regex make_text_matcher( std::string_view template_base_string )
{
	auto pattern = R"(\$\{\$)" + std::string( template_base_string ) + R"(\$\}\$)";
	return std::regex( pattern, std::regex::optimize );
}

inline std::regex make_file_name_matcher( std::string_view pattern )
{
	return std::regex( std::string(pattern), std::regex::optimize );
}

struct PropertyInfo {
	PropertyInfo( std::string_view str )
		: base_string( str )
		, text_matcher( make_text_matcher(str) )
		, file_name_matcher( make_file_name_matcher( str ) )
	{
	}
	std::string_view base_string;
	std::regex       text_matcher;
	std::regex       file_name_matcher;
};

const PropertyInfo ProjectName{ "PROJECT_NAME"  };
const PropertyInfo TargetName{  "TARGET_NAME" };
const PropertyInfo LinkName{  "CMAKE_TARGET_LINK_NAME"  };
const PropertyInfo IncludeDirName{  "INCLUDE_DIR_NAME"  };
const PropertyInfo CMakeNamespace{  "CMAKE_NAMESPACE"  };
const PropertyInfo CppNamespace{  "CPP_NAMESPACE"  };
const PropertyInfo Acronym{  "ACRONYM"  };


// This is what the user can specify
struct BaseConfig {
	std::string project_name;
	std::string target_base_name;
	std::string abbreviation;
	std::string cpp_namespace;
	std::string cmake_namespace;
	std::string public_include_dir;

	ProjectType project_type;
	bool        create_git;
};

// This is what the user did specify
struct UserConfig {
	std::string                project_name;
	std::optional<std::string> target_base_name;
	std::optional<std::string> abbreviation;
	std::optional<std::string> cpp_namespace;
	std::optional<std::string> cmake_namespace;
	std::optional<std::string> public_include_dir;

	std::optional<ProjectType> project_type;
	bool                       create_git;
};

std::string to_string( const UserConfig& cfg );

auto parse_config_clara( int argc, char const* const* argv ) -> UserConfig;

std::string to_string( const BaseConfig& cfg );

BaseConfig generate_base_config( const UserConfig& user_cfg );

struct DerivedConfigProperties {

	std::string           target;
	std::string           cmake_link_target;
	std::string           include_dir;
	std::filesystem::path template_dir;
	std::filesystem::path project_dir;
};

DerivedConfigProperties generate_derived_config( const BaseConfig& base );

struct FullConfig : BaseConfig, DerivedConfigProperties {
};

FullConfig generate_full_config( const UserConfig& user_cfg );

struct Names {
	std::string project;
	std::string target;
	std::string ns;
	std::string cmake_ns;
	std::string component_name;
	std::string cmake_link_target;
	std::string include_dir;
};

struct Config {
	ProjectType           prj_type;
	Names                 names;
	std::filesystem::path template_dir;
	std::filesystem::path project_dir;
	bool                  create_git;
};

Config to_old_config( const FullConfig& cfg );

std::string to_string( const Config& cfg );

} // namespace mba
