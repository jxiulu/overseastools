#ifndef UNITTESTPROTOS_H
#define UNITTESTPROTOS_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 * Generated-by: extract-unit-protos
 *
 ***************************************************************************/
#include "llist.h"
#include "uint-spbset.h"
#include "uint-table.h"
UNITTEST void Curl_node_uremove(struct Curl_llist_node *, void *);
UNITTEST void Curl_uint_spbset_clear(struct uint_spbset *bset);
UNITTEST void Curl_uint_tbl_clear(struct uint_tbl *tbl);
UNITTEST int dedotdotify(const char *input, size_t clen, char **outp);
#endif /* UNITTESTPROTOS_H */
