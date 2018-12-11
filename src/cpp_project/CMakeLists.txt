cmake_minimum_required(VERSION 3.10)

########## Generate Proxy Library
# We put all source files (except main) into a separate library
# this Library is then linked to the main target -- makes testing simpler

file(GLOB_RECURSE SOURCE_FILES "*.cpp")

if(SOURCE_FILES)
	add_library				( cpp_project_lib	STATIC ${SOURCE_FILES} )
else()
	add_library				( cpp_project_lib	INTERFACE )
endif()

target_include_directories	( cpp_project_lib INTERFACE .. )

# create link target (e.g. for testing)
add_library( Mba::cpp_project_lib ALIAS cpp_project_lib )

