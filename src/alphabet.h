#pragma once

#include <string>
#include <string_view>

namespace normalized {

static constexpr char kCharMin = char{0};
static constexpr char kCharMax = char{25};
static constexpr char kNumChars = char{26};

static constexpr char kSpace = char{26};
static constexpr char kMin = char{0};
static constexpr char kMax = char{26};

size_t ShortWordToIdx(std::string_view s);
std::string IdxToShortWord(size_t idx, size_t len);

}  // namespace normalized

char NormalizeChar(char c);
char RestoreChar(char c);
std::string RestoreString(std::string_view s);
