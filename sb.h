#ifndef SB_H
#define SB_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SB_ALLOC
#define SB_ALLOC malloc
#endif

typedef struct {
  size_t size;
  size_t cap;
  char *data;
} StringBuilder;

void sb_init(StringBuilder *sb, size_t size) {
  sb->cap = size;
  sb->data = SB_ALLOC(size);
  assert(sb->data);
}

int sb_append(StringBuilder *sb, char *data, size_t len) {
  if (sb->size + len > sb->cap) {
    sb->data = realloc(sb->data, (sb->cap + len) * 2);
    sb->cap *= 2;
    assert(sb->data);
  }
  memcpy(&sb->data[sb->size], data, len);
  sb->size += len;
  return EXIT_SUCCESS;
}

int sb_appendf(StringBuilder *sb, const char *format, ...) {
  char buf[1024];
  va_list args;
  va_start(args, format);
  int rc = vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);

  if (rc < 0) {
    printf("ERROR: could not append to StringBuilder\n");
    return EXIT_FAILURE;
  }
  return sb_append(sb, buf, strlen(buf));
}

char *sb_string(StringBuilder *sb) {
  char *s = SB_ALLOC(sb->size + 1);
  memcpy(s, sb->data, sb->size);
  s[sb->size] = '\0';
  return s;
}

void sb_free(StringBuilder *sb) {
  free(sb->data);
  sb->size = 0;
  sb->cap = 0;
}
#endif // ! SB_H
