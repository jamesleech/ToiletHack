#ifndef UNIT_TEST
#ifndef OSX

#ifndef __BRIDGE_WEBSERVER_H
#define __BRIDGE_WEBSERVER_H

//TODO: what's the best why to do this? maybe progmem
#define ContextType_TextPlain "text/plain"
#define ContextType_TextJson "text/json"
#define ContextType_TextHtml "text/html"

#include <ESP8266WebServer.h>

class BridgeWebServer
{
  public:
    BridgeWebServer(ESP8266WebServer* ws); //TODO: How to we mock this sucker in c++

  private:
    ESP8266WebServer* _webServer;
    void handleNotFound();
    void handleRoot();
    void handleStatus();

    void handleGetNodeStatus();
    void handlePutNodeStatus();

};

#endif // __BRIDGE_WEBSERVER_H
#endif // OSX
#endif // UNIT_TEST
