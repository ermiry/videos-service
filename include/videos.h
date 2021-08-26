#ifndef _VIDEOS_H_
#define _VIDEOS_H_

#include "runtime.h"

#define MONGO_URI_SIZE				256
#define MONGO_APP_NAME_SIZE			32
#define MONGO_DB_SIZE				32

#define VIDEOS_UPLOAD_PATH			"/home/videos/uploads/videos"

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