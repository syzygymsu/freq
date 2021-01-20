#include "cli.h"

#include <fstream>
#include <iostream>
#include <random>

#include <boost/program_options.hpp>

#include "freq_counter.h"

namespace po = boost::program_options;

namespace {

auto ProgramOptionsDescription() {
  struct {
    po::options_description options;
    po::positional_options_description positional;
  } res{{"Options"}, {}};

  res.options.add_options()
      ("help,h", "Display this help message and exit")
      ("input,i", po::value<std::string>()->default_value(""), "Input file")
      ("output,o", po::value<std::string>()->default_value(""), "Output file")
      ("no-randomization,s", "Do not use randomization");

  res.positional.add("input", 1);
  res.positional.add("output", 2);

  return res;
}

template<typename T>
std::shared_ptr<T> ExternSharedPtr(T* ptr) {
  return std::shared_ptr<T>(ptr, [](const T*){});
}

}  // anonymous namespace

int PrepareContext(int argc, char** argv, ExecuteContext& context) {
  const auto desc = ProgramOptionsDescription();
  auto PrintHelp = [&desc]() {
    std::cout << "Usage: freq [OPTION]... [INPUT-FILE] [OUTPUT-FILE]" << std::endl;
    std::cout << "Counts number of each word in given file." << std::endl;
    std::cout << std::endl;
    std::cout << "If INPUT-FILE or OUTPUT-FILE is empty or missing and not specified via options then standard input or output is used." << std::endl;
    std::cout << std::endl;
    std::cout << desc.options << std::endl;
  };

  po::variables_map vm;
  po::command_line_parser parser(argc, argv);
  parser.options(desc.options).positional(desc.positional);
  try {
    po::store(parser.run(), vm);
  } catch (const po::error& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << std::endl;
    PrintHelp();

    return -1;
  }
  po::notify(vm);

  if (vm.count("help")) {
    PrintHelp();

    return 1;
  }

  const std::string if_path = vm["input"].as<std::string>();
  if (if_path.empty()) {
    context.input = ExternSharedPtr(&std::cin);
  } else {
    std::shared_ptr<std::ifstream> if_stream(new std::ifstream(if_path));
    if (!if_stream->good()) {
      std::cerr << "Unable to open input file " << if_path << std::endl;
      std::cerr << strerror(errno) << std::endl;
      return -1;
    }

    context.input = if_stream;
  }

  const std::string of_path = vm["output"].as<std::string>();
  if (of_path.empty()) {
    context.output = ExternSharedPtr(&std::cout);
  } else {
    std::shared_ptr<std::ofstream> of_stream(new std::ofstream(of_path));
    if (!of_stream->good()) {
      std::cerr << "Unable to open output file " << of_path << std::endl;
      std::cerr << strerror(errno) << std::endl;
      return -1;
    }

    context.output = of_stream;
  }

  context.randomization = !vm.count("no-randomization");

  return 0;
}

void Execute(ExecuteContext& context) {
  FreqCounter::Config config;
  if (context.randomization) {
    config.hash_seed = std::random_device()();
  }

  const FreqCountResult result = FreqCounter(config).Count(*context.input);

  std::ostream & out = *context.output;
  for (const FreqCountItem& item : result) {
    out.width(5);
    out << std::right << item.second;

    out.width(0);
    out << std::left << ' ' << item.first << std::endl;
  }
}
