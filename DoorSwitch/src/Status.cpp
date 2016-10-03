#include <stdio.h>
#include "Status.h"

#pragma GCC diagnostic ignored "-Wwrite-strings"

Status::Status(unsigned int node) {
  _node = node;
}

unsigned int Status::getNode() {
  return _node;
}

void Status::toJson(char *json, int json_len) {
  snprintf(json, json_len,
    "{\"node\":\"%u\",\"counter\":\"%lu\",\"state\":\"%i\"}",
    _node, ++_counter, State);
  //sprintf("{\"Node\":\"%ld\",\"Counter\":\"%ld\",\"State\":\"%ld\"}", json, _node, Counter, State);
}

int Status::fromJson(char *json, int json_len) {
  return -1;
  // jsmn_parser p;
	// jsmntok_t t[6]; /* we only expect 3 tokens */
  // jsmn_init(&p);
  // int r = jsmn_parse(&p, json, strlen(json), t, sizeof(t)/sizeof(t[0]));
  // if (r < 0) {
	// 	return 1;
	// }
}
