#include "freq_count.h"

#include <algorithm>
#include <list>
#include <unordered_map>

#include "parse.h"

FreqCountResult FreqCount(std::istream& input) {
  std::list<std::string> words;
  std::unordered_map<std::string_view, size_t> counters;
  counters.reserve(100000);

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
      restore_table[c] = RestoreChar(c);
    }

    for (FreqCountItem &item : res) {
      std::transform(
          item.first.begin(),
          item.first.end(),
          item.first.begin(),
          [&restore_table](char c) { return restore_table[c]; });
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
