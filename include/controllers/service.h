#ifndef _VIDEOS_MAIN_H_
#define _VIDEOS_MAIN_H_

struct _HttpResponse;

extern struct _HttpResponse *videos_works;
extern struct _HttpResponse *current_version;

extern struct _HttpResponse *catch_all;

extern unsigned int videos_service_init (void);

extern void videos_service_end (void);

#endif