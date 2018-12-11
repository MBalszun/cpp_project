#include "../arch.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <filesystem>

namespace mba {
// https://stackoverflow.com/questions/4031672/without-access-to-argv0-how-do-i-get-the-program-name
std::filesystem::path GetExeFileName()
{
	std::string buffer( 300, ' ' );

	auto cnt = readlink( "/proc/self/exe", buffer.data(), buffer.size() - 1 );
	while( cnt == ( (std::ptrdiff_t)buffer.size() ) - 1 ) {
		buffer.resize( buffer.size() * 2 );
		cnt = readlink( "/proc/self/exe", buffer.data(), buffer.size() - 1 );
	}
	if( cnt == -1 ) {
		return {};
	} else {
		return buffer;
	}
}

std::filesystem::path get_exec_directory()
{
	return GetExeFileName().parent_path();
}

} // namespace mba
