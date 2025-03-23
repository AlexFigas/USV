#include "FileSystem.h"
#include <Arduino.h>
#include <Utils.h>

void initializeSPIFFS()
{
    DebugMessagePrintf("FileSystem::initializeSPIFFS()\n");

    if (!SPIFFS.begin(true))
    {
        DebugMessagePrintf("FileSystem::SPIFFS error.\n");
        delay(2000);
        ESP.restart();
    }
    else
    {
        DebugMessagePrintf("FileSystem::SPIFFS OK.\n");
    }
}

String readFileContents(String contentsPath)
{
    DebugMessagePrintf("FileSystem::readFileContents(%s)\n", contentsPath.c_str());

    File file = SPIFFS.open(contentsPath);

    if (!file)
    {
        DebugMessagePrintf("FileSystem::open(%s, READ) failed\n", contentsPath.c_str());
    }

    String fileContents = "";
    while (file.available())
    {
        fileContents += (char)file.read();
    }
    file.close();

    return fileContents;
}

void writeFileContents(String contentsPath, String contents)
{
    DebugMessagePrintf("FileSystem::writeFileContents(%s)\n", contentsPath.c_str());

    File file = SPIFFS.open(contentsPath, FILE_WRITE);

    if (!file)
    {
        DebugMessagePrintf("FileSystem::open(%s, WRITE) failed\n", contentsPath.c_str());
    }

    file.print(contents);
    file.flush();
    file.close();
}

unsigned char* readWavData(char* contentsPath)
{
    DebugMessagePrintf("FileSystem::readWavData(%s)\n", contentsPath);

    unsigned char* result;

    char line[128];
    int numBytesRD;

    File fSound = SPIFFS.open(contentsPath);
    numBytesRD = fSound.readBytesUntil('\n', line, sizeof(line));
    line[numBytesRD] = '\0';
    int dataSize = strtol(line, NULL, 10);

    result = new unsigned char[dataSize];

    int idx = 0;
    while ((numBytesRD = fSound.readBytesUntil('\n', line, sizeof(line))) != 0)
    {
        line[numBytesRD] = '\0';

        char* currentValueRaw;
        unsigned char currentValue;
        currentValueRaw = strtok(line, ", ");
        while (currentValueRaw != NULL)
        {
            currentValue = (unsigned char)strtol(currentValueRaw, NULL, 16);
            result[idx++] = currentValue;
            currentValueRaw = strtok(NULL, ", ");
        }
    }
    fSound.close();

    return result;
}