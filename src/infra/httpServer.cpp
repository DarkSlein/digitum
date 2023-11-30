#include "utils/print.h"
#include "infra/httpServer.h"

AsyncWebServer server(80);

char *fileContent;
String fileContentStr;

void redirectToIndex(AsyncWebServerRequest *request) {
#ifdef CAPTIVE_DOMAIN
  request->redirect(CAPTIVE_DOMAIN);
#else
  request->redirect("http://" + ETH.localIP());
#endif
}

AsyncWebServer& getServer() {
  return server;
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var) {
  if (var == "API_URL")
    return F(ETH.localIP().toString().c_str());
  return String();
}

void readFileFromLittleFS() {
  File file = LittleFS.open("/www/assets/index-48bea978.js.gz", "r");

  if (file) {
    size_t fileSize = file.size();
    println("Heap size free before loading file: ", esp_get_free_heap_size());
    println("File size: ", fileSize);

    if (fileSize > esp_get_free_heap_size()) {
      fileSize = 110000;
    }
    println("Tile size: ", fileSize);

    fileContent = (char *)malloc(fileSize + 1);
    println("Heap size free after malloc: ", esp_get_free_heap_size());

    if (fileContent) {
        file.readBytes(fileContent, fileSize);

        fileContent[fileSize] = '\0';
        file.close();

        println("Heap size free after loading file: ", esp_get_free_heap_size());
        fileContentStr = String(fileContent, fileSize + 1);

    } else {
      Serial.println("Failed to allocate memory for file content");
    }
  } else {
      Serial.println("Failed to open file for reading");
  }
}

void initHttpServer() {
  //readFileFromLittleFS();
  server.serveStatic("/", LittleFS, "/www/");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/www/index.html.gz", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
  });
  /*server.on("/test", [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", fileContentStr);
  });*/
  server.onNotFound(handleNotFound);
  server.begin();
}