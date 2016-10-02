#include "Status.h"

Status::Status(unsigned int node) {
  _node = node;
}

unsigned int Status::getNode() {
  return _node;
}

char* Status::toJson() {
  char* result;

  //snprintf_P(result, 100, "{\"Node\":\"%ld\",\"Counter\":\"%ld\",\"State\":\"%ld\"}", _node,Counter,State);

  return result;
}
