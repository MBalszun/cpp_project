#pragma once

#include <string>
#include <string_view>
#include <optional>

#include "ProjectType.h"

namespace mba {

constexpr bool my_is_capital( char c )
{
	return 'A' <= c && c <= 'Z';
}

inline char simple_to_lower( char c )
{
	if( 'A' <= c && c <= 'Z' ) {
		return c - 'A' + 'a';
	}
	return c;
}

inline std::string pascal_to_snake_case( std::string_view base )
{
	std::string ret;
	if( base.size() == 0 ) {
		return ret;
	}

	ret.push_back( simple_to_lower( base[0] ) );

	for( const char& c : base.substr( 1 ) ) {
		if( 'A' <= c && c <= 'Z' ) {
			ret.push_back( '_' );
			ret.push_back( c - 'A' + 'a' );
		} else {
			ret.push_back( c );
		}
	}
	return ret;
}

inline std::string toString( const std::string& s )
{
	return s;
}

inline std::string toString( ProjectType type )
{
	return to_string( type );
}

inline std::string toString( bool value )
{
	if( value ) {
		return "yes";
	} else {
		return "no";
	}
}

template <class T>
std::string toString( const std::optional<T>& v )
{
	if( v ) {
		return toString( v.value() );
	} else {
		return "[Not set]";
	}
}

} // namespace mba
