#ifndef _MODELS_VIDEOS_H_
#define _MODELS_VIDEOS_H_

#include <time.h>

#include <bson/bson.h>

#define VIDEOS_COLL_NAME  		"videos"

#define VIDEO_ID_SIZE			32
#define VIDEO_NAME_SIZE			128
#define VIDEO_PATH_SIZE			512
#define VIDEO_IMAGE_SIZE		256

extern unsigned int videos_model_init (void);

extern void videos_model_end (void);

typedef struct Video {

	bson_oid_t oid;
	char id[VIDEO_ID_SIZE];

	unsigned int name_len;
	char name[VIDEO_NAME_SIZE];

	unsigned int filename_len;
	char filename[VIDEO_PATH_SIZE];

	unsigned int image_len;
	char image[VIDEO_IMAGE_SIZE];

	double duration;

	time_t date;

} Video;

extern Video *video_new (void);

extern void video_delete (void *video_ptr);

extern Video *video_create (const char *filename);

extern unsigned int video_insert_one (const Video *video);

#endif