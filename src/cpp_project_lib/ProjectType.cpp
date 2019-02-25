#include "ProjectType.h"

#include <stdexcept>

#include <cassert>

namespace mba {

std::string to_string( ProjectType type )
{
	switch( type ) {
		case ProjectType::exec: return "executable";
		case ProjectType::lib: return "library";
		case ProjectType::lib_header_only: return "library-header-only";
		default:
			assert( false );
			throw std::runtime_error( "Unkown project type:"
									  + std::to_string( (std::underlying_type_t<ProjectType>)type ) );
			break;
	}
}

std::string to_string_short( ProjectType type )
{
	switch( type ) {
		case ProjectType::exec: return "exec";
		case ProjectType::lib: return "lib";
		case ProjectType::lib_header_only: return "header";
		default:
			assert( false );
			throw std::runtime_error( "Unkown project type:"
									  + std::to_string( (std::underlying_type_t<ProjectType>)type ) );
			break;
	}
}

std::optional<ProjectType> parse_ProjectType( std::string_view str )
{
	if( ( str == to_string( ProjectType::exec ) ) || ( str == to_string_short( ProjectType::exec ) ) ) {
		return ProjectType::exec;
	} else if( ( str == to_string( ProjectType::lib ) ) || ( str == to_string_short( ProjectType::lib ) ) ) {
		return ProjectType::lib;
	} else if( ( str == to_string( ProjectType::lib_header_only ) )
			   || ( str == to_string_short( ProjectType::lib_header_only ) ) ) {
		return ProjectType::lib_header_only;
	}
	return std::nullopt;
}

} // namespace mba
