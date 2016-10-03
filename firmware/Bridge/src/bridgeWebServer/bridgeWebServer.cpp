#ifndef UNIT_TEST
#ifndef OSX

#include "bridgeWebServer.h"

BridgeWebServer::BridgeWebServer(ESP8266WebServer* ws) {
  _webServer = ws;

  _webServer->onNotFound(std::bind(&BridgeWebServer::handleNotFound, this));
  _webServer->on("/", std::bind(&BridgeWebServer::handleRoot, this));
  _webServer->on("/status", std::bind(&BridgeWebServer::handleStatus, this));

  //node function
  _webServer->on("/nodestatus", HTTP_GET, std::bind(&BridgeWebServer::handleGetNodeStatus, this));
  _webServer->on("/nodestatus", HTTP_PUT, std::bind(&BridgeWebServer::handlePutNodeStatus, this));

  _webServer->begin();
}

void BridgeWebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += _webServer->uri();
  message += "\nMethod: ";
  message += (_webServer->method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += _webServer->args();
  message += "\n";

  for (uint8_t i=0; i< _webServer->args(); i++){
    message += " " + _webServer->argName(i) + ": " + _webServer->arg(i) + "\n";
  }

  _webServer->send(404, ContextType_TextPlain, message);
}

void BridgeWebServer::handleRoot() {
  _webServer->send(200, ContextType_TextHtml,
      "<!DOCTYPE html><html><head><title>Toilet Hack Node</title></head><body><p>I'm a hub, one of many.</p></body></html>");
}

void BridgeWebServer::handleStatus() {

  char status[50];
  snprintf(status, 50,
    "{\"status\":\"%ld\"}", 1);

  _webServer->send(200, ContextType_TextJson, status);
}

void BridgeWebServer::handleGetNodeStatus()
{
  _webServer->send(200, ContextType_TextPlain, "TODO");
}

void BridgeWebServer::handlePutNodeStatus(){
  _webServer->send(200, ContextType_TextPlain, "TODO");
}

#endif // OSX
#endif // UNIT_TEST
