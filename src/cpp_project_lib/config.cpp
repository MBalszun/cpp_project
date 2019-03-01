#include "config.h"

#include "arch.h"
#include "helpers.h"

#include <clara/clara.hpp>
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

template <class T>
T get_or( const cxxopts::ParseResult& cmd_line_options,
		  const std::string&          key,
		  const T&                    default_value )
{
	if( cmd_line_options.count( key ) > 0 ) {
		return cmd_line_options[key].as<T>();
	}
	return default_value;
}

auto parse_config_clara( int argc, char const* const* argv ) -> UserConfig
{
	UserConfig cfg{};

	const std::string type_option_string = to_string_short( ProjectType::exec ) + " | "
										   + to_string_short( ProjectType::lib ) + " | "
										   + to_string_short( ProjectType::lib_header_only );

	auto type_parse = [&]( const std::string& s ) {
		cfg.project_type = parse_ProjectType( s );
		if( cfg.project_type ) {
			return clara::ParserResult::ok( clara::ParseResultType::Matched );
		} else {
			return clara::ParserResult::runtimeError( "" );
		}
	};

	bool request_help = false;

	auto cli =
		clara::Arg( cfg.project_name, "Project name" ).required()
		| clara::Opt( [&]( const std::string& s ) { return type_parse( s ); },
					  "Project type" )["-t"]["--type"]( "One of (" + type_option_string + ")" )
		| clara::Opt( cfg.abbreviation, "Short form" )["-s"]["--short"]
		| clara::Opt( cfg.target_base_name, "Target name" )["-T"]["--target"]
		| clara::Opt( cfg.cpp_namespace, "Namespace usid in cmake" )["-n"]["--ns_cpp"]
		| clara::Opt( cfg.cmake_namespace, "Namespace used in c++" )["-N"]["--ns_cmake"]
		| clara::Opt( cfg.create_git, "Initialize as git repository" )["-g"]
		| clara::Help( request_help );

	auto result = cli.parse( clara::Args( argc, argv ) );
	if( !result ) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		std::cout << cli << std::endl;
		exit( 1 );
	}
	if( request_help ) {
		std::cout << cli << std::endl;
		exit( 0 );
	}

	return cfg;
}

std::string toString( const std::string& s )
{
	return s;
}

std::string toString( ProjectType type )
{
	return to_string( type );
}

std::string toString( bool value )
{
	if( value ) {
		return "yes";
	} else {
		return "no";
	}
}

template <class T>
std::string toString( const std::optional<T>& v )
{
	if( v ) {
		return toString( v.value() );
	} else {
		return "[Not set]";
	}
}

std::string to_string( const UserConfig& cfg )
{
	std::stringstream ss;

	// clang-format off
	ss << "\n Project name:        " << toString(cfg.project_name)
	   << "\n Project type:        " << toString(cfg.project_type)
	   << "\n Target name:         " << toString(cfg.target_base_name)
	   << "\n namespace:           " << toString(cfg.cpp_namespace)
	   << "\n cmake namespace:     " << toString(cfg.cmake_namespace)
		<< "\n create a git:       " << toString(cfg.create_git)
		<< "\n Abbreviation:       " << toString(cfg.abbreviation);

	// clang-format on

	return ss.str();
}

Config parse_config( int argc, char** argv )
{
	Config cfg;

	cxxopts::Options options( "cpp_project",
							  "A simple tool to create a standard project "
							  "layout for executables or libraries" );

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

	options.parse_positional( { "name" } );
	auto result = options.parse( argc, argv );

	if( result.count( "help" ) > 0 || result.count( "name" ) == 0 ) {
		std::cout << options.help();
		exit( 0 );
	}

	cfg.create_git = result.count( "git" ) > 0;

	cfg.prj_type     = parse_ProjectType( result["type"].as<std::string>() ).value();
	cfg.template_dir = get_template_directory();

	// by default, use current directory
	// if project name is specified, create appropriate sub-directory

	cfg.names.project        = result["name"].as<std::string>();
	cfg.project_dir          = fs::current_path() / cfg.names.project;
	cfg.names                = create_default_names( cfg.names.project );
	cfg.names.target         = get_or( result, "target", cfg.names.target );
	cfg.names.ns             = get_or( result, "namespace", cfg.names.ns );
	cfg.names.cmake_ns       = get_or( result, "cmake_namespace", cfg.names.cmake_ns );
	cfg.names.component_name = get_or( result, "module", cfg.names.component_name );

	const std::string default_link_name = cfg.names.cmake_ns + "::" + cfg.names.component_name
										  + ( cfg.prj_type == ProjectType::exec ? "_lib" : "" );

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

std::string get_generic_abbrev( const std::string_view full_name )
{
	std::string ret;
	if( full_name.empty() ) {
		return ret;
	}
	ret.push_back( full_name[0] );

	for( std::size_t i = 1; i < full_name.size(); ++i ) {
		if( my_is_capital( full_name[i] ) ) {
			ret.push_back( full_name[i] );
		}
		if( full_name[i] == '_' && i < full_name.size() - 1 ) {
			ret.push_back( full_name[i + 1] );
		}
	}

	return ret;
}

Config generate_full_config( const UserConfig& user_cfg )
{
	Config cfg;
	cfg.create_git    = user_cfg.create_git;
	cfg.prj_type      = user_cfg.project_type.value_or( ProjectType::exec );
	cfg.names.project = user_cfg.project_name;

	const std::string base_name =
		user_cfg.target_base_name.value_or( pascal_to_snake_case( user_cfg.project_name ) );
	const std::string abbrev = user_cfg.abbreviation.value_or( get_generic_abbrev(base_name) );

	cfg.names.cmake_ns       = user_cfg.cmake_namespace.value_or( capitalize_first( abbrev ) );
	cfg.names.ns             = user_cfg.cpp_namespace.value_or( my_tolower( abbrev ) );
	cfg.names.target = ( cfg.prj_type == ProjectType::exec ? "" : abbrev + '_' ) + base_name;
	cfg.names.component_name = base_name;
	cfg.names.cmake_link_target =
		cfg.names.cmake_ns + "::" + base_name + ( cfg.prj_type == ProjectType::exec ? "_lib" : "" );

	cfg.project_dir  = fs::current_path() / cfg.names.project;
	cfg.template_dir = get_template_directory();

	return cfg;
}

} // namespace mba
