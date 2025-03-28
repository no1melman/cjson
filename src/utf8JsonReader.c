#include "utf8JsonReader.h"
#include "types.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  ReadState state;
  size_t read;
} ReadResult;

size_t readForward(char *stream, char *output, size_t currentIndex,
                   size_t readLength) {
  for (size_t y = 0; y < readLength; y++) {
    output[y] = stream[y + currentIndex];
  }

  return readLength;
}

ReadResult consumeTill(const char *stream, size_t length, char nextChar) {
  size_t count = 0;
  for (size_t i = 0; i < length; i++) {
    const char current = stream[i];
    if (current == nextChar) {
      return (ReadResult) {
        .state = COMPLETE,
        .read = i + 1,
      };
    }
    count++;
  }

  return (ReadResult) {
    .state = INCOMPLETE,
    .read = length,
  };
}

int utf8JsonReader_read(char *stream, size_t length,
                        Utf8JsonReaderState *state) {
  if (!state) {
    fprintf(stderr, "Need to provide an initial value for reader state\n");
    return 0;
  }

  JsonTokenType tokenType;
  ReadState readState;
  char readChars[50];
  unsigned int readLength = 0;
  bool isStringType = false;
  bool isBeforeColon = state->beforeColon;
  bool inArray = false;
  bool inObject = false;
  bool beforeColon = false;
  bool inString = false;

  for (size_t i = 0; i < length; i++) {
    char value = stream[i];

    printf("  Reading char: %c\n", value);
    printf("  Reader State:\n");
    printf("    Token Type: %i\n", tokenType);
    printf("    Read State: %i\n", readState);
    printf("    Read Length: %i\n", readLength);
    printf("    Is String: %d\n", isStringType);
    printf("    Is Before Colon: %d\n", isBeforeColon);

    // if it's a space type char and we're not collecting a string
    if (isspace(value)) {
      readLength++;
      continue;
    }

    bool shouldContinue = false;
    switch (value) {
    case OPEN_BRACE:
      tokenType = START_OBJECT;
      readChars[0] = value;
      readLength = 1;
      readState = COMPLETE;
      inObject = true;
      break;
    case CLOSED_BRACE:
      tokenType = END_OBJECT;
      readChars[0] = value;
      readLength = 1;
      readState = COMPLETE;
      inObject = false;
      break;
    case QUOTE:
      if (isBeforeColon) {
        tokenType = PROPERTY;
      } else {
        tokenType = STRING_JT;
        // this means we've just consumed
        isBeforeColon = true;
      }
      const char *restOfStream = stream + 1;
      const ReadResult result = consumeTill(restOfStream, length - 1, '"');
      if (result.state == INCOMPLETE) {
        inString = true;
        readLength += result.read;
        readState = result.state;
        break;
      }
      readLength++;
      break;
      case COLON:
        isBeforeColon = false;

        break;

    case OPEN_BRACKET:
      tokenType = START_ARRAY;
      readChars[0] = value;
      readLength = 1;
      readState = COMPLETE;
      inArray = true;
      break;
    case CLOSED_BRACKET:
      tokenType = END_ARRAY;
      readChars[0] = value;
      readLength = 1;
      readState = COMPLETE;
      inArray = false;
      break;
    case TRUE_TOKEN: {
      unsigned int trueLength = 4;
      if (i + trueLength > length) {
        readState = INCOMPLETE;
        shouldContinue = false;
        break;
      }
      readLength = readForward(stream, readChars, i, trueLength);

      tokenType = TRUE_JT;
      readState = COMPLETE;
      break;
    }
    case FALSE_TOKEN: {
      unsigned int falseLength = 5;
      if (i + falseLength > length) {
        readState = INCOMPLETE;
        shouldContinue = false;
        break;
      }
      readLength = readForward(stream, readChars, i, falseLength);

      tokenType = FALSE_JT;
      readLength = falseLength;
      readState = COMPLETE;
      break;
    }
    case NULL_TOKEN: {
      unsigned int nullLength = 4;
      if (i + nullLength > length) {
        readState = INCOMPLETE;
        shouldContinue = false;
        break;
      }
      readLength = readForward(stream, readChars, i, nullLength);

      tokenType = NULL_JT;
      readLength = nullLength;
      readState = COMPLETE;
      break;
    }
    case UNDEFINED: {
      unsigned int undefinedLength = 9;
      if (i + undefinedLength > length) {
        readState = INCOMPLETE;
        shouldContinue = false;
        break;
      }
      readLength = readForward(stream, readChars, i, undefinedLength);

      tokenType = UNDEFINED_JT;
      readLength = undefinedLength;
      readState = COMPLETE;
      break;
    }
    }

    if (!shouldContinue) {
      break;
    }
  }

  (*state) = (Utf8JsonReaderState){
      .readCount = readLength,
      .tokenType = tokenType,
      .readState = readState,
      .inObject = inObject,
      .inArray = inArray,
      .beforeColon = false,
  };

  return readLength;
}
