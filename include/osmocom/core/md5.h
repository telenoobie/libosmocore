/* Declaration of functions and data types used for MD5 sum computing
   library functions.
   Copyright (C) 1995-1997,1999,2000,2001,2004,2005,2011
      Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _MD5_H
#define _MD5_H 1

#include <stdio.h>

#include <limits.h>

#define MD5_DIGEST_SIZE 16
#define MD5_BLOCK_SIZE 64

/* The following contortions are an attempt to use the C preprocessor
   to determine an unsigned integral type that is 32 bits wide.  An
   alternative approach is to use autoconf's AC_CHECK_SIZEOF macro, but
   doing that would require that the configure script compile and *run*
   the resulting executable.  Locally running cross-compiled executables
   is usually not possible.  */

# include <stdint.h>
typedef uint32_t osmo_md5_uint32;
typedef uintptr_t osmo_md5_uintptr;

/* Structure to save state of computation between the single steps.  */
struct osmo_md5_ctx
{
  osmo_md5_uint32 A;
  osmo_md5_uint32 B;
  osmo_md5_uint32 C;
  osmo_md5_uint32 D;

  osmo_md5_uint32 total[2];
  osmo_md5_uint32 buflen;
  union
  {
    char buffer[128];
    osmo_md5_uint32 buffer32[32];
  };
};

/*
 * The following three functions are build up the low level used in
 * the functions `md5_stream' and `md5_buffer'.
 */

/* Initialize structure containing state of computation.
   (RFC 1321, 3.3: Step 3)  */
void osmo_md5_init_ctx (struct osmo_md5_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is necessary that LEN is a multiple of 64!!! */
void osmo_md5_process_block (const void *buffer, size_t len,
				 struct osmo_md5_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is NOT required that LEN is a multiple of 64.  */
void osmo_md5_process_bytes (const void *buffer, size_t len,
				 struct osmo_md5_ctx *ctx);

/* Process the remaining bytes in the buffer and put result from CTX
   in first 16 bytes following RESBUF.  The result is always in little
   endian byte order, so that a byte-wise output yields to the wanted
   ASCII representation of the message digest.

   IMPORTANT: On some systems it is required that RESBUF is correctly
   aligned for a 32 bits value.  */
void *osmo_md5_finish_ctx (struct osmo_md5_ctx *ctx, void *resbuf);


/* Put result from CTX in first 16 bytes following RESBUF.  The result is
   always in little endian byte order, so that a byte-wise output yields
   to the wanted ASCII representation of the message digest.

   IMPORTANT: On some systems it is required that RESBUF is correctly
   aligned for a 32 bits value.  */
void *osmo_md5_read_ctx (const struct osmo_md5_ctx *ctx, void *resbuf);


/* Compute MD5 message digest for bytes read from STREAM.  The
   resulting message digest number will be written into the 16 bytes
   beginning at RESBLOCK.  */
int osmo_md5_stream (FILE *stream, void *resblock);

/* Compute MD5 message digest for LEN bytes beginning at BUFFER.  The
   result is always in little endian byte order, so that a byte-wise
   output yields to the wanted ASCII representation of the message
   digest.  */
void *osmo_md5_buffer (const char *buffer, size_t len,
			   void *resblock);

#endif /* md5.h */
