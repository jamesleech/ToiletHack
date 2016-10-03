#ifndef __STATUS_H
#define __STATUS_H

#define Status_Json_Len 100

class Status
{
  public:
    Status(unsigned int nodeId);
    bool State;
    char NodeIp[17];
    unsigned int getNodeId();
    char* toJson();
    int fromJson(char *json);
    void setNodeIp(const char* ip);

  private:
    unsigned int _nodeId;

    unsigned long _counter;
    char _charbuffer[Status_Json_Len]; //alloc this once and keep it.

};

#endif /* __STATUS_H */
