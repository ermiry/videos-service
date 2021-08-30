#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cerver/utils/log.h>

#include "errors.h"
#include "files.h"
#include "videos.h"

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

Video *service_video_get_by_name (const char *name) {

	Video *video = video_new ();
	if (video) {
		if (video_get_by_name (video, name, NULL)) {
			video_delete (video);
			video = NULL;
		}
	}

	return video;

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
	const Video *video
) {

	VideoError error = VIDEO_ERROR_NONE;

	error = videos_uploads_merge_files (
		video->name, video->filename
	);

	switch (error) {
		case VIDEO_ERROR_NONE: {
			video_update_filename (video);
		} break;

		// TODO: generate an error
		default: break;
	}

	return error;

}

ServiceError service_video_complete (const char *video_name) {

	ServiceError error = SERVICE_ERROR_NONE;

	Video *video = video_new ();
	if (video) {
		if (!video_get_by_name (video, video_name, NULL)) {
			// generate video filename
			video_generate_filename (
				video,
				VIDEOS_UPLOAD_PATH, video->name
			);

			// merge chunks into final file
			error = video_to_service_error (
				service_video_complete_internal (video)
			);

			video_delete (video);
		}

		else {
			error = SERVICE_ERROR_NOT_FOUND;
		}
	}

	else {
		error = SERVICE_ERROR_SERVER_ERROR;
	}

	return error;

}
