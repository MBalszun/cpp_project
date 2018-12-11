#include <cpp_project/util.hpp>



#include <catch2/catch.hpp>

#include <iostream>

using namespace mba;

TEST_CASE("say_hello", "[cpp_project_tests]")
{
	std::cout << hello() << std::endl;
	CHECK( hello() != nullptr );
}

