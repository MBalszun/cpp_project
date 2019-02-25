#include <cpp_project_lib/ProjectType.h>

#include <catch2/catch.hpp>

TEST_CASE( "project_type_round_trips", "[gen_cpp_prj_tests]" )
{
	CHECK( mba::ProjectType::exec == mba::parse_ProjectType( to_string( mba::ProjectType::exec ) ) );
	CHECK( mba::ProjectType::lib == mba::parse_ProjectType( to_string( mba::ProjectType::lib) ) );
	CHECK( mba::ProjectType::lib_header_only
		   == mba::parse_ProjectType( to_string( mba::ProjectType::lib_header_only ) ) );
}
