#include "hashset.h"
#include "hasher.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

ht *create_map(void) {
  ht *map = malloc(sizeof(ht));
  if (map == NULL) {
    return NULL;
  }

  map->length = 0;
  map->capacity = INITIAL_CAPACITY;

  // allocate zeroed space for entry buckets
  map->entries = calloc(map->capacity, sizeof(ht_entry));
  if (map->entries == NULL) {
    free(map);
    return NULL;
  }

  return map;
}

void destroy_map(ht *map) {
  // free allocated keys
  for (size_t i = 0; i < map->capacity; i++) {
    free((void *)map->entries[i].key);
  }

  free(map->entries);
  free(map);
}

void *map_get(ht *map, const char *key) {
  uint64_t hash = crc32b(key);

  size_t index = (size_t)(hash & (uint64_t)(map->capacity - 1));

  while (map->entries[index].key != NULL) {
    if (strcmp(key, map->entries[index].key) == 0) {
      return map->entries[index].value;
    }
    // if the key wasn't in the slot, move to the next
    index++;
    if (index >= map->capacity) {
      index = 0;
    }
  }

  return NULL;
}

const char *map_set(ht *map, const char *key, void *value) {
  assert(value != NULL);

  if (value == NULL) {
    return NULL;
  }

  if (map->length >= map->capacity / 2) {
    if (!map_expand(map)) {
      return NULL;
    }
  }

  return map_set_entry(map->entries, map->capacity, key, value, &map->length);
}

const char *map_set_entry(ht_entry *entries, size_t capacity, const char *key,
                          void *value, size_t *plength) {
  uint64_t hash = crc32b(key);

  size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      // Found key (it already exists), update value.
      entries[index].value = value;
      return entries[index].key;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  // Didn't find key, allocate+copy if needed, then insert it.
  if (plength != NULL) {
    key = strdup(key);
    if (key == NULL) {
      return NULL;
    }
    (*plength)++;
  }
  entries[index].key = (char *)key;
  entries[index].value = value;
  return key;
}

static bool map_expand(ht *map) {
  // Allocate new entries array.
  size_t new_capacity = map->capacity * 2;
  if (new_capacity < map->capacity) {
    return false; // overflow (capacity would be too big)
  }
  ht_entry *new_entries = calloc(new_capacity, sizeof(ht_entry));
  if (new_entries == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < map->capacity; i++) {
    ht_entry entry = map->entries[i];
    if (entry.key != NULL) {
      map_set_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
    }
  }

  // Free old entries array and update this table's details.
  free(map->entries);
  map->entries = new_entries;
  map->capacity = new_capacity;
  return true;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key) {
  uint64_t hash = FNV_OFFSET;
  for (const char *p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}
