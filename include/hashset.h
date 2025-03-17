#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  const char *key;
  void *value;
} ht_entry;

typedef struct {
  ht_entry *entries;
  size_t capacity;
  size_t length;
} ht;

ht *create_map(void);
void destroy_map(ht *map);
void *map_get(ht *map, const char *key);
const char *map_set(ht *map, const char *key, void *value);
const char *map_set_entry(ht_entry *entries, size_t capacity, const char *key,
                          void *value, size_t *plength);
static bool map_expand(ht *map);

#endif // HASHSET_H
