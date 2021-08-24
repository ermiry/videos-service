#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <unistd.h>

#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cerver/collections/dlist.h>

#include <cerver/http/http.h>
#include <cerver/http/request.h>

#include <cerver/utils/log.h>
#include <cerver/utils/utils.h>

#include "files.h"
#include "videos.h"

#include "controllers/upload.h"

static unsigned long video_chunk_get_id (const char *filename);

static VideoChunk *video_chunk_create (
	const char *dirname, const char *filename
) {

	VideoChunk *chunk = (VideoChunk *) malloc (sizeof (VideoChunk));
	if (chunk) {
		chunk->id = video_chunk_get_id (filename);

		(void) strncpy (chunk->name, filename, VIDEO_CHUNK_FILENAME_SIZE - 1);
		chunk->name_len = (int) strlen (chunk->name);

		chunk->complete_name_len = snprintf (
			chunk->complete_name, VIDEO_COMPLETE_FILENAME_SIZE - 1,
			"%s/%s",
			dirname, filename
		);
	}

	return chunk;

}

static void video_chunk_delete (void *chunk_ptr) {

	if (chunk_ptr) free (chunk_ptr);

}

static unsigned long video_chunk_get_id (const char *filename) {

	char string[VIDEO_CHUNK_FILENAME_SIZE] = { 0 };
	(void) strncpy (string, filename, VIDEO_CHUNK_FILENAME_SIZE - 1);

	static const char dlm[2] = { '-', '\0' };

	char *token = NULL;
	char *rest = string;

	// we want the first value
	token = __strtok_r (rest, dlm, &rest);

	return (unsigned long) atol (token);

}

static int video_chunks_comparator (
	const void *a, const void *b
) {

	const unsigned long a_chunk_id = ((VideoChunk *) a)->id;
	const unsigned long b_chunk_id = ((VideoChunk *) b)->id;

	if (a_chunk_id < b_chunk_id) return -1;
	else if (a_chunk_id == b_chunk_id) return 0;
	return 1;

}

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

	DoubleList *matches = dlist_init (video_chunk_delete, video_chunks_comparator);

	DIR *dp = opendir (VIDEOS_UPLOAD_PATH);
	if (dp) {
		struct dirent *ep = NULL;
		while ((ep = readdir (dp)) != NULL) {
			if (strcmp (ep->d_name, ".") && strcmp (ep->d_name, "..")) {
				if (strstr (ep->d_name, video_name)) {
					(void) dlist_insert_after (
						matches, dlist_end (matches),
						video_chunk_create (VIDEOS_UPLOAD_PATH, ep->d_name)
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

static VideoError videos_uploads_merge_files_actual (
	int output_fd, const VideoChunk *chunk
) {

	VideoError error = VIDEO_ERROR_NONE;

	struct stat filestats = { 0 };
	if (!stat (chunk->complete_name, &filestats)) {
		int input_fd = open (chunk->complete_name, O_RDONLY);
		if (input_fd) {
			ssize_t copied = 0;
			unsigned int count = 0;

			// copy contents of input into output
			do {
				copied = sendfile (
					output_fd, input_fd, 0, VIDEO_CHUNK_COPY_BUFFER_SIZE
				);

				count += 1;
			} while (copied > 0);

			#ifdef VIDEOS_DEBUG
			cerver_log_debug (
				"Chunk %lu took %u copies", chunk->id, count
			);
			#endif

			(void) close (input_fd);
		}

		else {
			cerver_log_error (
				"Failed to open %s chunk!", chunk->complete_name
			);

			error = VIDEO_ERROR_BAD_CHUNK;
		}
	}

	else {
		cerver_log_error (
			"Chunk %s was not found!", chunk->complete_name
		);

		error = VIDEO_ERROR_MISSING_CHUNK;
	}

	return error;

}

static VideoError videos_uploads_merge_files_internal (
	const char *video_name, const DoubleList *files
) {

	VideoError error = VIDEO_ERROR_NONE;

	char filename_buffer[VIDEO_COMPLETE_FILENAME_SIZE] = { 0 };

	(void) snprintf (
		filename_buffer, VIDEO_COMPLETE_FILENAME_SIZE - 1,
		"%s/%s",
		VIDEOS_UPLOAD_PATH, video_name
	);

	const int output_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IRGRP | S_IROTH | S_IROTH;
	int output_fd = open (filename_buffer, O_WRONLY | O_CREAT, output_flags);
	if (output_fd) {
		const VideoChunk *chunk = NULL;
		ListElement *le = dlist_start (files);
		unsigned int error = 0;
		do {
			chunk = (const VideoChunk *) le->data;

			#ifdef VIDEOS_DEBUG
			(void) printf ("%s\n", chunk->complete_name);
			#endif

			error = videos_uploads_merge_files_actual (
				output_fd, chunk
			);

			// remove input file
			(void) remove (chunk->complete_name);

			le = le->next;
		} while (!error && le);

		(void) close (output_fd);
	}

	else {
		cerver_log_error (
			"Failed to create %s output!", filename_buffer
		);

		error = VIDEO_ERROR_OUTPUT;
	}

	return error;

}

VideoError videos_uploads_merge_files (const char *video_name) {

	VideoError error = VIDEO_ERROR_NONE;

	DoubleList *files = videos_uploads_merge_get_matching_files (
		video_name
	);

	if (dlist_size (files)) {
		(void) dlist_sort (files, video_chunks_comparator);

		error = videos_uploads_merge_files_internal (
			video_name, files
		);
	}

	else {
		cerver_log_error ("Failed to get %s matches!", video_name);

		error = VIDEO_ERROR_NO_MATCHES;
	}

	dlist_delete (files);

	return error;

}
