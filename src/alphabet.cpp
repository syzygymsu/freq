#include "alphabet.h"

#include <algorithm>

char NormalizeChar(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a';
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  }
  return normalized::kSpace;
}

char RestoreChar(char c) {
  if(c >= normalized::kCharMin && c <= normalized::kCharMax) {
    return 'a' + c - normalized::kCharMin;
  }
  return ' ';
}

std::string RestoreString(std::string_view s) {
  std::string res(s);
  for (char& c : res) {
    c = RestoreChar(c);
  }
  return res;
}

namespace normalized {

size_t ShortWordToIdx(std::string_view s) {
  size_t res = 0;
  for (char c : s) {
    res *= kNumChars;
    res += c;
  }
  return res;
}

std::string IdxToShortWord(size_t idx, size_t len) {
  std::string res(len, char{0});
  for (size_t i = 0; i < len; ++i) {
    res[len - 1 - i] = idx % kNumChars;
    idx /= kNumChars;
  }

  return res;
}

}  // namespace normalized
