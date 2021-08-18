#ifndef _VIDEOS_ROUTES_UPLOAD_H_
#define _VIDEOS_ROUTES_UPLOAD_H_

struct _HttpReceive;
struct _HttpRequest;

// POST /api/videos/upload
extern void videos_upload_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

// POST /api/videos/upload/complete
extern void videos_upload_complete_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

#endif