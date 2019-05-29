/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Default visibility */
#define API_EXPORTED __attribute__((visibility("default")))

/* Debug message logging */
/* #undef ENABLE_DEBUG_LOGGING */

/* Message logging */
/* #undef ENABLE_LOGGING */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `rt' library (-lrt). */
#define HAVE_LIBRT 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

#ifdef _OSX_
/* Darwin backend for Mac */
#define OS_DARWIN
#else
/* Linux backend for Android and generic Linux */
#define OS_LINUX
#endif /* _OSX */

/* Name of package */
#define PACKAGE "libusb"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "libusb"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libusb 1.0.8"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libusb"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.8"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

#ifdef _OSX_
/* Backend handles timeout */
#define USBI_OS_HANDLES_TIMEOUT
#endif /* _OSX_ */

/* timerfd headers available */
/* #undef USBI_TIMERFD_AVAILABLE */

/* Version number of package */
#define VERSION "1.0.8"

/* Use GNU extensions */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE /**/
#endif

/* Android's time.h has no definition of TIMESPEC_TO_TIMEVAL */
#ifndef TIMESPEC_TO_TIMEVAL
#define TIMESPEC_TO_TIMEVAL(tv, ts)				\
	do 											\
	{											\
		(tv)->tv_sec = (ts)->tv_sec;			\
		(tv)->tv_usec = (ts)->tv_nsec / 1000;	\
	}											\
	while (0)
#endif /* TIMESPEC_TO_TIMEVAL */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

#ifdef FORTIFY
/* Fortify (memory checking tool) enabled so all sources must include
 * fortify.h.  Do it here to avoid modifying libusb sources.
 */
#include "fortify.h"
#endif /* FORTIFY */
