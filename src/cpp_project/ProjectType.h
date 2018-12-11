#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace mba {

enum class ProjectType { exec, lib, lib_header_only };

std::string                to_string( ProjectType type );
std::string                to_string_short( ProjectType type );
std::optional<ProjectType> parse_ProjectType( std::string_view str );

} // namespace mba
