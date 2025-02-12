#include <stdio.h>
#include <stdlib.h>

#include "cjson.h"
#include "utf8JsonReader.h"
#include "utils.h"

const char *JsonValueKindStrings[NULL_VK + 1] = {"Undefined", "Object", "Array",
                                                 "String",    "Number", "True",
                                                 "False",     "Null"};

const char *JsonTokenTypeStrings[UNDEFINED_JT + 1] = {
    "None",     "StartObject", "EndObject", "StartArray", "EndArray",
    "Property", "Comment",     "String",    "Number",     "True",
    "False",    "Null",        "Undefined"};

const char *ReadStateStrings[COMPLETE + 1] = {"Incomplete", "Complete"};

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
  }

  const size_t bufferLength = 50;
  char jsonString[bufferLength];
  size_t bytesRead = fread(jsonString, sizeof(char), 50, file);

  fclose(file);

  char readAmount[100];
  size_t charsRead = 0;
  Utf8JsonReaderState readerState;
  char *jsonPtr = jsonString;
  while (charsRead <= bufferLength) {
    Utf8JsonReaderState readerState = {.readCount = 0,
                                       .tokenType = NONE,
                                       .readState = INCOMPLETE,
                                       .inObject = false,
                                       .inArray = false,
                                       .beforeColon = false};
    size_t currentRead = utf8JsonReader_read(jsonPtr, 50, &readerState);
    jsonPtr += currentRead;
  }

  printf("Reader State: \n");
  printf("  Read Count: %i\n", readerState.readCount);
  printf("  Token Token: %s\n", JsonTokenTypeStrings[readerState.tokenType]);
  printf("  Read State: %s\n", ReadStateStrings[readerState.readState]);
  printf("  In Object: %b\n", readerState.inObject);
  printf("  In Array: %b\n", readerState.inArray);
  printf("  Before Colon: %b\n", readerState.beforeColon);

  return 0;
}
