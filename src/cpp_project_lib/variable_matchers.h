#pragma once

#include <regex>

namespace mba {

// clang-format off
const std::regex regex_prj(			R"--(\$\{\$PROJECT_NAME\$\}\$)--"			, std::regex::optimize );
const std::regex regex_target(		R"--(\$\{\$TARGET_NAME\$\}\$)--"			, std::regex::optimize );
const std::regex regex_ns(			R"--(\$\{\$NAMESPACE\$\}\$)--"				, std::regex::optimize );
const std::regex regex_cmake_ns(	R"--(\$\{\$CMAKE_NAMESPACE\$\}\$)--"		, std::regex::optimize );
const std::regex regex_link_target( R"--(\$\{\$CMAKE_TARGET_LINK_NAME\$\}\$)--"	, std::regex::optimize );
const std::regex regex_include_dir( R"--(\$\{\$INCLUDE_DIR_NAME\$\}\$)--"	    , std::regex::optimize );

const std::regex regex_project_filename(	R"--(PROJECT_NAME)--"	, std::regex::optimize );
const std::regex regex_target_filename(		R"--(TARGET_NAME)--"	, std::regex::optimize );
//const std::regex regex_component_filename(	R"--(COMPONENT_NAME)--"	, std::regex::optimize );
const std::regex regex_include_dir_filename(R"--(INCLUDE_DIR_NAME)--"	, std::regex::optimize );
// clang-format on

const std::regex regex_cmake_public_visibility( R"--(\$\{\$CMAKE_PUBLIC_VISIBILITY\$\}\$)--", std::regex::optimize );

const std::regex regex_cmake_library_definition_snippet( R"--(\$\{\$SNIPP_\$(.*)\$\$\}\$)--", std::regex::optimize );

} // namespace mba
