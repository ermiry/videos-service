#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>

#include <bson/bson.h>
#include <mongoc/mongoc.h>

#include <cmongo/collections.h>
#include <cmongo/crud.h>
#include <cmongo/model.h>
#include <cmongo/types.h>

#include "models/video.h"

static void video_doc_parse (
	void *video_ptr, const bson_t *video_doc
);

static CMongoModel *videos_model = NULL;

unsigned int videos_model_init (void) {

	unsigned int retval = 1;

	videos_model = cmongo_model_create (VIDEOS_COLL_NAME);
	if (videos_model) {
		cmongo_model_set_parser (videos_model, video_doc_parse);

		retval = 0;
	}

	return retval;

}

void videos_model_end (void) {

	cmongo_model_delete (videos_model);

}

Video *video_new (void) {

	Video *video = (Video *) malloc (sizeof (Video));
	if (video) {
		(void) memset (video, 0, sizeof (Video));
	}

	return video;

}

void video_delete (void *video_ptr) {

	if (video_ptr) free (video_ptr);

}

Video *video_create (const char *filename) {

	Video *video = video_new ();
	if (video) {
		bson_oid_init (&video->oid, NULL);
		bson_oid_to_string (&video->oid, video->id);

		(void) strncpy (video->name, filename, VIDEO_NAME_SIZE - 1);
		video->name_len = (unsigned int) strlen (video->name);

		video->date = time (NULL);
	}

	return video;

}

void video_generate_filename (
	Video *video,
	const char *base_path, const char *video_name
) {

	(void) snprintf (
		video->filename, VIDEO_PATH_SIZE - 1,
		"%s/%s",
		base_path, video_name
	);

	video->filename_len = (unsigned int) strlen (video->filename);

}

static void video_doc_parse (
	void *video_ptr, const bson_t *video_doc
) {

	Video *video = (Video *) video_ptr;

	bson_iter_t iter = { 0 };
	if (bson_iter_init (&iter, video_doc)) {
		char *key = NULL;
		bson_value_t *value = NULL;
		while (bson_iter_next (&iter)) {
			key = (char *) bson_iter_key (&iter);
			value = (bson_value_t *) bson_iter_value (&iter);

			if (!strcmp (key, "_id")) {
				bson_oid_copy (&value->value.v_oid, &video->oid);
				bson_oid_to_string (&video->oid, video->id);
			}

			else if (!strcmp (key, "name") && value->value.v_utf8.str) {
				(void) strncpy (
					video->name,
					value->value.v_utf8.str,
					VIDEO_NAME_SIZE - 1
				);

				video->name_len = (unsigned int) strlen (video->name);
			}

			else if (!strcmp (key, "filename") && value->value.v_utf8.str) {
				(void) strncpy (
					video->filename,
					value->value.v_utf8.str,
					VIDEO_PATH_SIZE - 1
				);

				video->filename_len = (unsigned int) strlen (video->filename);
			}

			else if (!strcmp (key, "duration"))
				video->duration = value->value.v_double;

			else if (!strcmp (key, "date"))
				video->date = (time_t) bson_iter_date_time (&iter) / 1000;
		}
	}

}

static bson_t *video_to_bson (const Video *video) {

	bson_t *doc = bson_new ();
	if (doc) {
		(void) bson_append_oid (doc, "_id", -1, &video->oid);

		(void) bson_append_utf8 (doc, "name", -1, video->name, -1);
		(void) bson_append_utf8 (doc, "filename", -1, video->filename, -1);

		(void) bson_append_utf8 (doc, "image", -1, video->image, -1);

		(void) bson_append_double (doc, "duration", -1, video->duration);

		(void) bson_append_date_time (doc, "date", -1, video->date * 1000);
	}

	return doc;

}

static bson_t *video_query_by_oid (const bson_oid_t *video_oid) {

	bson_t *query = bson_new ();
	if (query) {
		(void) bson_append_oid (query, "_id", -1, video_oid);
	}

	return query;

}

static bson_t *video_query_by_name (const char *name) {

	bson_t *query = bson_new ();
	if (query) {
		(void) bson_append_utf8 (query, "name", -1, name, -1);
	}

	return query;

}

unsigned int video_get_by_name (
	Video *video,
	const char *name,
	const bson_t *query_opts
) {

	return mongo_find_one_with_opts (
		videos_model,
		video_query_by_name (name), query_opts,
		video
	);

}

unsigned int video_insert_one (const Video *video) {

	return mongo_insert_one (
		videos_model, video_to_bson (video)
	);

}

static inline bson_t *video_update_filename_bson (
	const char *filename
) {

	bson_t *doc = bson_new ();
	if (doc) {
		bson_t set_doc = BSON_INITIALIZER;
		(void) bson_append_document_begin (doc, "$set", -1, &set_doc);
		(void) bson_append_utf8 (&set_doc, "filename", -1, filename, -1);
		(void) bson_append_document_end (doc, &set_doc);
	}

	return doc;

}

unsigned int video_update_filename (
	const Video *video
) {

	return mongo_update_one (
		videos_model,
		video_query_by_oid (&video->oid),
		video_update_filename_bson (video->filename)
	);

}
