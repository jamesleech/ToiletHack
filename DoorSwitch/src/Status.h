#ifndef __STATUS_H
#define __STATUS_H

class Status
{
  public:
    Status(unsigned int node);
    unsigned long Counter;
    bool State;
    unsigned int getNode();
    char* toJson();
  private:
    unsigned int _node;
};

#endif /* __STATUS_H */
