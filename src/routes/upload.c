#include <cerver/http/http.h>
#include <cerver/http/request.h>
#include <cerver/http/response.h>

#include "errors.h"

#include "controllers/service.h"
#include "controllers/upload.h"

// POST /api/videos/create
void videos_create_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	const char *video_filename = http_request_multi_parts_get_value (
		request, "filename"
	);

	if (video_filename) {
		service_error_send_response (
			service_video_create (video_filename), http_receive
		);
	}

	else {
		service_error_send_response (
			SERVICE_ERROR_BAD_REQUEST, http_receive
		);
	}

}

// POST /api/videos/upload
void videos_upload_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	// http_request_multi_parts_print (request);

	http_request_multi_part_keep_files ((HttpRequest *) request);

	service_error_send_response (SERVICE_ERROR_NONE, http_receive);

}

// POST /api/videos/upload/complete
void videos_upload_complete_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	// http_request_multi_parts_print (request);

	service_error_send_response (
		service_video_complete (
			http_request_multi_parts_get_value (request, "filename")
		),
		http_receive
	);

}
