#ifndef _VIDEOS_H_
#define _VIDEOS_H_

#include "runtime.h"

#define VIDEOS_UPLOAD_PATH		"/home/videos/uploads"

struct _HttpResponse;

extern RuntimeType RUNTIME;

extern unsigned int PORT;

extern unsigned int CERVER_RECEIVE_BUFFER_SIZE;
extern unsigned int CERVER_TH_THREADS;
extern unsigned int CERVER_CONNECTION_QUEUE;

// inits videos main values
extern unsigned int videos_init (void);

// ends videos main values
extern unsigned int videos_end (void);

#endif