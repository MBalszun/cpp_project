cmake_minimum_required(VERSION 3.10)
project(cpp_project  LANGUAGES CXX)

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

########## Lookup libraries ##################################################

# find_package(<package>)
# add_subdirectory( <libs/libname>)

########## Generate Executable  ##############################################

add_subdirectory( src )

########## Testing ###########################################################
option(cpp_project_INCLUDE_TESTS "Generate targets in test directory" ON)

if( cpp_project_INCLUDE_TESTS )
	enable_testing()
	add_subdirectory( tests )
endif()

