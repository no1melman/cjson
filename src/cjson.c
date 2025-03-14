#include <stdio.h>
#include <stdlib.h>

#include "cjson.h"
#include "hashset.h"

const char *JsonValueKindStrings[NULL_VK + 1] = {"Undefined", "Object", "Array",
                                                 "String",    "Number", "True",
                                                 "False",     "Null"};

const char *JsonTokenTypeStrings[UNDEFINED_JT + 1] = {
    "None",     "StartObject", "EndObject", "StartArray", "EndArray",
    "Property", "Comment",     "String",    "Number",     "True",
    "False",    "Null",        "Undefined"};

const char *ReadStateStrings[ERROR + 1] = {"Incomplete", "Complete", "Error"};

int main(int argc, char *argv[]) {
  printf("CJSON Project Initialized\n");

  if (argc != 2) {
    fprintf(stderr, "Error: Incorrect number of arguments!\n");
    return 1;
  }

  char *fileName = argv[1];

  printf("  FileName: %s\n", fileName);

  FILE *file = fopen(fileName, "r");

  if (file == NULL) {
    fprintf(stderr, "Error: Can't open file\n");
    return 1;
  }

  ht *jsonData = create_map();
  const size_t bufferLength = 50;
  char jsonString[bufferLength];
  printf("Reading file...");
  size_t bytesRead = fread(jsonString, sizeof(char), bufferLength, file);

  printf("Closing file...");
  fclose(file);

  char readAmount[100];
  size_t charsRead = 0;
  /*Utf8JsonReaderState readerState;*/
  printf("Closing file...");
  char *jsonPtr = jsonString;
  while (charsRead <= bufferLength) {
    readerState = (Utf8JsonReaderState){.readCount = 0,
                                        .tokenType = NONE,
                                        .readState = INCOMPLETE,
                                        .inObject = false,
                                        .inArray = false,
                                        .beforeColon = true};
    printf("  Loop: %llu %s\n", charsRead, jsonPtr);
    size_t currentRead = utf8JsonReader_read(jsonPtr, 50, &readerState);
    printf("  After Read: %llu\n", currentRead);
    jsonPtr += currentRead;
    charsRead += currentRead;

    if (currentRead == 0) {
      break;
    }
  }

  printf("Reader State: \n");
  printf("  Read Count: %i\n", readerState.readCount);
  printf("  Token Type: %s\n", JsonTokenTypeStrings[readerState.tokenType]);
  printf("  Read State: %s\n", ReadStateStrings[readerState.readState]);
  printf("  In Object: %d\n", readerState.inObject);
  printf("  In Array: %d\n", readerState.inArray);
  printf("  Before Colon: %d\n", readerState.beforeColon);

  destroy_map(jsonData);

  return 0;
}
