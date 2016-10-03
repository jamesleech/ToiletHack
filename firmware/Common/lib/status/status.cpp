#include <stdio.h>
#include "status.h"

Status::Status(unsigned int nodeId) {
  _nodeId = nodeId;
}

unsigned int Status::getNodeId() {
  return _nodeId;
}

char* Status::toJson() {

  snprintf(_charbuffer, Status_Json_Len,
    "{\"node\":\"%u\",\"ip\":\"%s\",\"counter\":\"%lu\",\"state\":\"%i\"}",
    _nodeId, NodeIp, ++_counter, State);

  return _charbuffer;
}

int Status::fromJson(char *json) {
  return -1;
  // jsmn_parser p;
	// jsmntok_t t[6]; /* we only expect 3 tokens */
  // jsmn_init(&p);
  // int r = jsmn_parse(&p, json, strlen(json), t, sizeof(t)/sizeof(t[0]));
  // if (r < 0) {
	// 	return 1;
	// }
}
