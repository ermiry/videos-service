#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <cerver/types/string.h>

#include <cerver/collections/dlist.h>

#include <cerver/http/http.h>
#include <cerver/http/request.h>

#include <cerver/utils/log.h>
#include <cerver/utils/utils.h>

#include "files.h"
#include "videos.h"

void videos_uploads_filename_generator (
	const HttpReceive *http_receive,
	const HttpRequest *request
) {

	const MultiPart *mpart = http_request_get_current_mpart (request);

	const char *id = http_request_multi_parts_get_value (request, "id");
	const char *filename = http_request_multi_parts_get_value (request, "filename");

	http_multi_part_set_generated_filename (
		mpart,
		"%s-%s",
		id, filename
	);

}
