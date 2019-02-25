#include "git.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace mba {

void git_init_dir( std::filesystem::path dir )
{
	try {
		std::string cmd_cd         = "cd \"" + dir.generic_string() + "\"";
		std::string cmd_git_init   = "git init";
		std::string cmd_git_add    = "git add .";
		std::string cmd_git_commit = "git commit -m\"[Auto commit] Initial commit by cpp_project_generator\"";

		auto throw_on_fail = []( std::string s ) {
			std::cout << "Executing: " << s << std::endl;
			auto res = std::system( s.c_str() );
			if( res != 0 ) {
				throw std::runtime_error( "This command failed: " + s );
			}
		};

		throw_on_fail( cmd_cd );
		throw_on_fail( cmd_cd + " && " + cmd_git_init );
		throw_on_fail( cmd_cd + " && " + cmd_git_add );
		throw_on_fail( cmd_cd + " && " + cmd_git_commit );
	} catch( std::runtime_error& e ) {
		std::cout << "Error during creation of git directory: \n\n" << e.what() << "\n" << std::endl;
		throw;
	}
}

} // namespace mba
