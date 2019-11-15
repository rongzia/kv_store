/*
 * nessDB storage engine
 * Copyright (c) 2011-2012, BohuTANG <overred.shuttler at gmail dot com>
 * All rights reserved.
 * Code is licensed with BSD. See COPYING.BSD file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "level.h"

struct level *level_creat()
{
	return  malloc(sizeof(struct level));
}

void level_set_head(struct level *level, struct level_node *n)
{
	if (n == NULL)
		return;

	level->count++;

	if (level->first == NULL) {
		level->first = n;
	} else {
		n->pre = NULL;
		n->nxt = level->first;
		level->first = n;
	}
}

void level_remove_link(struct level *level, struct level_node *n)
{
	level->count--;

	if (n == NULL)
		return;

	if (n->pre == NULL) {
		if (n->nxt != NULL) {
			level->first = n->nxt;
			n->nxt = NULL;
		}
	} else {
		if (n->nxt == NULL) {
			level->last = n->pre;
			n->pre = NULL;
		} else {
			n->pre->nxt = n->nxt;
			n->nxt = NULL;
			n->pre = NULL;
		}
	}

}	

void level_free_node(struct level *level, struct level_node *n)
{
	if (n) {
		level->used_size -= n->size;
		level_remove_link(level, n);

		if (n->sk.data)
			free(n->sk.data);
		if (n->sv.data)
			free(n->sv.data);
		free(n);
	}
}

void level_free_last(struct level *level, struct ht *h)
{
	struct level_node *n;

	n = level->last;
	ht_remove(h, n->sk.data);
	level_free_node(level, n);
}
