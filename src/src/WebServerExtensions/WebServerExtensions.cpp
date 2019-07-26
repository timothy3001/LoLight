#include "WebServerExtensions.h"

void WebServerExtensions::registerLargeFileEndpoint(String endPointName, String contentType, AsyncWebServer &server, const byte *file, int fileSize)
{
    server.on(endPointName.c_str(), HTTP_GET, [endPointName, contentType, file, fileSize](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginChunkedResponse(contentType, [file, fileSize, endPointName](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
            if (index >= fileSize)
            {
                logDebug(String("Finished sending large file '") + String(endPointName) + String("'"));
                return 0;
            }
            else if (index == 0)
            {
                logDebug(String("Start sending large file '") + String(endPointName) + String("'"));
            }

            int endPosition = index + (maxLen - 1) < fileSize ? index + (maxLen - 1) : fileSize - 1;
            int currentChunkSize = endPosition - index + 1;

            for (int i = 0; i < currentChunkSize; i++)
            {
                buffer[i] = file[index + i];
            }

            return currentChunkSize;
        });

        request->send(response);
    });
}

void WebServerExtensions::registerBootstrap(AsyncWebServer &server)
{
    registerLargeFileEndpoint("/bootstrap.min.css", "text/css; charset=utf-8", server, bootstrapMinCss, sizeof(bootstrapMinCss) / sizeof(byte));
}

void WebServerExtensions::registerNotFound(AsyncWebServer &server)
{
    server.onNotFound([](AsyncWebServerRequest *request) -> void {
        request->send(404, "text/plain", "Not found!");
    });
}

void WebServerExtensions::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("WebServerExtensions: ");
    Serial.println(message);
#endif
}