#pragma once

#include <iostream>
#include <memory>
#include <optional>

struct ExecuteContext {
  std::shared_ptr<std::istream> input;
  std::shared_ptr<std::ostream> output;
};

int PrepareContext(int argc, char** argv, ExecuteContext& context);

void Execute(ExecuteContext& context);
