/*
 * nessDB storage engine
 * Copyright (c) 2011-2012, BohuTANG <overred.shuttler at gmail dot com>
 * All rights reserved.
 * Code is licensed with BSD. See COPYING.BSD file.
 *
 */

#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "config.h"
#include "bloom.h"

#define HFUNCNUM (3)

struct bloom *bloom_new()
{
	struct bloom *bl = malloc(sizeof(struct bloom));

	bl->size = BLOOM_BITS;
	bl->bitset = calloc((bl->size+1) / CHAR_BIT, sizeof(char));
	bl->hashfuncs = malloc(HFUNCNUM * sizeof(hashfuncs));

	bl->hashfuncs[0] = sax_hash;
	bl->hashfuncs[1] = sdbm_hash;
	bl->hashfuncs[2] = djb_hash;

	return bl;
}

void bloom_add(struct bloom *bloom, const char *k)
{
	int i;
	unsigned int bit;

	if (!k)
		return;

	for (i = 0; i < HFUNCNUM; i++) {
		bit = bloom->hashfuncs[i](k) % bloom->size;
		SETBIT_1(bloom->bitset, bit);
	}

	bloom->count++;
}

int bloom_get(struct bloom *bloom, const char *k)
{
	int i;
	unsigned int bit;

	if (!k)
		return 0;

	for (i = 0; i < HFUNCNUM; i++) {
		bit = bloom->hashfuncs[i](k) % bloom->size;
		if (GETBIT(bloom->bitset, bit) == 0)
			return 0;
	}

	return 1;
}

void bloom_free(struct bloom *bloom)
{
	if (bloom) {
		if (bloom->bitset)
			free(bloom->bitset);

		if(bloom->hashfuncs)
			free(bloom->hashfuncs);

		free(bloom);
	}
}
