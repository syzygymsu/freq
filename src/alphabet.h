#pragma once

#include <string>
#include <string_view>

namespace normalized {

static constexpr char kCharMin = char{0};
static constexpr char kCharMax = char{25};
static constexpr char kSpace = char{26};
static constexpr char kMin = char{0};
static constexpr char kMax = char{26};

}

char NormalizeChar(char c);
char RestoreChar(char c);

std::string NormalizeWord(std::string_view s);
std::string RestoreWord(std::string_view s);