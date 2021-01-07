#include "cli.h"

int main(int argc, char** argv) {
  ExecuteContext context;
  int context_result = PrepareContext(argc, argv, context);
  if (context_result != 0) {
    return context_result;
  }

  Execute(context);
  return 0;
}
