#pragma once

#include "arch.h"
#include "config.h"

#include <filesystem>
#include <regex>
#include <string>

namespace mba {

std::string my_tolower( const std::string& s );

std::string capitalize_first( const std::string& s );

void install_file( const std::filesystem::path& template_path,
				   const std::filesystem::path& dest_path,
				   const Config&                cfg );

void install_recursive( const std::filesystem::path& template_dir,
						const std::filesystem::path& dest,
						const Config&                cfg );

void merge_snippets_recursive( const std::filesystem::path& dir );

} // namespace mba
