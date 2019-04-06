#include "WebServerExtensions.h"

void WebServerExtensions::registerLargeFileEndpoint(String endPointName, String contentType, WebServer &server, const byte *file, int fileSize)
{
    server.on(endPointName, [endPointName, contentType, &server, file, fileSize]() {
        int chunkSize = 1500;
        int currentPosition = 0;

        server.setContentLength(CONTENT_LENGTH_UNKNOWN);

        server.send(200, contentType, "");
        while (currentPosition < fileSize)
        {
            int endPosition = currentPosition + (chunkSize - 1) < fileSize ? currentPosition + (chunkSize - 1) : fileSize - 1;
            int currentChunkSize = endPosition - currentPosition + 1;

            byte *currentChunk = new byte[currentChunkSize];
            for (int i = 0; i < currentChunkSize; i++)
            {
                currentChunk[i] = file[currentPosition + i];
            }

            server.sendContent((char *)currentChunk);

            delete currentChunk;
            currentPosition = endPosition + 1;

            yield();
        }
    });
}