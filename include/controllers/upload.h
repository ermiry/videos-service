#ifndef _VIDEOS_UPLOAD_H_
#define _VIDEOS_UPLOAD_H_

#include "errors.h"

extern ServiceError service_video_create (
	const char *filename
);

#endif