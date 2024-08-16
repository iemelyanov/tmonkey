#pragma once

#include "ast.h"
#include "common.h"

namespace tmonkey {

auto parse(std::string_view source, Arena& arena) -> std::vector<AstNode*>;

}  // namespace tmonkey