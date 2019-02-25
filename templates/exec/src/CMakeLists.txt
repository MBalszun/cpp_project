add_subdirectory( ${$TARGET_NAME$}$_lib )

add_executable(	${$TARGET_NAME$}$ main.cpp )
target_link_libraries( ${$TARGET_NAME$}$ ${$CMAKE_TARGET_LINK_NAME$}$ )
