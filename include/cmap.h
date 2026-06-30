#ifndef ARENA_H
#include <arena.h>
#endif

#ifndef CMAP_H
#define CMAP_H
#include <stdio.h>
#include <string.h>

typedef struct Entry {
	const char *key;
	void *value;
	struct Entry *next;
} Entry;

typedef struct cmap {
	int capacity;
	int count;
	Entry **buckets;
	Arena *arena;
} Cmap;

Cmap *map_init(int capacity);
void map_free(Cmap *cmap);

void map_add(Cmap *cmap, const char *key, void *val);
void *map_get(Cmap *cmap, const char *key);
#endif
