#include <cmap.h>

Cmap *map_init(int capacity) {
	Arena *arena = arena_init(4096);
	Cmap *cmap = (Cmap *)arena_alloc(arena, sizeof(Cmap));
	cmap->buckets = (Entry **)arena_alloc(arena, capacity * sizeof(Entry *));

	cmap->capacity = capacity;
	cmap->count = 0;
	cmap->arena = arena;

	for (int i = 0; i < capacity; i++) {
		cmap->buckets[i] = NULL;
	}

	return cmap;
}

void map_free(Cmap *cmap) {
	Arena *arena_to_free = cmap->arena;
	arena_free(&arena_to_free);
}

unsigned long hash_function(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

void map_add(Cmap *cmap, const char *key, void *val) {
	unsigned long hash = hash_function(key);
	int idx = hash % cmap->capacity;

	Entry *current = cmap->buckets[idx];
	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			current->value = val;
			return;
		}
		current = current->next;
	}

	Entry *entry = (Entry *)arena_alloc(cmap->arena, sizeof(Entry));
	size_t key_len = strlen(key) + 1;
	char *new_key = (char *)arena_alloc(cmap->arena, key_len);
	memcpy(new_key, key, key_len);
	entry->key = new_key;
	entry->value = val;
	entry->next = cmap->buckets[idx];
	cmap->buckets[idx] = entry;
}

void *map_get(Cmap *cmap, const char *key) {
	unsigned long hash = hash_function(key);
	int idx = hash % cmap->capacity;

	Entry *current = cmap->buckets[idx];
	while (current != NULL) {
		if (strcmp(current->key, key) == 0) {
			return current->value;
		}
		current = current->next;
	}
	return NULL;
}
