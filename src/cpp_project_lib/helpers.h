#pragma once

#include "arch.h"
#include "config.h"

#include <filesystem>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

namespace mba {

std::string my_tolower( const std::string& s );

std::string capitalize_first( const std::string& s );

void install_file( const std::filesystem::path& template_path,
				   const std::filesystem::path& dest_path,
				   const Config&                cfg );

std::vector<std::filesystem::path>
install_recursive( const std::filesystem::path& template_dir, const std::filesystem::path& dest, const Config& cfg );

void merge_snippets_recursive( const std::filesystem::path& dir );

void merge_snippets_into_files( const std::vector<std::filesystem::path>& files );

template<class T>
void merge( std::vector<T>& base, std::vector<T>&& addition )
{
	base.insert( base.end(), std::make_move_iterator( addition.begin() ), std::make_move_iterator( addition.end() ) );
}

inline std::string strip_ending_newline( std::string&& base )
{
	if( base.size() == 0 ) {
		return std::move( base );
	}
	if( base.back() == '\n' ) {
		base.resize( base.size() - 1 );
	}
	if( base.size() == 0 ) {
		return std::move( base );
	}
	if( base.back() == '\r' ) {
		base.resize( base.size() - 1 );
	}
	return std::move( base );
}

constexpr bool my_is_capital( char c )
{
	return 'A' <= c && c <= 'Z';
}

inline char simple_to_lower(char c) {
	if ('A' <= c && c <= 'Z') {
		return c - 'A' + 'a';
	}
	return c;
}

inline std::string pascal_to_snake_case(std::string_view base) {
	std::string ret;
	if (base.size() == 0) {
		return ret;
	}

	ret.push_back(simple_to_lower(base[0]));

	for (const char& c : base.substr(1)) {
		if ('A' <= c && c <= 'Z') {
			ret.push_back( '_' );
			ret.push_back( c - 'A' + 'a' );
		} else {
			ret.push_back( c );
		}

	}
	return ret;
}

} // namespace mba
