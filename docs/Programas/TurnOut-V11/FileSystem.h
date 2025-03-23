#ifndef _FileSystem_h_
#define _FileSystem_h_

#include <SPIFFS.h>

/**
 * Initialization of the SPIFFS file system
 */
void initializeSPIFFS();

/**
 * Read the contents of file in a single String
 *
 * @param contentsPath - path to the file
 *
 * @return the contends of the file as a String object
 */
String readFileContents(String contentsPath);

/**
 * Write (replace/update) Read the contents of file from the contents of a single String
 *
 * @param contentsPath  - path to the file
 * @param contents      - the data to be written in the file
 */
void writeFileContents(String contentsPath, String contents);

/**
 * Read a wav sound from a file
 *
 * @param contentsPath  - path to the file
 *
 * @return the contends of the file a pointer (array) of unsigned char
 */
unsigned char* readWavData(char* contentsPath);

#endif  // _FileSystem_h_
