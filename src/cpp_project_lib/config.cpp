#include "config.h"

#include "arch.h"
#include "helpers.h"

#include <cxxopts.hpp>

#include <filesystem>
#include <sstream>

namespace mba {

namespace fs = std::filesystem;

std::filesystem::path get_template_directory()
{
	return get_exec_directory() / "cpp_project_templates";
}

Names create_default_names( const std::string& project_name )
{
	Names names;
	names.project           = project_name;
	names.target            = my_tolower( project_name );
	names.ns                = my_tolower( project_name );
	names.cmake_ns          = capitalize_first( project_name );
	names.component_name    = my_tolower( project_name );
	names.cmake_link_target = names.cmake_ns + "::" + names.component_name;
	return names;
}

template<class T>
T get_or( const cxxopts::ParseResult& cmd_line_options, const std::string& key, const T& default_value )
{
	if( cmd_line_options.count( key ) > 0 ) {
		return cmd_line_options[key].as<T>();
	}
	return default_value;
}

Config parse_config( int argc, char** argv )
{
	Config cfg;

	cxxopts::Options options( "cpp_project",
							  "A simple tool to create a standard project layout for executables or libraries" );

	// clang-format off
	const std::string type_option_string =
		to_string_short(ProjectType::exec) + " | "	+
		to_string_short(ProjectType::lib) + " | "	+
		to_string_short(ProjectType::lib_header_only);

	options.add_options()
		("h,help",          "print this documentation")
		("N,name",          "Project name",										   cxxopts::value<std::string>() )
		("t,type",          "Project type ( "+type_option_string+" )",			   cxxopts::value<std::string>()->default_value( to_string_short( ProjectType::exec ) ) )
		("T,target",        "Target name",                                         cxxopts::value<std::string>() )
		("n,namespace",     "namespace used in the library",                       cxxopts::value<std::string>() )
		("c,cmake_namespace", "namespace for the cmake",                           cxxopts::value<std::string>() )
		("m,module",        "component name inside cmake namespace",               cxxopts::value<std::string>() )
		("l,link_target",   "target name used by cmake to link to the library",    cxxopts::value<std::string>() )
		("g,git",           "creates a git repository (requires git to be installed)" );
	// clang-format on

	options.parse_positional( {"name"} );
	auto result = options.parse( argc, argv );

	if( result.count( "help" ) > 0 || result.count("name") == 0) {
		std::cout << options.help();
		exit( 0 );
	}

	cfg.create_git = result.count( "git" ) > 0;

	cfg.prj_type     = parse_ProjectType( result["type"].as<std::string>() ).value();
	cfg.template_dir = get_template_directory();

	// by default, use current directory
	// if project name is specified, create appropriate sub-directory

	cfg.names.project = result["name"].as<std::string>();
	cfg.project_dir   = fs::current_path() / cfg.names.project;
	cfg.names                = create_default_names( cfg.names.project );
	cfg.names.target         = get_or( result, "target", cfg.names.target );
	cfg.names.ns             = get_or( result, "namespace", cfg.names.ns );
	cfg.names.cmake_ns       = get_or( result, "cmake_namespace", cfg.names.cmake_ns );
	cfg.names.component_name = get_or( result, "module", cfg.names.component_name );

	const std::string default_link_name
		= cfg.names.cmake_ns + "::" + cfg.names.component_name + ( cfg.prj_type == ProjectType::exec ? "_lib" : "" );

	cfg.names.cmake_link_target = get_or( result, "l", default_link_name );

	for( int i = 0; i < int( cfg.names.cmake_link_target.size() - 1 ); ++i ) {
		if( cfg.names.cmake_link_target[i] == ':' && cfg.names.cmake_link_target[i + 1] == ':' ) {
			cfg.names.component_name = cfg.names.cmake_link_target.substr( i + 2 );
			break;
		}
	}
	return cfg;
}

std::string to_string( const Config& cfg )
{
	std::stringstream ss;

	// clang-format off
	ss << "\n Project name:          " << cfg.names.project
	   << "\n Project directory:     " << cfg.project_dir
	   << "\n Template directory :   " << cfg.template_dir
	   << "\n Target name:           " << cfg.names.target
	   << "\n namespace:             " << cfg.names.ns
	   << "\n cmake namespace:       " << cfg.names.cmake_ns
	   << "\n cmake component name:  " << cfg.names.component_name
	   << "\n cmake link target:     " << cfg.names.cmake_link_target;
	// clang-format on

	return ss.str();
}

} // namespace mba
