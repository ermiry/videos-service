#ifndef _VIDEOS_UPLOAD_H_
#define _VIDEOS_UPLOAD_H_

#include "errors.h"

#include "models/video.h"

#define VIDEO_ERROR_MAP(XX)							\
	XX(0,	NONE, 				None)				\
	XX(1,	NO_MATCHES, 		No matches)			\
	XX(2,	OUTPUT, 			Output)				\
	XX(3,	MISSING_CHUNK, 		Missing chunk)		\
	XX(4,	BAD_CHUNK, 			Bad chunk)			\
	XX(5,	SERVER_ERROR, 		Server error)

typedef enum VideoError {

	#define XX(num, name, string) VIDEO_ERROR_##name = num,
	VIDEO_ERROR_MAP (XX)
	#undef XX

} VideoError;

extern const char *video_error_to_string (
	const VideoError type
);

extern ServiceError video_to_service_error (
	const VideoError type
);

extern Video *service_video_get_by_name (const char *name);

extern ServiceError service_video_create (
	const char *filename
);

extern ServiceError service_video_complete (
	const char *filename
);

#endif