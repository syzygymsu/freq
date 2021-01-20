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
  CountersMap counters = CreateCounters();

  Parse(input, [&words, &counters](std::string_view word) {
    auto it = counters.find(word);
    if (it == counters.end()) {
      words.push_back(std::string(word));
      counters.emplace(std::string_view(words.back()), 1);
    } else {
      ++it->second;
    }
  });

  FreqCountResult res(counters.begin(), counters.end());

  {
    char restore_table[normalized::kMax];
    for (char c = 0; c <= normalized::kMax; ++c) {
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
