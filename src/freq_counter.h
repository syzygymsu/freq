#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using FreqCountItem = std::pair<std::string, size_t>;
using FreqCountResult = std::vector<FreqCountItem>;

class FreqCounter {
 public:
  struct Config {
    size_t initial_capacity = 100000;
    std::optional<size_t> hash_seed;
  };

  class Hash {
   public:
    Hash() {}
    Hash(std::optional<size_t> seed) : seed_(seed) {}

    size_t operator()(std::string_view) const;

   private:
    std::optional<size_t> seed_;
  };

  using CountersMap = std::unordered_map<std::string_view, size_t, Hash>;

  FreqCounter() : FreqCounter(Config{}) {}
  FreqCounter(Config config) : config_(config) {}
  const Config& GetConfig() const { return config_; }

  CountersMap CreateCounters() const;

  FreqCountResult Count(std::istream& input) const;

 private:
  Config config_;
};
