#include "freq_count.h"

#include <algorithm>
#include <unordered_map>

#include "parse.h"

FreqCountResult FreqCount(std::istream& input) {
  std::unordered_map<std::string, size_t> counters;

  Parse(input, [&counters](std::string_view word) {
    ++counters[std::string(word)];  // TODO: optimize map to use string_view
  });

  FreqCountResult res(counters.begin(), counters.end());

  std::transform(res.begin(), res.end(), res.begin(), [](FreqCountItem item) {
    item.first = RestoreWord(item.first);
    return item;
  });

  std::sort(res.begin(), res.end(), [](const FreqCountItem& l, const FreqCountItem& r) {
    if(l.second != r.second) {
      return l.second > r.second;
    }
    return l.first < r.first;
  });

  return res;
}
