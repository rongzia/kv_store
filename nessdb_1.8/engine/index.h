/*
 * nessDB storage engine
 * Copyright (c) 2011-2012, BohuTANG <overred.shuttler at gmail dot com>
 * All rights reserved.
 * Code is licensed with BSD. See COPYING.BSD file.
 *
 */

#ifndef _INDEX_H
#define _INDEX_H

#include <pthread.h>
#include "skiplist.h"
#include "util.h"

struct idx_park{
	struct skiplist *list;
	int lsn;
};

struct index{
	int lsn;
	int db_rfd;
	int meta_lsn;
	int bg_merge_count;
	int mtbl_add_count;
	int mtbl_rem_count;
	int max_mtbl;
	int max_mtbl_size;
	int slowest_merge_count; /* the max merge time's count */
	long long max_merge_time; /* the slowest merge time cost */
	uint64_t bloom_hits;

	char basedir[FILE_PATH_SIZE];
	struct log *log;
	struct sst *sst;
	struct skiplist *list;
	struct idx_park *park;
	pthread_attr_t attr;
	pthread_mutex_t merge_mutex;
};

struct index *index_new(const char *basedir, int max_mtbl_size, int tolog);
int index_add(struct index *idx, struct slice *sk, struct slice *sv);
int index_get(struct index *idx, struct slice *sk, struct slice *sv);
uint64_t index_allcount(struct index *idx);
void index_free(struct index *idx);

#endif
