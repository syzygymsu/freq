#pragma once

#include <iostream>
#include <memory>
#include <optional>

struct ExecuteContext {
  bool randomization = true;
  std::shared_ptr<std::istream> input;
  std::shared_ptr<std::ostream> output;
};

int PrepareContext(int argc, char** argv, ExecuteContext& context);

void Execute(ExecuteContext& context);
