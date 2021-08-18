#ifndef _VIDEOS_ROUTES_MAIN_H_
#define _VIDEOS_ROUTES_MAIN_H_

struct _HttpReceive;
struct _HttpRequest;

// GET /videos
extern void videos_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

// GET /videos/version
extern void videos_version_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

// GET *
extern void videos_catch_all_handler (
	const struct _HttpReceive *http_receive,
	const struct _HttpRequest *request
);

#endif