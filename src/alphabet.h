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

char Normalize(char c);
char Restore(char c);

std::string Normalize(std::string_view s);
std::string Restore(std::string_view s);
