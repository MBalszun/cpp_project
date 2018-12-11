cmake_minimum_required(VERSION 3.10)

add_subdirectory( cpp_project )

add_executable(	cpp_project main.cpp )
target_link_libraries( cpp_project Mba::cpp_project_lib)

