#ifndef __STATUS_H
#define __STATUS_H

#include <stdio.h>

#define Status_Json_Len 100

class Status
{
  public:
    Status(unsigned int node);
    bool State;
    unsigned int getNode();
    void toJson(char *json);
    int fromJson(char *json);
  private:
    unsigned int _node;
    unsigned long _counter;
};

#endif /* __STATUS_H */
