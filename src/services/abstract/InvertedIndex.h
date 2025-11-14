#pragma once

#include <string>
#include <unordered_map>
#include <set>

using InvertedIndex = std::unordered_map<std::string, std::set<std::string>>;
