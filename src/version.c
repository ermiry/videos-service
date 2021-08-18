#include <cerver/utils/log.h>

#include "version.h"

// print full test version information
void videos_version_print_full (void) {

	cerver_log_both (
		LOG_TYPE_NONE, LOG_TYPE_NONE,
		"\nVideos Service Version: %s", VIDEOS_VERSION_NAME
	);

	cerver_log_both (
		LOG_TYPE_NONE, LOG_TYPE_NONE,
		"Release Date & time: %s - %s", VIDEOS_VERSION_DATE, VIDEOS_VERSION_TIME
	);

	cerver_log_both (
		LOG_TYPE_NONE, LOG_TYPE_NONE,
		"Author: %s\n", VIDEOS_VERSION_AUTHOR
	);

}

// print the version id
void videos_version_print_version_id (void) {

	cerver_log_both (
		LOG_TYPE_NONE, LOG_TYPE_NONE,
		"\nVideos Service Version ID: %s\n", VIDEOS_VERSION
	);

}

// print the version name
void videos_version_print_version_name (void) {

	cerver_log_both (
		LOG_TYPE_NONE, LOG_TYPE_NONE,
		"\nVideos Service Version: %s\n", VIDEOS_VERSION_NAME
	);

}
