#include <chrono>
#include <iostream>

#include <boost/program_options.hpp>

#include "src/alphabet.h"
#include "src/freq_counter.h"

namespace po = boost::program_options;

void NextString(std::string& s) {
  for (char& c : s) {
    if (c != normalized::kCharMax) {
      c++;
      return;
    }

    c = normalized::kCharMin;
  }
  s.push_back(normalized::kCharMin);
}

std::vector<std::string> PickAttackPayload(FreqCounter::Config config) {
  auto start = std::chrono::high_resolution_clock::now();

  const FreqCounter freq_counter(config);
  const FreqCounter::CountersMap counters = freq_counter.CreateCounters();

  const size_t target_load = static_cast<size_t>(
      std::floor(counters.max_load_factor() * counters.bucket_count()));

  // Pass 1 -- find the most popular bucket
  size_t popular_bucket;
  size_t max_load = 0;
  std::string s(1, normalized::kCharMin);
  std::vector<size_t> bucket_counter(counters.bucket_count(), 0);
  for (;;) {
    size_t bucket = counters.bucket(std::string_view(s));
    if (++bucket_counter[bucket] >= target_load) {
      popular_bucket = bucket;
      break;
    }
    if (max_load < bucket_counter[bucket]) {
      max_load = bucket_counter[bucket];
      std::cerr << "Search: " << max_load << " / " << target_load  << "\r";
    }
    NextString(s);
  }
  std::cerr << std::endl;

  // Pass 2 -- gather items for selected bucket
  std::vector<std::string> res;
  s = std::string(1, normalized::kCharMin);
  while(res.size() < target_load) {
    if (popular_bucket == counters.bucket(std::string_view(s))) {
      res.push_back(RestoreString(s));
      std::cerr << "Gather: " << res.size() << " / " << target_load  << "\r";
    }
    NextString(s);
  }
  std::cerr << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::cerr
      << "Picking took: "
      << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count()
      << " seconds" << std::endl;

  return res;
}

int main(int argc, char** argv) {
  po::options_description desc;
  desc.add_options()
      ("capacity,c", po::value<size_t>()->default_value(FreqCounter::Config{}.initial_capacity), "Initial capacity");

  po::variables_map vm;
  po::command_line_parser parser(argc, argv);
  parser.options(desc);
  try {
    po::store(parser.run(), vm);
  } catch (const po::error& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  po::notify(vm);

  FreqCounter::Config config;
  config.initial_capacity = vm["capacity"].as<size_t>();
  const std::vector<std::string> payload = PickAttackPayload(config);
  for (const std::string& s : payload) {
    std::cout << s << std::endl;
  }

  return 0;
}
