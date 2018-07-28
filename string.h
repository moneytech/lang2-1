typedef struct string {
  int len, cap;
  char *s;
} String;

const char *sprintf_alloc(const char *fmt, ...);
