/*
     This file is part of libmicrohttpd
     Copyright (C) 2007, 2008 Christian Grothoff

     libmicrohttpd is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published
     by the Free Software Foundation; either version 2, or (at your
     option) any later version.

     libmicrohttpd is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with libmicrohttpd; see the file COPYING.  If not, write to the
     Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
     Boston, MA 02110-1301, USA.
*/

/**
 * @file test_put_large.c
 * @brief  Testcase for libmicrohttpd PUT operations
 * @author Christian Grothoff
 */

#include "MHD_config.h"
#include "platform.h"
#include <curl/curl.h>
#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef WINDOWS
#include <unistd.h>
#endif

#include "socat.c"

static int oneone;

/**
 * Do not make this much larger since we will hit the
 * MHD default buffer limit and the test code is not
 * written for incremental upload processing...
 */
#define PUT_SIZE (256 * 1024)

static char *put_buffer;

struct CBC
{
  char *buf;
  size_t pos;
  size_t size;
};

static size_t
putBuffer (void *stream, size_t size, size_t nmemb, void *ptr)
{
  unsigned int *pos = ptr;
  unsigned int wrt;

  wrt = size * nmemb;
  if (wrt > PUT_SIZE - (*pos))
    wrt = PUT_SIZE - (*pos);
  memcpy (stream, &put_buffer[*pos], wrt);
  (*pos) += wrt;
  return wrt;
}

static size_t
copyBuffer (void *ptr, size_t size, size_t nmemb, void *ctx)
{
  struct CBC *cbc = ctx;

  if (cbc->pos + size * nmemb > cbc->size)
    return 0;                   /* overflow */
  memcpy (&cbc->buf[cbc->pos], ptr, size * nmemb);
  cbc->pos += size * nmemb;
  return size * nmemb;
}

static int
ahc_echo (void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size,
          void **unused)
{
  int *done = cls;
  struct MHD_Response *response;
  int ret;
  (void) version; (void) unused; /* Unused. Silent compiler warning. */

  if (0 != strcmp ("PUT", method))
    return MHD_NO;              /* unexpected method */
  if ((*done) == 0)
  {
    if (*upload_data_size != PUT_SIZE)
    {
#if 0
      fprintf (stderr,
               "Waiting for more data (%u/%u)...\n",
               *upload_data_size, PUT_SIZE);
#endif
      return MHD_YES;           /* not yet ready */
    }
    if (0 == memcmp (upload_data, put_buffer, PUT_SIZE))
    {
      *upload_data_size = 0;
    }
    else
    {
      return MHD_NO;
    }
    *done = 1;
    return MHD_YES;
  }
  response = MHD_create_response_from_buffer (strlen (url),
                                              (void *) url,
                                              MHD_RESPMEM_MUST_COPY);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}


static int
testInternalPut ()
{
  struct MHD_Daemon *d;
  CURL *c;
  struct CBC cbc;
  unsigned int pos = 0;
  int done_flag = 0;
  char buf[2048];
  int i;

  cbc.buf = buf;
  cbc.size = 2048;
  cbc.pos = 0;
  d = MHD_start_daemon (
    MHD_USE_INTERNAL_POLLING_THREAD /* | MHD_USE_ERROR_LOG */,
    11080,
    NULL, NULL, &ahc_echo, &done_flag, MHD_OPTION_END);
  if (d == NULL)
    return 1;
  zzuf_socat_start ();
  for (i = 0; i < LOOP_COUNT; i++)
  {
    fprintf (stderr, ".");

    c = curl_easy_init ();
    curl_easy_setopt (c, CURLOPT_URL, "http://127.0.0.1:11081/hello_world");
    curl_easy_setopt (c, CURLOPT_WRITEFUNCTION, &copyBuffer);
    curl_easy_setopt (c, CURLOPT_WRITEDATA, &cbc);
    curl_easy_setopt (c, CURLOPT_READFUNCTION, &putBuffer);
    curl_easy_setopt (c, CURLOPT_READDATA, &pos);
    curl_easy_setopt (c, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt (c, CURLOPT_INFILESIZE, (long) PUT_SIZE);
    curl_easy_setopt (c, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt (c, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT);
    if (oneone)
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    else
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt (c, CURLOPT_CONNECTTIMEOUT_MS, CURL_TIMEOUT);
    /* NOTE: use of CONNECTTIMEOUT without also
     *   setting NOSIGNAL results in really weird
     *   crashes on my system! */
    curl_easy_setopt (c, CURLOPT_NOSIGNAL, 1L);
    curl_easy_perform (c);
    curl_easy_cleanup (c);
  }
  fprintf (stderr, "\n");
  zzuf_socat_stop ();
  MHD_stop_daemon (d);
  return 0;
}

static int
testMultithreadedPut ()
{
  struct MHD_Daemon *d;
  CURL *c;
  struct CBC cbc;
  unsigned int pos = 0;
  int done_flag = 0;
  char buf[2048];
  int i;

  cbc.buf = buf;
  cbc.size = 2048;
  cbc.pos = 0;
  d = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION
                        | MHD_USE_INTERNAL_POLLING_THREAD /* | MHD_USE_ERROR_LOG */,
                        11080,
                        NULL, NULL, &ahc_echo, &done_flag, MHD_OPTION_END);
  if (d == NULL)
    return 16;
  zzuf_socat_start ();
  for (i = 0; i < LOOP_COUNT; i++)
  {
    fprintf (stderr, ".");

    c = curl_easy_init ();
    curl_easy_setopt (c, CURLOPT_URL, "http://127.0.0.1:11081/hello_world");
    curl_easy_setopt (c, CURLOPT_WRITEFUNCTION, &copyBuffer);
    curl_easy_setopt (c, CURLOPT_WRITEDATA, &cbc);
    curl_easy_setopt (c, CURLOPT_READFUNCTION, &putBuffer);
    curl_easy_setopt (c, CURLOPT_READDATA, &pos);
    curl_easy_setopt (c, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt (c, CURLOPT_INFILESIZE, (long) PUT_SIZE);
    curl_easy_setopt (c, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt (c, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT);
    if (oneone)
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    else
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt (c, CURLOPT_CONNECTTIMEOUT_MS, CURL_TIMEOUT);
    /* NOTE: use of CONNECTTIMEOUT without also
     *   setting NOSIGNAL results in really weird
     *   crashes on my system! */
    curl_easy_setopt (c, CURLOPT_NOSIGNAL, 1L);
    curl_easy_perform (c);
    curl_easy_cleanup (c);
  }
  fprintf (stderr, "\n");
  zzuf_socat_stop ();
  MHD_stop_daemon (d);
  return 0;
}


static int
testExternalPut ()
{
  struct MHD_Daemon *d;
  CURL *c;
  struct CBC cbc;
  CURLM *multi;
  CURLMcode mret;
  fd_set rs;
  fd_set ws;
  fd_set es;
  int max;
  int running;
  time_t start;
  struct timeval tv;
  unsigned int pos = 0;
  int done_flag = 0;
  char buf[2048];
  int i;

  cbc.buf = buf;
  cbc.size = 2048;
  cbc.pos = 0;
  multi = NULL;
  d = MHD_start_daemon (MHD_NO_FLAG /* | MHD_USE_ERROR_LOG */,
                        11080,
                        NULL, NULL, &ahc_echo, &done_flag,
                        MHD_OPTION_CONNECTION_MEMORY_LIMIT,
                        (size_t) (PUT_SIZE * 4), MHD_OPTION_END);
  if (d == NULL)
    return 256;
  multi = curl_multi_init ();
  if (multi == NULL)
  {
    MHD_stop_daemon (d);
    return 512;
  }
  zzuf_socat_start ();
  for (i = 0; i < LOOP_COUNT; i++)
  {
    fprintf (stderr, ".");

    c = curl_easy_init ();
    curl_easy_setopt (c, CURLOPT_URL, "http://127.0.0.1:11081/hello_world");
    curl_easy_setopt (c, CURLOPT_WRITEFUNCTION, &copyBuffer);
    curl_easy_setopt (c, CURLOPT_WRITEDATA, &cbc);
    curl_easy_setopt (c, CURLOPT_READFUNCTION, &putBuffer);
    curl_easy_setopt (c, CURLOPT_READDATA, &pos);
    curl_easy_setopt (c, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt (c, CURLOPT_INFILESIZE, (long) PUT_SIZE);
    curl_easy_setopt (c, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt (c, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT);
    if (oneone)
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    else
      curl_easy_setopt (c, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt (c, CURLOPT_CONNECTTIMEOUT_MS, CURL_TIMEOUT);
    /* NOTE: use of CONNECTTIMEOUT without also
     *   setting NOSIGNAL results in really weird
     *   crashes on my system! */
    curl_easy_setopt (c, CURLOPT_NOSIGNAL, 1L);



    mret = curl_multi_add_handle (multi, c);
    if (mret != CURLM_OK)
    {
      curl_multi_cleanup (multi);
      curl_easy_cleanup (c);
      zzuf_socat_stop ();
      MHD_stop_daemon (d);
      return 1024;
    }
    start = time (NULL);
    while ((time (NULL) - start < 5) && (c != NULL))
    {
      max = 0;
      FD_ZERO (&rs);
      FD_ZERO (&ws);
      FD_ZERO (&es);
      curl_multi_perform (multi, &running);
      mret = curl_multi_fdset (multi, &rs, &ws, &es, &max);
      if (mret != CURLM_OK)
      {
        curl_multi_remove_handle (multi, c);
        curl_multi_cleanup (multi);
        curl_easy_cleanup (c);
        zzuf_socat_stop ();
        MHD_stop_daemon (d);
        return 2048;
      }
      if (MHD_YES != MHD_get_fdset (d, &rs, &ws, &es, &max))
      {
        curl_multi_remove_handle (multi, c);
        curl_multi_cleanup (multi);
        curl_easy_cleanup (c);
        zzuf_socat_stop ();
        MHD_stop_daemon (d);
        return 4096;
      }
      tv.tv_sec = 0;
      tv.tv_usec = 1000;
      select (max + 1, &rs, &ws, &es, &tv);
      curl_multi_perform (multi, &running);
      if (running == 0)
      {
        curl_multi_info_read (multi, &running);
        curl_multi_remove_handle (multi, c);
        curl_easy_cleanup (c);
        c = NULL;
      }
      MHD_run (d);
    }
    if (c != NULL)
    {
      curl_multi_remove_handle (multi, c);
      curl_easy_cleanup (c);
    }
  }
  fprintf (stderr, "\n");
  zzuf_socat_stop ();
  curl_multi_cleanup (multi);
  MHD_stop_daemon (d);
  return 0;
}


int
main (int argc, char *const *argv)
{
  unsigned int errorCount = 0;
  (void) argc; /* Unused. Silent compiler warning. */

  oneone = (NULL != strrchr (argv[0], (int) '/')) ?
           (NULL != strstr (strrchr (argv[0], (int) '/'), "11")) : 0;
  if (0 != curl_global_init (CURL_GLOBAL_WIN32))
    return 2;
  put_buffer = malloc (PUT_SIZE);
  memset (put_buffer, 1, PUT_SIZE);
  if (MHD_YES == MHD_is_feature_supported (MHD_FEATURE_THREADS))
  {
    errorCount += testInternalPut ();
    errorCount += testMultithreadedPut ();
  }
  errorCount += testExternalPut ();
  free (put_buffer);
  if (errorCount != 0)
    fprintf (stderr, "Error (code: %u)\n", errorCount);
  curl_global_cleanup ();
  return errorCount != 0;       /* 0 == pass */
}
