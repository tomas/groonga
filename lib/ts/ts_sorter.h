/* -*- c-basic-offset: 2 -*- */
/*
  Copyright(C) 2015 Brazil

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GRN_TS_SORTER_H
#define GRN_TS_SORTER_H

#include "../grn.h"

#include "ts_str.h"
#include "ts_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: Sorting should take into account the order of input records. */

typedef struct {
  int REMOVE_ME;
} grn_ts_sorter_node;

typedef struct {
  grn_obj *table;
  grn_ts_sorter_node *head;
} grn_ts_sorter;

/* grn_ts_sorter_open() creates a sorter. */
grn_rc grn_ts_sorter_open(grn_ctx *ctx, grn_obj *table,
                          grn_ts_sorter_node *head, grn_ts_int offset,
                          grn_ts_int limit, grn_ts_sorter **sorter);

/* grn_ts_sorter_parse() parses a string and creates a sorter. */
grn_rc grn_ts_sorter_parse(grn_ctx *ctx, grn_obj *table,
                           grn_ts_str str, grn_ts_int offset,
                           grn_ts_int limit, grn_ts_sorter **sorter);

/* grn_ts_sorter_close() destroys a sorter. */
grn_rc grn_ts_sorter_close(grn_ctx *ctx, grn_ts_sorter *sorter);

/* grn_ts_sorter_progress() progresses sorting. */
grn_rc grn_ts_sorter_progress(grn_ctx *ctx, grn_ts_sorter *sorter,
                              grn_ts_record *recs, size_t n_recs);

/* grn_ts_sorter_complete() completes sorting. */
grn_rc grn_ts_sorter_complete(grn_ctx *ctx, grn_ts_sorter *sorter,
                              grn_ts_record *recs, size_t n_recs);

typedef struct {
  grn_obj *table;
} grn_ts_sorter_builder;

/* grn_ts_sorter_builder_open() creates a sorter builder. */
grn_rc grn_ts_sorter_builder_open(grn_ctx *ctx, grn_obj *table,
                                  grn_ts_sorter_builder **builder);

/* grn_ts_sorter_builder_close() destroys a sorter builder. */
grn_rc grn_ts_sorter_builder_close(grn_ctx *ctx,
                                   grn_ts_sorter_builder *builder);

#ifdef __cplusplus
}
#endif

#endif /* GRN_TS_SORTER_H */