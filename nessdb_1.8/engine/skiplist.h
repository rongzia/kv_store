/*
 * nessDB storage engine
 * Copyright (c) 2011-2012, BohuTANG <overred.shuttler at gmail dot com>
 * All rights reserved.
 * Code is licensed with BSD. See COPYING.BSD file.
 *
 */

#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <stdint.h>
#include "config.h"

#define MAXLEVEL (15)

typedef enum {ADD,DEL} OPT;

struct skipnode{
	uint64_t  val;
	unsigned opt:24;                   

	char key[NESSDB_MAX_KEY_SIZE];
	struct skipnode *forward[1]; 
	struct skipnode *next;
};

struct skiplist{
	int count;
	int size;
	int level; 

	char pool_embedded[1024];
	struct  skipnode *hdr;                 
	struct pool *pool;
};

struct skiplist *skiplist_new(size_t size);
int skiplist_insert(struct skiplist *list, char *key, uint64_t val, OPT opt);
int skiplist_insert_node(struct skiplist *list, struct skipnode *node);
struct skipnode *skiplist_lookup(struct skiplist *list, char *data);
int skiplist_notfull(struct skiplist *list);
void skiplist_free(struct skiplist *list);


#endif
