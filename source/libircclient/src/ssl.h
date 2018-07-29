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
#ifndef INCLUDE_IRC_SSL_H
#define INCLUDE_IRC_SSL_H

#include "session.h"

#if defined (ENABLE_SSL)

#if defined (_WIN32)
void cb_openssl_locking_function( int mode, int n, const char * file, int line );

// OpenSSL callback to get the thread ID
unsigned long cb_openssl_id_function(void);
int alloc_mutexes( unsigned int total );

#else

// OpenSSL callback to utilize static locks
void cb_openssl_locking_function( int mode, int n, const char * file, int line );

// OpenSSL callback to get the thread ID
unsigned long cb_openssl_id_function();
int alloc_mutexes( unsigned int total );

#endif

int ssl_init_context( irc_session_t * session );


// #if defined (_WIN32)
// 	#define SSLINIT_LOCK_MUTEX(a)		WaitForSingleObject( a, INFINITE )
// 	#define SSLINIT_UNLOCK_MUTEX(a)		ReleaseMutex( a )
// #else
// 	#define SSLINIT_LOCK_MUTEX(a)		pthread_mutex_lock( &a )
// 	#define SSLINIT_UNLOCK_MUTEX(a)		pthread_mutex_unlock( &a )
// #endif

// Initializes the SSL context. Must be called after the socket is created.
int ssl_init( irc_session_t * session );
void ssl_handle_error( irc_session_t * session, int ssl_error );
int ssl_recv( irc_session_t * session );
int ssl_send( irc_session_t * session );

#endif


// Handles both SSL and non-SSL reads.
// Returns -1 in case there is an error and socket should be closed/connection terminated
// Returns 0 in case there is a temporary error and the call should be retried (SSL_WANTS_WRITE case)
// Returns a positive number if we actually read something
int session_socket_read( irc_session_t * session );

// Handles both SSL and non-SSL writes.
// Returns -1 in case there is an error and socket should be closed/connection terminated
// Returns 0 in case there is a temporary error and the call should be retried (SSL_WANTS_WRITE case)
// Returns a positive number if we actually sent something
int session_socket_write( irc_session_t * session );

#endif /* INCLUDE_IRC_SSL_H */