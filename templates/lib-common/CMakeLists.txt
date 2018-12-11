cmake_minimum_required( VERSION 3.10 )
project( ${$PROJECT_NAME$}$ LANGUAGES CXX )

option( ${$PROJECT_NAME$}$_INCLUDE_TESTS "Generate targets in test directory" ON )

#file( GLOB_RECURSE SOURCE_FILES "src/*.cpp" )
#add_library(
#	${$TARGET_NAME$}$
#	${SOURCE_FILES}
#	# add further source files here
#)

${$SNIPP_$CMAKE_LIBRARY_DEF.cmake$$}$

target_include_directories(
	${$TARGET_NAME$}$
${$CMAKE_PUBLIC_VISIBILITY$}$
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(
	${$TARGET_NAME$}$
${$CMAKE_PUBLIC_VISIBILITY$}$
	cxx_std_17
)

if( ${$PROJECT_NAME$}$_INCLUDE_TESTS )
	enable_testing()
	add_subdirectory( tests )
endif()

add_library( ${$CMAKE_TARGET_LINK_NAME$}$ ALIAS ${$TARGET_NAME$}$ )
