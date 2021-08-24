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

static CMongoModel *videos_model = NULL;

unsigned int videos_model_init (void) {

	unsigned int retval = 1;

	videos_model = cmongo_model_create (VIDEOS_COLL_NAME);
	if (videos_model) {
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

unsigned int video_insert_one (const Video *video) {

	return mongo_insert_one (
		videos_model, video_to_bson (video)
	);

}
