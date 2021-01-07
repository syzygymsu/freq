#include "alphabet.h"

#include <algorithm>

char Normalize(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a';
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  }
  return normalized::kSpace;
}

char Restore(char c) {
  if(c >= normalized::kCharMin && c <= normalized::kCharMax) {
    return 'a' + c - normalized::kCharMin;
  }
  return ' ';
}

std::string Normalize(std::string_view s) {
  std::string res;
  res.reserve(s.size());
  std::transform(
      s.begin(),
      s.end(),
      std::back_inserter(res),
      static_cast<char(*)(char)>(&Normalize));
  return res;
}

std::string Restore(std::string_view s) {
  std::string res;
  res.reserve(s.size());
  std::transform(
      s.begin(),
      s.end(),
      std::back_inserter(res),
      static_cast<char(*)(char)>(&Restore));
  return res;
}
