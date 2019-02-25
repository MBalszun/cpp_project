#cmake_minimum_required(VERSION 3.10)

add_subdirectory( cpp_project_lib )

add_executable(	cpp_project main.cpp )
target_link_libraries( cpp_project Mba::cpp_project_lib)

add_custom_command(TARGET cpp_project POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/../templates"
     "$<TARGET_FILE_DIR:cpp_project>/cpp_project_templates"
  COMMENT "Copying templates to output directory")

########## Installation  #####################################################

# TODO move to top level cmake file	(currently INSTALL target can only install targets from the current directory)

INSTALL( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../templates/" DESTINATION "./cpp_project_templates" )
INSTALL( TARGETS cpp_project RUNTIME DESTINATION ".")

