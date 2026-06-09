#pragma once

#include <string>
#include <utility>
#include <vector>

bool load_dict_json(const std::string &path, std::vector<std::pair<std::string, std::string>> &entries);
