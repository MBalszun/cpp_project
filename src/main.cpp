#include <cpp_project_lib/ProjectType.h>
#include <cpp_project_lib/config.h>
#include <cpp_project_lib/git.h>
#include <cpp_project_lib/helpers.h>

#include <cassert>
#include <charconv>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <filesystem>
#include <stdexcept>

namespace mba {
namespace fs = std::filesystem;

void install_project( const Config& cfg )
{
	const fs::path&        template_dir = cfg.template_dir;
	const mba::fs::path&   project_dir  = cfg.project_dir;
	const mba::ProjectType prj_type     = cfg.prj_type;

	std::vector<std::filesystem::path> installed_files;

	fs::create_directories( project_dir );
	merge( installed_files, install_recursive( template_dir / "common", project_dir, cfg ) );
	switch( prj_type ) {
		case ProjectType::exec:
			merge( installed_files, install_recursive( template_dir / "exec", project_dir, cfg ) );
			// create some empty
			fs::create_directories( project_dir / "src" );
			fs::create_directories( project_dir / "libs" );
			break;
		case ProjectType::lib:
			merge( installed_files, install_recursive( template_dir / "lib-common", project_dir, cfg ) );
			merge( installed_files, install_recursive( template_dir / "lib-compiled", project_dir, cfg ) );
			break;
		case ProjectType::lib_header_only:
			merge( installed_files, install_recursive( template_dir / "lib-common", project_dir, cfg ) );
			merge( installed_files, install_recursive( template_dir / "lib-header", project_dir, cfg ) );
			break;
		default: assert( false );
	}
	merge_snippets_recursive( project_dir );
}

} // namespace mba

using namespace mba;

const std::string post_build_message
	= "\n###################################################################"
	  "\n###      Project creation completed successfully!               ###"
	  "\n###################################################################"
	  "\n#"
	  "\n# To build your project and run test cases "
	  "\n# you can perform the following steps:"
	  "\n#"
	  "\n# - Create and switch to the directory you want to build in"
	  "\n# - cmake <Project directory> "
	  "\n# - cmake --build ."
	  "\n# - ctest . # or for MSVC: ctest . -C Debug"
	  "\n#"
	  "\n###################################################################"
	  "\n\n";

// example command : cpp_project_generator.exe -N flat_map -t lib -T mba_flat_map -n mba -c MBa -m flat_map -g
int main( int argc, char** argv )
{
	try {
		Config cfg = parse_config( argc, argv );

		std::cout << "This will create a \"" << to_string( cfg.prj_type )
				  << "\" project with the following configuration:\n"
				  << to_string( cfg ) << std::endl;

		std::cout << "\nProceede? (y/n)";

		char answer{};
		std::cin >> answer;
		if( answer != 'y' ) {
			return 1;
		}

		try {
			install_project( cfg );

			std::cout << post_build_message << std::endl;

			if( cfg.create_git ) {
				mba::git_init_dir( cfg.project_dir );
			}

		} catch( const std::exception& e ) {
			std::cout << "Error during project creation: \"" << e.what() << "\"" << std::endl;
		}

	} catch( const std::exception& e ) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}
