cmake_minimum_required(VERSION 3.10)

########## Generate Proxy Library
# We put all source files (except main) into a separate library
# this Library is then linked to the main target -- makes testing simpler

set(arch $<IF:$<BOOL:${WIN32}>,win,linux>)

file(GLOB SOURCE_FILES "*.cpp")

add_library( cpp_project_lib
	STATIC
		${SOURCE_FILES}
		${CMAKE_CURRENT_LIST_DIR}/${arch}/arch.cpp
)

target_include_directories	( cpp_project_lib INTERFACE .. )

target_link_libraries(
	cpp_project_lib
PUBLIC
	cxxopts::cxxopts
	# just a guess that we ar using libstdc++: gcc 8 requires the filesystem library to be linked explicitly
	$<IF:$<BOOL:${WIN32}>,,stdc++fs>
)

# create link target (e.g. for testing)
add_library( Mba::cpp_project_lib ALIAS cpp_project_lib )

