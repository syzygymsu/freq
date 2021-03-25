#include "freq_counter.h"

#include <algorithm>
#include <deque>
#include <unordered_map>

#include "parse.h"

size_t FreqCounter::Hash::operator()(std::string_view s) const {
  return
      seed_
      ? std::_Hash_impl::hash(s.data(), s.size(), *seed_)
      : std::_Hash_impl::hash(s.data(), s.size());
}

FreqCounter::CountersMap
FreqCounter::CreateCounters() const {
  CountersMap counters(0, Hash(config_.hash_seed));
  counters.reserve(config_.initial_capacity);
  return counters;
}

FreqCountResult FreqCounter::Count(std::istream& input) const {
  std::deque<std::string> words;

  static const size_t kShortSize = 5;  // optimized for 100MB-ish files

  std::array<std::vector<size_t>, kShortSize + 1> short_counters;
  CountersMap long_counters = CreateCounters();

  // init counters
  size_t counters_size = 1;
  for (size_t len = 0; len <= kShortSize; ++len) {
    short_counters[len].resize(counters_size, 0);
    counters_size *= normalized::kNumChars;
  }

  // parse & count
  Parse(input, [&words, &short_counters, &long_counters](std::string_view word) {
    if (word.size() <= kShortSize) {
      short_counters[word.size()][normalized::ShortWordToIdx(word)]++;
      return;
    }

    auto it = long_counters.find(word);
    if (it == long_counters.end()) {
      words.push_back(std::string(word));
      long_counters.emplace(std::string_view(words.back()), 1);
    } else {
      ++it->second;
    }
  });

  // collect results
  FreqCountResult res(long_counters.begin(), long_counters.end());
  for (size_t len = 0; len <= kShortSize; ++len) {
    for (size_t idx = 0; idx < short_counters[len].size(); ++idx) {
      size_t count = short_counters[len][idx];
      if (count) {
        res.emplace_back(normalized::IdxToShortWord(idx, len), count);
      }
    }
  }

  {
    char restore_table[normalized::kMax];
    for (char c = 0; c < normalized::kMax; ++c) {
      restore_table[static_cast<unsigned char>(c)] = RestoreChar(c);
    }

    for (FreqCountItem &item : res) {
      std::transform(
          item.first.begin(),
          item.first.end(),
          item.first.begin(),
          [&restore_table](char c) { return restore_table[static_cast<unsigned char>(c)]; });
    }
  }

  std::sort(res.begin(), res.end(), [](const FreqCountItem& l, const FreqCountItem& r) {
    if(l.second != r.second) {
      return l.second > r.second;
    }
    return l.first < r.first;
  });

  return res;
}
