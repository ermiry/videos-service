#include <cerver/http/http.h>
#include <cerver/http/request.h>
#include <cerver/http/response.h>

#include "errors.h"

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

	// TODO: merge chunks into final file

	service_error_send_response (SERVICE_ERROR_NONE, http_receive);

}
