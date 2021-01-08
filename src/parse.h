#pragma once

#include <iostream>

#include "alphabet.h"

template<typename Callback>
void Parse(std::istream& input, Callback&& callback) {
  const size_t buffer_size = 1000000;
  std::vector<char> buffer(buffer_size);

  std::string word;
  do {
    input.read(buffer.data(), buffer.size());
    size_t chunk_size = input.gcount();
    for (size_t i = 0; i < chunk_size; ++i) {
      char c = NormalizeChar(buffer[i]);  // TODO: use lookup table to normalize chars
      if (c == normalized::kSpace) {
        if (!word.empty()) {
          callback(word);
          word.clear();
        }
      } else {
        word.push_back(c);
      }
    }
  } while(input.good());

  if (!word.empty()) {
    callback(word);
  }
}
