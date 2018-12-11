#include <cpp_project/helpers.h>

#include <catch2/catch.hpp>

using namespace mba;

TEST_CASE( "my_tolower", "[gen_cpp_prj_tests]" )
{
	CHECK( my_tolower("") == "" );
	CHECK( my_tolower( "hello world" ) == "hello world" );
	CHECK( my_tolower( "Hello World" ) == "hello world" );
}

TEST_CASE( "capitalize_first", "[gen_cpp_prj_tests]" )
{
	CHECK( capitalize_first( "" ) == "" );
	CHECK( capitalize_first( " " ) == " " );
	CHECK( capitalize_first( "T" ) == "T" );
	CHECK( capitalize_first( "t" ) == "T" );
	CHECK( capitalize_first( "hello world" ) == "Hello world" );
	CHECK( capitalize_first( "Hello World" ) == "Hello World" );
}
