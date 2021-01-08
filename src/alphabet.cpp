#include "alphabet.h"

#include <algorithm>

char NormalizeChar(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a';
  }
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  }
  return normalized::kSpace;
}

char RestoreChar(char c) {
  if(c >= normalized::kCharMin && c <= normalized::kCharMax) {
    return 'a' + c - normalized::kCharMin;
  }
  return ' ';
}
