file( GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp" )
target_sources(
	${$TARGET_NAME$}$
PRIVATE
	${SOURCE_FILES}
	# add further source files here
)