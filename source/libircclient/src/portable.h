/* 
 * Copyright (C) 2004-2012 George Yunaev gyunaev@ulduzsoft.com
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
 * License for more details.
 */
#ifndef INCLUDE_IRC_PORTABLE_H
#define INCLUDE_IRC_PORTABLE_H

#if !defined (_WIN32)
	#include "config.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <unistd.h>
	#include <string.h>
	#include <stdlib.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <arpa/inet.h>	
	#include <netinet/in.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <ctype.h>
	#include <time.h>

	#if defined (ENABLE_THREADS)
		#include <pthread.h>
		typedef pthread_mutex_t		port_mutex_t;

		#if !defined (PTHREAD_MUTEX_RECURSIVE) && defined (PTHREAD_MUTEX_RECURSIVE_NP)
			#define PTHREAD_MUTEX_RECURSIVE		PTHREAD_MUTEX_RECURSIVE_NP
		#endif
	#endif 
#else
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>
	#include <time.h>
	#include <stdio.h>
	#include <stdarg.h>
	#include <string.h>
	#include <stdlib.h>
	#include <sys/stat.h>

	#if defined (ENABLE_THREADS)
		typedef CRITICAL_SECTION	port_mutex_t;
	#endif

	#define inline
	#define snprintf			_snprintf
	#define vsnprintf			_vsnprintf
	#define strncasecmp			_strnicmp
#endif


#if defined (ENABLE_SSL)
	#include <openssl/ssl.h>
	#include <openssl/err.h>
	#include <openssl/rand.h>
#endif


#if defined (ENABLE_THREADS)
int libirc_mutex_init (port_mutex_t * mutex);
void libirc_mutex_destroy (port_mutex_t * mutex);
void libirc_mutex_lock (port_mutex_t * mutex);
void libirc_mutex_unlock (port_mutex_t * mutex);

#else

typedef void *	port_mutex_t;

int libirc_mutex_init (port_mutex_t * mutex);
void libirc_mutex_destroy (port_mutex_t * mutex);
void libirc_mutex_lock (port_mutex_t * mutex);
void libirc_mutex_unlock (port_mutex_t * mutex);

#endif

#if defined (WIN32_DLL)
BOOL WINAPI DllMain (HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved);
#endif

#endif /* INCLUDE_IRC_PORTABLE_H */