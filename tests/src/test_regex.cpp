#include <cpp_project_lib/variable_matchers.h>

#include <catch2/catch.hpp>
#include <iostream>

using namespace mba;

TEST_CASE( "snippet", "[gen_cpp_prj_tests][regex]" )
{
	std::cmatch match;
	CHECK( std::regex_search( "Hello ${$SNIPP_$hello$$}$", match, regex_cmake_library_definition_snippet ) );
	std::csub_match sub_match = match[1];
	CHECK( match.str() == "${$SNIPP_$hello$$}$");
	CHECK( sub_match.str() == "hello" );
	CHECK( !std::regex_search( "${$SNIPP_$}$", match, regex_cmake_library_definition_snippet ) );
	CHECK( !std::regex_search( "${$SNIP_$hello$$}$", match, regex_cmake_library_definition_snippet ) );

}


