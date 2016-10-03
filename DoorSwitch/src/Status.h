#ifndef __STATUS_H
#define __STATUS_H
#include <jsmn.h>

class Status
{
  public:
    Status(unsigned int node);
    bool State;
    unsigned int getNode();
    void toJson(char *json, int json_len);
    int fromJson(char *json, int json_len);
  private:
    unsigned int _node;
    unsigned long _counter;
};

#endif /* __STATUS_H */
