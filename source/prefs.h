/* prefs.h -- see prefs.c. MIT licensed, see LICENSE. */
#ifndef DD_PREFS_H
#define DD_PREFS_H

/* Returns a malloc'd copy; "" when the key is unset (SharedPreferences
 * getString(key, "") semantics). */
char *prefs_get(const char *key);
void  prefs_set(const char *key, const char *value);

/* Drop the in-memory copy so the next get/set reloads prefs.txt from disk.
 * Used by the tests to exercise the round trip; harmless at runtime. */
void  prefs_reset(void);

#endif
