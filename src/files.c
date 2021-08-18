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

// traverse uploads dir and find all matching files
static DoubleList *videos_uploads_merge_get_matching_files (
	const char *video_name
) {

	DoubleList *matches = dlist_init (str_delete, str_comparator);

	DIR *dp = opendir (VIDEOS_UPLOAD_PATH);
	if (dp) {
		struct dirent *ep = NULL;
		while ((ep = readdir (dp)) != NULL) {
			if (strcmp (ep->d_name, ".") && strcmp (ep->d_name, "..")) {
				if (strstr (ep->d_name, video_name)) {
					(void) dlist_insert_after (
						matches, dlist_end (matches),
						str_create ("%s/%s", VIDEOS_UPLOAD_PATH, ep->d_name)
					);
				}
			}
		}

		(void) closedir (dp);
	}

	else {
		cerver_log_error (
			"Failed to open dir %s", VIDEOS_UPLOAD_PATH
		);
	}

	return matches;

}

void videos_uploads_merge_files_slow (const char *video_name) {

	char filename_buffer[VIDEO_FILENAME_SIZE] = { 0 };

	DoubleList *files = videos_uploads_merge_get_matching_files (
		video_name
	);

	if (dlist_size (files)) {
		(void) dlist_sort (files, NULL);

		(void) snprintf (
			filename_buffer, VIDEO_FILENAME_SIZE - 1,
			"%s/%s",
			VIDEOS_UPLOAD_PATH, video_name
		);

		FILE *output = fopen (filename_buffer, "wb");

		FILE *file = NULL;
		String *filename = NULL;
		__off_t count = 0;
		struct stat filestats = { 0 };
		for (ListElement *le = dlist_start (files); le; le = le->next) {
			filename = (String *) le->data;

			(void) printf ("%s\n", filename->str);

			file = fopen (filename->str, "rb");
			if (file) {
				// copy contents of file into output
				(void) stat (filename->str, &filestats);

				while (count < filestats.st_size) {
					(void) fputc (fgetc (file), output);
					count += 1;
				}

				cerver_log_success ("Copied %ld bytes!", count);

				count = 0;

				(void) fclose (file);

				// remove file
				// (void) remove (filename->str);
			}

			else {
				cerver_log_error ("Failed to open %s", filename->str);
			}
		}

		(void) fclose (output);
	}

	dlist_delete (files);

}
