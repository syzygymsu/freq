#pragma once

#include <iostream>

#include "alphabet.h"

template<typename Callback>
void Parse(std::istream& input, Callback&& callback) {
  std::string buffer;
  char c;
  while (input.get(c).good()) {
    c = NormalizeChar(c);  // TODO: use lookup table to normalize chars
    if (c == normalized::kSpace) {
      if (!buffer.empty()) {
        callback(buffer);
        buffer.clear();
      }
    } else {
      buffer.push_back(c);
    }
  }

  if (!buffer.empty()) {
    callback(buffer);
  }
}
