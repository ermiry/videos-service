#ifndef _VIDEOS_FILES_H_
#define _VIDEOS_FILES_H_

#define VIDEO_CHUNK_FILENAME_SIZE		256
#define VIDEO_COMPLETE_FILENAME_SIZE	512

#define VIDEO_CHUNK_COPY_BUFFER_SIZE	16384

struct _HttpReceive;
struct _HttpRequest;

typedef struct VideoChunk {

	unsigned long id;

	int name_len;
	char name[VIDEO_CHUNK_FILENAME_SIZE];

	int complete_name_len;
	char complete_name[VIDEO_COMPLETE_FILENAME_SIZE];

} VideoChunk;

extern void videos_uploads_filename_generator (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

extern void videos_uploads_merge_files_slow (
	const char *video_name
);

#endif