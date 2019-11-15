/*
 * nessDB storage engine
 * Copyright (c) 2011-2012, BohuTANG <overred.shuttler at gmail dot com>
 * All rights reserved.
 * Code is licensed with BSD. See COPYING.BSD file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "ht.h"

static size_t find_slot(struct ht *ht, void *key)
{
	return djb_hash((const char*)key) % ht->cap;
}

struct ht *ht_new(size_t cap)
{
	struct ht *ht = calloc(1, sizeof(struct ht));

	ht->cap = cap;
	ht->nodes = calloc(cap, sizeof(struct ht_node*));

	return ht;
}

void ht_set(struct ht *ht, void *key, void *value)
{
	size_t slot;
	struct ht_node *node;

	slot = find_slot(ht, key);
	node = calloc(1, sizeof(struct ht_node));
	node->next = ht->nodes[slot];
	node->key = key;
	node->value = value;

	ht->nodes[slot] = node;
	ht->size++;
}

void *ht_get(struct ht *ht, void *key)
{
	size_t slot;
	struct ht_node *node;

	slot = find_slot(ht, key);
	node = ht->nodes[slot];

	while (node) {
		if (strcmp(key, node->key) == 0)
			return node->value;

		node=node->next;
	}

	return NULL;
}

void ht_remove(struct ht *ht, void *key)
{
	size_t slot;
	struct ht_node *node, *prev = NULL;

	slot = find_slot(ht,key);
	node = ht->nodes[slot];

	while (node) {
		if (strcmp(key, node->key) == 0) {
			if (prev != NULL)
				prev->next = node->next;
			else
				ht->nodes[slot] = node->next;
			if (node)
				free(node);
			ht->size--;

			return;
		}

		prev = node;
		node = node->next;	
	}
}

void ht_free(struct ht *ht)
{
	free(ht->nodes);
	free(ht);
}
