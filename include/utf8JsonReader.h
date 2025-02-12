#include "types.h"
#include <stdbool.h>

#ifndef UTF8JSONREADER_H
#define UTF8JSONREADER_H

typedef enum { INCOMPLETE, COMPLETE } ReadState;

typedef struct {
  unsigned int readCount;
  JsonTokenType tokenType;
  ReadState readState;
  bool inObject;
  bool inArray;
  bool beforeColon;
} Utf8JsonReaderState;

int utf8JsonReader_read(char *stream, size_t length, Utf8JsonReaderState *state);

#endif // UTF8JSONREADER_H
