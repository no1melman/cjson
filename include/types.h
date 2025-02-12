#include <stdlib.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum {
  OPEN_BRACE = '{',
  CLOSED_BRACE = '}',
  QUOTE = '"',
  COLON = ':',
  OPEN_BRACKET = '[',
  CLOSED_BRACKET = ']',
  TRUE_TOKEN = 't',
  FALSE_TOKEN = 'f',
  NULL_TOKEN = 'n',
  UNDEFINED = 'u'
} CharToken;

typedef enum {
  UNDEFINED_VALUE,
  OBJECT,
  ARRAY,
  STRING,
  NUMBER,
  TRUE,
  FALSE,
  NULL_VK
} JsonValueKind;

typedef enum {
  NONE,
  START_OBJECT,
  END_OBJECT,
  START_ARRAY,
  END_ARRAY,
  PROPERTY,
  COMMENT,
  STRING_JT,
  NUMBER_JT,
  TRUE_JT,
  FALSE_JT,
  NULL_JT,
  UNDEFINED_JT
} JsonTokenType;

typedef struct {
  JsonTokenType TokenType;
  size_t index;
  size_t count;
} Token;

#endif // TYPES_H
