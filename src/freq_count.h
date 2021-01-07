#pragma once

#include <string>
#include <vector>

using FreqCountItem = std::pair<std::string, size_t>;
using FreqCountResult = std::vector<FreqCountItem>;

FreqCountResult FreqCount(std::istream& input);
