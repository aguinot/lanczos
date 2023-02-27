/*
# This file is part of the Astrometry.net suite.
# Licensed under a 3-clause BSD style license - see LICENSE
*/
#ifndef OS_FEATURES_H
#define OS_FEATURES_H

// Features we use that aren't standard across all supported platforms

#if defined(NEED_CANONICALIZE_FILE_NAME) && (NEED_CANONICALIZE_FILE_NAME == 0)
// don't redeclare it!
#else
char* canonicalize_file_name(const char* fn);
#endif

// This is actually in POSIX1b but may or may not be available.
int fdatasync(int fd);

// Not POSIX; doesn't exist in Solaris 10
#include <sys/param.h>
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif

// isfinite() on Solaris; from
// https://code.google.com/p/redis/issues/detail?id=20
#if defined(__sun) && defined(__GNUC__)

#undef isnan
#define isnan(x) \
      __extension__({ __typeof (x) __x_a = (x); \
      __builtin_expect(__x_a != __x_a, 0); })

#undef isfinite
#define isfinite(x) \
      __extension__ ({ __typeof (x) __x_f = (x); \
      __builtin_expect(!isnan(__x_f - __x_f), 1); })

#undef isinf
#define isinf(x) \
      __extension__ ({ __typeof (x) __x_i = (x); \
      __builtin_expect(!isnan(__x_i) && !isfinite(__x_i), 0); })

#undef isnormal
#define isnormal(x) \
  __extension__ ({ __typeof(x) __x_n = (x); \
                   if (__x_n < 0.0) __x_n = -__x_n; \
                   __builtin_expect(isfinite(__x_n) \
                                    && (sizeof(__x_n) == sizeof(float) \
                                          ? __x_n >= __FLT_MIN__ \
                                          : sizeof(__x_n) == sizeof(long double) \
                                            ? __x_n >= __LDBL_MIN__ \
                                            : __x_n >= __DBL_MIN__), 1); })

#undef HUGE_VALF
#define HUGE_VALF (1e50f)

#endif

#endif
