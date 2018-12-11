cmake_minimum_required(VERSION 3.10)

########## General Settings for the whole project ############################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	# Set warning level (CMAKE adds /W3 and msvc produces a warning, when we would just add /W4 )
	STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(
		/permissive-
	)
else()
	add_compile_options(-Wall -Wextra)
endif()

########## Generate test executable ##########################################
add_executable(
	cpp_project_tests
	main.cpp
)

# search for test source files (have to start with prefix test_)
file(GLOB_RECURSE TEST_FILES test_*.cpp)
target_sources(
	cpp_project_tests
PUBLIC
	${TEST_FILES}
)

find_package(Catch2 CONFIG REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(
	cpp_project_tests
PRIVATE
	Threads::Threads
	Catch2::Catch2
	Mba::cpp_project_lib
)

## Make ctest run build.
# idea taken from https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests
# TODO: DOES NOT WORK with MSVC open folder (${CMAKE_COMMAND} seems to be the problem, but a plain "cmake" doesn't pass the correct incldue directories)

ADD_TEST(NAME build_cpp_project_tests COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target cpp_project_tests)

include(ParseAndAddCatchTests)

set(PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS ON)
ParseAndAddCatchTests(cpp_project_tests)

