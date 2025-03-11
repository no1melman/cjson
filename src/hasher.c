#include <stdint.h>

uint64_t crc32b(const uint8_t *str) {
  // Source: https://stackoverflow.com/a/21001712
  unsigned int byte, crc, mask;
  int i = 0, j;
  crc = 0xFFFFFFFF;
  while (str[i] != 0) {
    byte = str[i];
    crc = crc ^ byte;
    for (j = 7; j >= 0; j--) {
      mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
    i = i + 1;
  }
  return ~crc;
}
