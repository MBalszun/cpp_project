#include "../arch.h"

#include <string>
#include <windows.h>

namespace mba {

std::filesystem::path GetExeFileName()
{
	std::wstring buffer( MAX_PATH, ' ' );

	auto cnt = GetModuleFileNameW( NULL, buffer.data(), (int)buffer.size() );

	while( ( cnt != 0 ) && ( GetLastError() == ERROR_INSUFFICIENT_BUFFER ) ) {
		buffer.resize( buffer.size() * 2 );
		cnt = GetModuleFileNameW( NULL, buffer.data(), (int)buffer.size() );
	}

	if( cnt == 0 ) {
		return std::filesystem::path{};
	}

	return std::filesystem::canonical( std::filesystem::path( buffer ) );
}

std::filesystem::path get_exec_directory()
{
	return GetExeFileName().parent_path();
}

} // namespace mba
