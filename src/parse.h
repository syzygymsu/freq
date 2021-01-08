#pragma once

#include <iostream>

#include "alphabet.h"

template<typename Callback>
void Parse(std::istream& input, Callback&& callback) {
  const size_t buffer_size = 1000000;
  std::vector<char> buffer(buffer_size);
  size_t chunk_start = 0;  // begin of unprocessed buffer
  size_t chunk_size = 0;  // end of unprocessed buffer
  size_t word_start = 0;  // start of current word
  std::string overflow_word;

  auto ReadNextChunk = [&buffer, &chunk_start, &chunk_size, &word_start, &input]() -> void {
    input.read(buffer.data(), buffer.size());
    chunk_start = 0;
    chunk_size = input.gcount();
  };

  auto NormalizeToNextSpace = [&buffer, &chunk_start, &chunk_size]() -> bool {
    for (; chunk_start < chunk_size; ++chunk_start) {
      buffer[chunk_start] = NormalizeChar(buffer[chunk_start]);  // TODO: use lookup table to normalize chars
      if (normalized::kSpace == buffer[chunk_start]) {
        ++chunk_start;
        return true;
      }
    }
    return false;
  };

  for(;;) {
    if (chunk_start == chunk_size) {
      if (!input.good()) {
        break;
      }
      ReadNextChunk();
      word_start = 0;
    }

    // Process buffer, normalizing inplace
    while (NormalizeToNextSpace()) {
      const size_t len = chunk_start - word_start - 1;
      if (len > 0) {
        callback(std::string_view(buffer.data() + word_start, len));
      }
      word_start = chunk_start;
    }

    if (word_start != chunk_size) {
      // Process word split between buffers
      overflow_word = std::string_view(buffer.data() + word_start, chunk_size - word_start);

      for(;;) {
        if (!input.good()) {
          break;
        }
        ReadNextChunk();

        if (NormalizeToNextSpace()) {
          overflow_word += std::string_view(buffer.data(), chunk_start - 1);
          callback(std::string_view(overflow_word));
          overflow_word.clear();
          word_start = chunk_start;
          break;
        } else {
          overflow_word += std::string_view(buffer.data(), chunk_size);
        }
      }

      // Handle leftovers
      if (!overflow_word.empty()) {
        callback(std::string_view(overflow_word));
      }
    }
  }
}
