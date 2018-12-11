${$SNIPP_$INCLUDE_HELLO.cpp$$}$

#include <catch2/catch.hpp>

#include <iostream>

using namespace ${$NAMESPACE$}$;

TEST_CASE("say_hello", "[${$TARGET_NAME$}$_tests]")
{
	std::cout << hello() << std::endl;
	CHECK( hello() != nullptr );
}