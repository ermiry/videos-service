#ifndef _VIDEOS_FILES_H_
#define _VIDEOS_FILES_H_

#define VIDEO_FILENAME_SIZE		256

struct _HttpReceive;
struct _HttpRequest;

extern void videos_uploads_filename_generator (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

#endif