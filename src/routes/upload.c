#include <cerver/http/http.h>
#include <cerver/http/request.h>
#include <cerver/http/response.h>

#include "errors.h"
#include "files.h"

#include "controllers/service.h"

// POST /api/videos/upload
void videos_upload_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	http_request_multi_parts_print (request);

	http_request_multi_part_keep_files ((HttpRequest *) request);

	service_error_send_response (SERVICE_ERROR_NONE, http_receive);

}

// POST /api/videos/upload/complete
void videos_upload_complete_handler (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	// http_request_multi_parts_print (request);

	// merge chunks into final file
	videos_uploads_merge_files (
		http_request_multi_parts_get_value (request, "filename")
	);

	service_error_send_response (SERVICE_ERROR_NONE, http_receive);

}
