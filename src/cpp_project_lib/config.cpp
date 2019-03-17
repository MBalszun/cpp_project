#include "config.h"

#include "arch.h"
#include "helpers.h"

#include <clara/clara.hpp>
//#include <cxxopts.hpp>

#include <filesystem>
#include <iostream>
#include <sstream>

namespace mba {

namespace fs = std::filesystem;

std::filesystem::path get_template_directory()
{
	return get_exec_directory() / "cpp_project_templates";
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


	// clang format off
	auto cli =
		clara::Arg( cfg.project_name,		"Project name" ).required()
		| clara::Opt( [&]( const std::string& s ) { return type_parse( s ); },
											"Project type" )				["-t"]["--type"]( "One of (" + type_option_string + ")" )
		| clara::Opt( cfg.abbreviation,		"Short form" )					["-s"]["--short"]
		| clara::Opt( cfg.target_base_name, "Target name" )					["-T"]["--target"]
		| clara::Opt( cfg.cpp_namespace,	"Namespace usid in cmake" )		["-n"]["--ns_cpp"]
		| clara::Opt( cfg.cmake_namespace,	"Namespace used in c++" )		["-N"]["--ns_cmake"]
		| clara::Opt( cfg.create_git,		"Initialize as git repository" )["-g"]
		| clara::Help( request_help );

	// clang format on

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
	   << "\n cmake link target:     " << cfg.names.cmake_link_target
	   << "\n include_dir:           " << cfg.names.include_dir;
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

Config to_old_config( const FullConfig& new_cfg )
{
	Config cfg;

	cfg.create_git              = new_cfg.create_git;
	cfg.prj_type                = new_cfg.project_type;
	cfg.names.project           = new_cfg.project_name;
	cfg.names.cmake_ns          = new_cfg.cmake_namespace;
	cfg.names.ns                = new_cfg.cpp_namespace;
	cfg.names.component_name    = new_cfg.target_base_name;
	cfg.names.include_dir       = new_cfg.include_dir;
	cfg.project_dir             = new_cfg.project_dir;
	cfg.template_dir            = new_cfg.template_dir;
	cfg.names.target            = new_cfg.target;
	cfg.names.cmake_link_target = new_cfg.cmake_link_target;

	return cfg;
}

// Those are the properties that cannot be specified by the user, but have to be derived

// Thios is where we assign values to everything the user hasn't specified on the command line
BaseConfig generate_base_config( const UserConfig& user_cfg )
{
	const auto&       name = user_cfg.project_name;
	const std::string abbrev =
		user_cfg.abbreviation.value_or( my_tolower( get_generic_abbrev( name ) ) );

	BaseConfig cfg;

	cfg.project_name     = name;
	cfg.create_git       = user_cfg.create_git;
	cfg.abbreviation     = abbrev;
	cfg.cmake_namespace  = user_cfg.cmake_namespace.value_or( capitalize_first( abbrev ) );
	cfg.cpp_namespace    = user_cfg.cpp_namespace.value_or( my_tolower( abbrev ) );
	cfg.project_type     = user_cfg.project_type.value_or( ProjectType::exec );
	cfg.target_base_name = user_cfg.target_base_name.value_or( //
		pascal_to_snake_case( user_cfg.project_name ) );

	return cfg;
}

DerivedConfigProperties generate_derived_config( const BaseConfig& base )
{
	DerivedConfigProperties cfg;

	if( base.project_type == ProjectType::exec ) {
		cfg.target            = base.target_base_name;
		cfg.cmake_link_target = base.cmake_namespace + "::" + base.target_base_name + "_lib";

	} else {
		cfg.target            = my_tolower( base.cmake_namespace ) + "_" + base.target_base_name;
		cfg.cmake_link_target = base.cmake_namespace + "::" + base.target_base_name;
	}

	cfg.include_dir  = my_tolower( base.abbreviation );
	cfg.project_dir  = fs::current_path() / base.project_name;
	cfg.template_dir = get_template_directory();

	return cfg;
}

FullConfig generate_full_config( const UserConfig& user_cfg )
{
	FullConfig cfg;
	static_cast<BaseConfig&>( cfg )    = generate_base_config( user_cfg );
	static_cast<DerivedConfigProperties&>( cfg ) = generate_derived_config( cfg );

	return cfg;
}

} // namespace mba
