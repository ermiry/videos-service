#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cerver/utils/log.h>

#include "errors.h"

#include "models/video.h"

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
