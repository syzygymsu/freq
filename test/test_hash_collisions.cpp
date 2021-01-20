#include <chrono>
#include <functional>
#include <iostream>
#include <random>

#include <boost/program_options.hpp>

#include "src/alphabet.h"
#include "src/freq_counter.h"

namespace po = boost::program_options;

std::chrono::high_resolution_clock::duration MeasureTime(std::function<void(void)> f) {
  auto start = std::chrono::high_resolution_clock::now();
  f();
  auto end = std::chrono::high_resolution_clock::now();
  return end - start;
}

auto TestAttackPayload(const FreqCounter::Config base_config, const std::string& attack_text) {
  struct {
    std::chrono::high_resolution_clock::duration control_time;
    std::chrono::high_resolution_clock::duration attack_time;
    std::chrono::high_resolution_clock::duration seeded_time;
  } res;

  std::string control_text;
  {
    std::default_random_engine random_engine;
    std::uniform_int_distribution<char> random_char('a', 'z');
    for (const char& c : attack_text) {
      if (normalized::kSpace == NormalizeChar(c)) {
        control_text += ' ';
      } else {
        control_text += random_char(random_engine);
      }
    }
  }

  FreqCounter::Config seeded_config = base_config;
  seeded_config.hash_seed = 42;

  const FreqCounter base_counter(base_config);
  const FreqCounter seeded_counter(seeded_config);

  res.control_time = MeasureTime([&]() {
    std::istringstream ss(control_text);
    base_counter.Count(ss);
  });
  std::cerr
      << "Control run took: "
      << std::chrono::duration_cast<std::chrono::duration<double>>(res.control_time).count()
      << " seconds" << std::endl;

  res.attack_time = MeasureTime([&]() {
    std::istringstream ss(attack_text);
    base_counter.Count(ss);
  });
  std::cerr
      << "Attack run took: "
      << std::chrono::duration_cast<std::chrono::duration<double>>(res.attack_time).count()
      << " seconds" << std::endl;

  res.seeded_time = MeasureTime([&]() {
    std::istringstream ss(attack_text);
    seeded_counter.Count(ss);
  });
  std::cerr
      << "Seeded run took: "
      << std::chrono::duration_cast<std::chrono::duration<double>>(res.seeded_time).count()
      << " seconds" << std::endl;

  return res;
}

template<typename A, typename B>
bool AlmostEquals(A a, B b, double restriction = 2.0) {
  return a * restriction >= b && b * restriction >= a;
}

int main(int argc, char** argv) {
  po::options_description desc;
  desc.add_options()
      ("capacity,c", po::value<size_t>()->default_value(FreqCounter::Config{}.initial_capacity), "Initial capacity")
      ("expected-rate,r", po::value<double>(), "Expected rate");

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
  double expected_rate = vm["expected-rate"].as<double>();

  std::default_random_engine random_engine;
  std::uniform_int_distribution<char> random_char('a', 'z');
  std::string attack_text;
  std::string control_text;
  for (;;) {
    char c;
    std::cin.get(c);
    if (!std::cin.good()) {
      break;
    }
    attack_text += c;
  }

  const auto res = TestAttackPayload(config, attack_text);

  if (!AlmostEquals(res.seeded_time, res.control_time)) {
    std::cerr << "Seeding didn't help" << std::endl;
    return 1;
  }

  std::cerr << "Rate: " << static_cast<double>(res.attack_time.count()) / res.seeded_time.count() << std::endl;
  if (!AlmostEquals(res.seeded_time * expected_rate, res.attack_time)) {
    std::cerr << "Attack time is outside of expected range" << std::endl;
    return 2;
  }

  return 0;
}
