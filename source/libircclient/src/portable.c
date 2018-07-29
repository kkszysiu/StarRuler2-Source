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
#include "portable.h"

#if defined (ENABLE_THREADS)
int libirc_mutex_init (port_mutex_t * mutex)
{
#if defined (_WIN32)
	InitializeCriticalSection (mutex);
	return 0;
#elif defined (PTHREAD_MUTEX_RECURSIVE)
	pthread_mutexattr_t	attr;

	return (pthread_mutexattr_init (&attr)
		|| pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE)
		|| pthread_mutex_init (mutex, &attr));
#else /* !defined (PTHREAD_MUTEX_RECURSIVE) */

	return pthread_mutex_init (mutex, 0);

#endif /* defined (_WIN32) */
}


void libirc_mutex_destroy (port_mutex_t * mutex)
{
#if defined (_WIN32)
	DeleteCriticalSection (mutex);
#else
	pthread_mutex_destroy (mutex);
#endif
}


void libirc_mutex_lock (port_mutex_t * mutex)
{
#if defined (_WIN32)
	EnterCriticalSection (mutex);
#else
	pthread_mutex_lock (mutex);
#endif
}


void libirc_mutex_unlock (port_mutex_t * mutex)
{
#if defined (_WIN32)
	LeaveCriticalSection (mutex);
#else
	pthread_mutex_unlock (mutex);
#endif
}

#else

	typedef void *	port_mutex_t;

	int libirc_mutex_init (port_mutex_t * mutex) { return 0; }
	void libirc_mutex_destroy (port_mutex_t * mutex) {}
	void libirc_mutex_lock (port_mutex_t * mutex) {}
	void libirc_mutex_unlock (port_mutex_t * mutex) {}

#endif


/*
 * Stub for WIN32 dll to initialize winsock API
 */
#if defined (WIN32_DLL)
BOOL WINAPI DllMain (HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls (hinstDll);
			break;

		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}
#endif
