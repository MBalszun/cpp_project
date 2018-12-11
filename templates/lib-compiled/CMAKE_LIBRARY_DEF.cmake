file( GLOB_RECURSE SOURCE_FILES "src/*.cpp" )
add_library(
	${$TARGET_NAME$}$
	${SOURCE_FILES}
	# add further source files here
)