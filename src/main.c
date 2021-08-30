#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <signal.h>

#include <cerver/cerver.h>
#include <cerver/version.h>

#include <cerver/http/http.h>
#include <cerver/http/route.h>

#include <cerver/utils/log.h>
#include <cerver/utils/utils.h>

#include "files.h"
#include "videos.h"
#include "version.h"

#include "routes/service.h"
#include "routes/upload.h"

static Cerver *videos_service = NULL;

void end (int dummy) {
	
	if (videos_service) {
		cerver_stats_print (videos_service, false, false);
		cerver_log_msg ("\nHTTP Cerver stats:\n");
		http_cerver_all_stats_print ((HttpCerver *) videos_service->cerver_data);
		cerver_log_line_break ();
		cerver_teardown (videos_service);
	}

	(void) videos_end ();

	cerver_end ();

	exit (0);

}

static void videos_set_videos_routes (HttpCerver *http_cerver) {

	/* register top level route */
	// GET /api/videos
	HttpRoute *main_route = http_route_create (REQUEST_METHOD_GET, "api/videos", videos_handler);
	http_cerver_route_register (http_cerver, main_route);

	/* register videos children routes */
	// GET /api/videos/version
	HttpRoute *version_route = http_route_create (REQUEST_METHOD_GET, "version", videos_version_handler);
	http_route_child_add (main_route, version_route);

	// POST /api/videos/create
	HttpRoute *create_route = http_route_create (REQUEST_METHOD_POST, "create", videos_create_handler);
	http_route_set_modifier (create_route, HTTP_ROUTE_MODIFIER_MULTI_PART);
	http_route_child_add (main_route, create_route);

	// POST /api/videos/upload
	HttpRoute *upload_route = http_route_create (REQUEST_METHOD_POST, "upload", videos_upload_handler);
	http_route_set_modifier (upload_route, HTTP_ROUTE_MODIFIER_MULTI_PART);
	http_route_child_add (main_route, upload_route);

	// POST /api/videos/upload/complete
	HttpRoute *complete_route = http_route_create (REQUEST_METHOD_POST, "upload/complete", videos_upload_complete_handler);
	http_route_set_modifier (complete_route, HTTP_ROUTE_MODIFIER_MULTI_PART);
	http_route_child_add (main_route, complete_route);

}

static void start (void) {

	videos_service = cerver_create (
		CERVER_TYPE_WEB,
		"videos-service",
		PORT,
		PROTOCOL_TCP,
		false,
		CERVER_CONNECTION_QUEUE
	);

	if (videos_service) {
		/*** cerver configuration ***/
		cerver_set_alias (videos_service, "videos");

		cerver_set_receive_buffer_size (videos_service, CERVER_RECEIVE_BUFFER_SIZE);
		cerver_set_thpool_n_threads (videos_service, CERVER_TH_THREADS);
		cerver_set_handler_type (videos_service, CERVER_HANDLER_TYPE_THREADS);

		cerver_set_reusable_address_flags (videos_service, true);

		/*** web cerver configuration ***/
		HttpCerver *http_cerver = (HttpCerver *) videos_service->cerver_data;

		http_cerver_set_uploads_path (http_cerver, VIDEOS_UPLOAD_PATH);
		// http_cerver_set_uploads_delete_when_done (http_cerver, true);
		// http_cerver_set_default_uploads_filename_generator (http_cerver);
		http_cerver_set_uploads_filename_generator (
			http_cerver, videos_uploads_filename_generator
		);

		videos_set_videos_routes (http_cerver);

		// add a catch all route
		http_cerver_set_catch_all_route (http_cerver, videos_catch_all_handler);

		// admin configuration
		http_cerver_enable_admin_routes (http_cerver, true);
		http_cerver_register_admin_file_system (http_cerver, VIDEOS_UPLOAD_PATH);

		if (cerver_start (videos_service)) {
			cerver_log_error (
				"Failed to start %s!",
				videos_service->info->name
			);

			cerver_delete (videos_service);
		}
	}

	else {
		cerver_log_error ("Failed to create cerver!");

		cerver_delete (videos_service);
	}

}

int main (int argc, char const **argv) {

	srand (time (NULL));

	// register to the quit signal
	(void) signal (SIGINT, end);
	(void) signal (SIGTERM, end);

	// to prevent SIGPIPE when writting to socket
	(void) signal (SIGPIPE, SIG_IGN);

	cerver_init ();

	cerver_version_print_full ();

	videos_version_print_full ();

	if (!videos_init ()) {
		start ();
	}

	else {
		cerver_log_error ("Failed to init videos!");
	}

	(void) videos_end ();

	cerver_end ();

	return 0;

}
