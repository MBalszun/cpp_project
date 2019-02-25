cmake_minimum_required(VERSION 3.10)

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

ParseAndAddCatchTests(cpp_project_tests)   
