#include <cpp_project/config.h>

#include <catch2/catch.hpp>

using namespace mba;

TEST_CASE( "detect_name_as_positional_1", "[gen_cpp_prj_tests]" )
{
	char   arg0[] = "";
	char   arg1[] = "my_name1";
	char*  argv[2] = {arg0, arg1};

	Config cfg     = parse_config( 2, argv );

	CHECK( cfg.names.project == "my_name1" );
	CHECK( cfg.names.target == "my_name1" );
}

TEST_CASE( "detect_name_as_positional_2", "[gen_cpp_prj_tests]" )
{
	char   arg0[]  = "";
	char   arg1[]  = "-T";
	char   arg2[]  = "target_name";
	char   arg3[]  = "my_name2";
	char*  argv[4] = {arg0, arg1, arg2, arg3};

	Config cfg     = parse_config( 4, argv );

	CHECK( cfg.names.project == "my_name2" );
	CHECK( cfg.names.target == "target_name" );
}

