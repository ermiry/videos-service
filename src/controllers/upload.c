#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cerver/utils/log.h>

#include "errors.h"
#include "files.h"

#include "models/video.h"

#include "controllers/upload.h"

const char *video_error_to_string (const VideoError type) {

	switch (type) {
		#define XX(num, name, string) case VIDEO_ERROR_##name: return #string;
		VIDEO_ERROR_MAP(XX)
		#undef XX
	}

	return video_error_to_string (VIDEO_ERROR_NONE);

}

ServiceError video_to_service_error (const VideoError type) {

	ServiceError error = SERVICE_ERROR_NONE;

	switch (type) {
		case VIDEO_ERROR_NONE: break;

		default:
			error = SERVICE_ERROR_SERVER_ERROR;
			break;
	}

	return error;

}

ServiceError service_video_create (const char *filename) {

	ServiceError error = SERVICE_ERROR_NONE;

	Video *video = video_create (filename);
	if (video) {
		if (!video_insert_one (video)) {
			cerver_log_success ("Created video %s!", video->id);
		}

		else {
			cerver_log_error ("Failed to save video model!");

			error = SERVICE_ERROR_SERVER_ERROR;
		}

		video_delete (video);
	}

	else {
		cerver_log_error ("Failed to create new video!");

		error = SERVICE_ERROR_SERVER_ERROR;
	}

	return error;

}

static inline VideoError service_video_complete_internal (
	const char *filename
) {

	VideoError error = VIDEO_ERROR_NONE;

	// merge chunks into final file
	error = videos_uploads_merge_files (filename);

	// TODO: handle any other error

	return error;

}

ServiceError service_video_complete (const char *filename) {

	return video_to_service_error (
		service_video_complete_internal (filename)
	);

}
