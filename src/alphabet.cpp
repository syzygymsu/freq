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

std::string NormalizeWord(std::string_view s) {
  std::string res;
  res.reserve(s.size());
  std::transform(
      s.begin(),
      s.end(),
      std::back_inserter(res),
      &NormalizeChar);
  return res;
}

std::string RestoreWord(std::string_view s) {
  std::string res;
  res.reserve(s.size());
  std::transform(
      s.begin(),
      s.end(),
      std::back_inserter(res),
      &RestoreChar);
  return res;
}
