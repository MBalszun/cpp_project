#include "helpers.h"

#include "variable_matchers.h"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>

namespace mba {

namespace fs = std::filesystem;

// from https:// en.cppreference.com/w/cpp/string/byte/toupper
std::string my_tolower( const std::string& in )
{
	std::string s = in;
	std::transform(
		s.begin(), s.end(), s.begin(), []( unsigned char c ) { return static_cast<char>( std::tolower( c ) ); } );
	return s;
}

std::string capitalize_first( const std::string& in )
{
	std::string s = in;
	if( s.empty() ) return s;

	s[0] = static_cast<char>( std::toupper( static_cast<unsigned char>( s[0] ) ) );
	return s;
}

void replace_inplace( std::string& src, const std::regex& reg, const std::string& replace )
{
	thread_local std::string buffer;
	buffer.clear();
	std::regex_replace( std::back_inserter( buffer ), src.begin(), src.end(), reg, replace );
	src = buffer;
}

std::string get_file_content( const fs::path& src_path )
{
	std::ifstream in( src_path, std::ios::in | std::ios::binary );
	if( !in ) {
		throw std::runtime_error( std::string( "get_file_content: " ) + strerror( errno ) + "When accessing "
								  + src_path.string() );
	}
	return std::string( std::istreambuf_iterator<char>( in ), std::istreambuf_iterator<char>() );
}

void set_file_content( const fs::path& src_path, const std::string& text )
{
	std::ofstream out( src_path, std::ios::out | std::ios::binary | std::ios::trunc );
	if( !out ) {
		throw std::runtime_error( std::string( "set_file_content: " ) + strerror( errno ) + " When accessing "
								  + src_path.string() );
	}
	out.write( text.c_str(), text.size() );
}

void merge_snippet( const std::filesystem::path& file, const std::regex& reg, std::set<fs::path>& files_to_delete )
{
	std::string text = get_file_content( file );

	std::smatch base_match;
	if( !std::regex_search( text, base_match, reg ) ) {
		return;
	}
	if( base_match.size() != 2 ) {
		throw std::runtime_error( std::string( "Wrong format: " ) + base_match.str() );
	}
	// The first sub_match is the whole string; the next
	// sub_match is the first parenthesized expression.

	std::ssub_match base_sub_match = base_match[1];
	fs::path        snippet_file   = file.parent_path() / base_sub_match.str();
	replace_inplace( text, reg, get_file_content( snippet_file ) );
	set_file_content( file, text );
	files_to_delete.insert( snippet_file );
}

void merge_snippets_recursive( const std::filesystem::path& dir )
{
	std::set<fs::path> files_to_delete_later;
	for( auto d : fs::recursive_directory_iterator( dir ) ) {
		if( !d.is_directory() ) {
			merge_snippet( d, regex_cmake_library_definition_snippet, files_to_delete_later );
		}
	}
	for( const auto& f : files_to_delete_later ) {
		fs::remove( f );
	}
}

void install_file( const fs::path& template_path, const fs::path& dest_path, const Config& cfg )
{
	const Names& names = cfg.names;
	try {
		std::ifstream source( template_path, std::ios_base::in | std::ios_base::binary );
		if( !source.is_open() ) {
			throw std::runtime_error( "Could not open source file " + template_path.string() );
		}
		std::ofstream dest( dest_path.native(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );
		if( !dest.is_open() ) {
			throw std::runtime_error( "Could not open destination file " + dest_path.string() );
		}

		std::string line;
		while( std::getline( source, line ) ) {
			replace_inplace( line, regex_prj, names.project );
			replace_inplace( line, regex_target, names.target );
			replace_inplace( line, regex_ns, names.ns );
			replace_inplace( line, regex_link_target, names.cmake_link_target );
			replace_inplace( line, regex_cmake_ns, names.cmake_ns );
			if( cfg.prj_type == ProjectType::lib_header_only )
				replace_inplace( line, regex_cmake_public_visibility, "INTERFACE" );
			else
				replace_inplace( line, regex_cmake_public_visibility, "PUBLIC" );

			dest << line;
			dest.put( '\n' );
		}
		dest << std::endl;
	} catch( const std::exception& e ) {
		std::cout << "Error while installing file" << template_path.u8string() << "\n"
				  << "Error details: \n"
				  << e.what() << std::endl;
	}
}

void install_recursive( const fs::path& template_dir, const fs::path& dest, const Config& cfg )
{
	const Names& names = cfg.names;

	for( auto dir : fs::directory_iterator( template_dir ) ) {
		std::string filename = dir.path().filename().u8string();
		replace_inplace( filename, regex_project_filename, names.project );
		replace_inplace( filename, regex_target_filename, names.target );
		replace_inplace( filename, regex_component_filename, names.component_name );

		if( dir.is_directory() ) {
			auto new_dir = dest / filename;
			fs::create_directories( new_dir );
			install_recursive( dir, new_dir, cfg );
		} else {
			install_file( dir.path(), dest / filename, cfg );
		}
	}
}

} // namespace mba
